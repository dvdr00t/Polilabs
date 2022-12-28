import 'bootstrap/dist/css/bootstrap.min.css';
import 'bootstrap-icons/font/bootstrap-icons.css';
import './App.css';

import React, { useState, useEffect, useContext, useRef} from 'react';
import { Container, Toast} from 'react-bootstrap/';
import { BrowserRouter, Routes, Route, Navigate, useLocation } from 'react-router-dom';

import { PrivateLayout, PublicLayout, PublicToReviewLayout, ReviewLayout, AddPrivateLayout, EditPrivateLayout,  AddPublicLayout, EditPublicLayout, EditReviewLayout, IssueLayout, DefaultLayout, NotFoundLayout, LoginLayout, LoadingLayout, OnlineLayout } from './components/PageLayout';
import { Navigation } from './components/Navigation';

import MessageContext from './messageCtx';
import API from './API';

const url = 'ws://localhost:50012'

/* --- MODULE IMPORT --- */
const MQTT = require('mqtt');

/* --- CONNECTING TO THE MQTT BROKER --- */
const clientId = 'mqttjs_' + Math.random().toString(16).substr(2, 8);
const OPTIONS = {
    keepalive: 30,  // NB: measured in seconds, not milliseconds!
    clientId: clientId,
    clean: true,    // client wants to start fresh with a new session, rather than resume an existing session.
    reconnectPeriod: 60 * 1000,
    connectTimeout: 30 * 1000,
    will: {
        topic: 'will_message',
        payload: 'connection closed abnormally',
        qos: 0,
        retain: false
    },
    rejectUnauthorized: false
}
const client = MQTT.connect("ws://localhost:9090", OPTIONS);

function App() {

	const [message, setMessage] = useState('');
	// If an error occurs, the error message will be shown in a toast.
	const handleErrors = (err) => {
		let msg = '';
		if (err.error) msg = err.error;
		else if (String(err) === "string") msg = String(err);
		else msg = "Unknown Error";
		setMessage(msg); // WARN: a more complex application requires a queue of messages. In this example only last error is shown.
	}

	return (
		<BrowserRouter>
		<MessageContext.Provider value={{ handleErrors }}>
			<Container fluid className="App">
			<Routes>
				<Route path="/*" element={<Main />} />
			</Routes>
			<Toast show={message !== ''} onClose={() => setMessage('')} delay={4000} autohide>
				<Toast.Body>{ message }</Toast.Body>
			</Toast>
			</Container>
		</MessageContext.Provider>
		</BrowserRouter>
	)
}

function Main() {

	// This state is used for displaying a LoadingLayout while we are waiting an answer from the server.
	const [loading, setLoading] = useState(true);
	// This state keeps track if the user is currently logged-in.
	const [loggedIn, setLoggedIn] = useState(false);
	// This state contains the user's info.
	const [user, setUser] = useState(null);
	// This state contains the possible selectable filters.
	const [filters, setFilters] = useState({});
	//This state contains the online list.
	const [onlineList, setOnlineList] = useState([]);
	// This state is used to manage the MQTT topics
	const [filmSelections, setFilmSelections] = useState([]);
	const [subscribedTopics, setSubscribedTopics] = useState([]);

	// Error messages are managed at context level (like global variables)
	const {handleErrors} = useContext(MessageContext);

	const location = useLocation();


	/**
	 * LAB03 - WEBSOCKET MANAGEMENT
	 * LAB05 - MQTT MANAGEMENT
	 */
	let socket = useRef(null);
	useEffect(() => {

		/* --- WEB SOCKETS CONFIGURATION --- */

		let ws = new WebSocket(url);
		ws.onopen = () => {
			ws.send("Message from client");
		};
		ws.onerror = (error) => {
			console.log(error);
		};
		ws.onmessage = (msg) => {
			try {
				messageReceived(msg);
			} catch (error) {
				console.log(error);
			}
		};

		/* callback function */
		const messageReceived = (msg) => {
			
			/* parsing information */
			const data = JSON.parse(msg.data.toString());

			/* case of login */
			if (data.typeMessage === 'login') {
				setOnlineList(currentArray => {
					
					/* building new array */
					const newArray = [...currentArray];
					let found = false;

					/* adding new user */
					for (let i = 0; i < newArray.length; i++) {
						if (newArray[i].userId === data.userId) {
							found = true;
						}
					}
					if (!found) newArray.push(data);

					/* updating list */
					return newArray
				});
			}

			/* case of logout */
			if (data.typeMessage === 'logout') {
				setOnlineList(currentArray => {
					
					/* removing user */
					const newArray = [...currentArray];
					for (let i = 0; i < newArray.length; i++) {
						if (newArray[i].userId === data.userId) {
							newArray.splice(i, 1);
						}
					}

					/* updating list */
					return newArray;
				});
			}

			/* case of film active selection */
			if (data.typeMessage === 'update') {
				setOnlineList(currentArray => {

					/* building new array */
					const newArray = [...currentArray];
					let found = false;

					/* searching target user */
					for (let i = 0; i < newArray.length; i++) {
						if (newArray[i].userId === data.userId) {
							found = true;
							newArray[i] = data;
							return newArray;
						}
					}
					if (!found) newArray.push(data);

					/* updating list */
					return newArray;
				});
			}
		}

		/* binding socket */
		socket.current = ws;


		/* --- MQTT CONFIGURATION --- */
		const displayFilmSelection = (topic, message) => {
			setFilmSelections(currentArray => {
				
				/* building new array */
				const newArray = [...currentArray];
				
				/* building object */
				let objectStatus = {
					filmId: topic,
					userName: message.userName,
					status: message.status
				};

				/* updating new array */
				const index = newArray.findIndex(film => film.filmId == topic);
				if (index === -1) {
					newArray.push(objectStatus);
				} else {
					newArray[index] = objectStatus;
				}

				return newArray;
			});
		}

		client.on('error', (error) => {
			console.log(error);
			client.end();
		});
		client.on('connect', () => {
			console.log(`client ${clientId} connected.`);
		});
		client.on('message', (topic, message) => {
			try {
				const parsedMessage = JSON.parse(message);
				if (parsedMessage.status === "deleted") {
					client.unsubscribe(topic);
				} 
				displayFilmSelection(topic, parsedMessage);
			} catch (error) {
				console.log(error);
			}
		});
		client.on('close', () => {
			console.log(`client ${clientId} disconnected.`);
		});


	}, []);


	useEffect(() => {
		const init = async () => {
			setLoading(true);

			// Define filters 
			const filters = ['private', 'public', 'public/to_review', 'online'];
			setFilters(filters);
			// NOTE: this method is called before getUserInfo because if not logged an exception is rised and it would be skipped

			//const user = await API.getUserInfo();  // here you have the user info, if already logged in
			if(localStorage.getItem('userId') != undefined){
			setUser(localStorage.getItem('userId'));
			setLoggedIn(true);
			setLoading(false);
			} else {
			setUser(null);
			setLoggedIn(false);
			setLoading(false);
			} 
		};
		init();
	}, []);  // This useEffect is called only the first time the component is mounted.

	/**
	 * This function handles the login process.
	 * It requires a email and a password inside a "credentials" object.
	 */
	const handleLogin = async (credentials) => {
		try {
			const parsedCredentials = {
				username: credentials.email,
				password: credentials.password
			}
			const user = await API.logIn(parsedCredentials);
			localStorage.setItem('userId', user.id);
			localStorage.setItem('username', user.name);
			localStorage.setItem('email', user.email);
			setUser(user);
			setLoggedIn(true);
		} catch (err) {
		// error is handled and visualized in the login form, do not manage error, throw it
		throw err;
		}
	};

	/**
	 * This function handles the logout process.
	 */ 
	const handleLogout = async () => {
		await API.logOut();
		
		setLoggedIn(false);
		setUser(null);
		localStorage.removeItem('userId');
		localStorage.removeItem('username');
		localStorage.removeItem('email');
	};


	return (
		<>
		<Navigation logout={handleLogout} user={user} loggedIn={loggedIn} />

		<Routes>
			<Route path="/" element={
			loading ? <LoadingLayout />
				: loggedIn ? <DefaultLayout filters={filters} onlineList={onlineList}/>
				: <Navigate to="/login" replace state={location} />
			} >
			<Route index element={<PrivateLayout/>} />
			<Route path="private" element={<PrivateLayout/>} />
			<Route path="private/add" element={<AddPrivateLayout />} />
			<Route path="private/edit/:filmId" element={<EditPrivateLayout />} />
			<Route path="public" element={<PublicLayout/>} />
			<Route path="public/add" element={<AddPublicLayout />} />
			<Route path="public/edit/:filmId" element={<EditPublicLayout />} />
			<Route path="public/:filmId/reviews" element={<ReviewLayout/>} />
			<Route path="public/:filmId/reviews/complete" element={<EditReviewLayout/>} />
			<Route path="public/:filmId/issue" element={<IssueLayout/>} />
			<Route path="public/to_review" element={<PublicToReviewLayout onlineList={onlineList} filmSelections={filmSelections} client={client} subscribedTopics={subscribedTopics} setSubscribedTopics={setSubscribedTopics}/>} />
			<Route path="online" element={<OnlineLayout onlineList={onlineList}/>} />
			<Route path="*" element={<NotFoundLayout />} />
			</Route>

			<Route path="/login" element={!loggedIn ? <LoginLayout login={handleLogin} /> : <Navigate replace to='/' />} />
		</Routes>
		</>
	);
}

export default App;
