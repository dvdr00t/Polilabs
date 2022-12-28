/**
 *				          REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file UsersController.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * UsersController.js implements all the logic related to the single user. 
 * It exploits the UsersService to query the DB. The UsersService acts as an 
 * interface to the DB and it has a set of defined queries.
*/
'use strict';

/* --- MODULE IMPORT --- */
const utils 			 	= require('../utils/writer.js');
const bcrypt        		= require('bcrypt');
const constants 			= require('../utils/constants.js');
var UsersService 			= require('../service/UsersService');

/* --- WEBSOCKET IMPORTS --- */
const WebSocket = require('../components/WebSocket');
const WSMessage = require('../components/WSMessage');


class UsersController {

	/**
     * Constructor of the class UsersController
     * ----------------------------------------
     * @param {Object} dao 
     */
 	constructor (dao) {
		this.usersService = new UsersService(dao);
	}

	/*
	 *	+ -------------------- +
     *  |      FUNCTIONS       |
     *  + -------------------- + 
	*/

	/**
     * Retrieves the specific user from the DB, given 
	 * its username and password.
     * ------------------------------------------------
    */
   	getUser = async (username, password) => {
		try {

			/* retrieving user from DB */
			const result = await this.usersService.getUser(username);
			if (result === undefined) {
                return false;
            }

			/* user exists, let's check the password */
			return new Promise((resolve, reject) => {
				const user = {
					id: 		result.id,
					username:	result.email,
					name: 		result.name
				}
				resolve((bcrypt.compareSync(password, result.hash)) ? user : false);
			});

		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
   	};

	/**
     * Retrieve the list of users from the DB with the
	 * given pagination mechanism.
     * ------------------------------------------------
     *             API: GET /api/users
     * ================================================
    */
	getUsers = async () => {
		try {

			/* retrieving user from DB */
			const users = await this.usersService.getUsers();
			if (users === undefined) {
                return constants.ERROR_404;
            } else {
				return users;
			}

		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
   	};

	/**
     * LAB03 - WEBSOCKET
	 * -------------------------------
	 * Retrieves the active film of the current user and send a message to all 
	 * the logged in clients.
    */
	getActiveFilmUser = async (userId, userName) => {
		try {

			/* checking if the user has an active film or not */
			var loginMessage;
			const film = await this.usersService.getActiveFilmUser(userId);
			if (film !== undefined) {
				loginMessage = new WSMessage('login', parseInt(userId), userName, film.id, film.title);
			} else {
				loginMessage = new WSMessage('login', parseInt(userId), userName, undefined, undefined);
			}

			/* sending messages */
			WebSocket.sendAllClients(loginMessage);
			WebSocket.saveMessage(userId, loginMessage);
			
			return;

		} catch (error) {
			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	}

	/**
     * LAB03 - WEBSOCKET
	 * -------------------------------
	 * Notify all clients that the current user has performed a logout.
    */
	logoutUser = async (userId, userName) => {
		var logoutMessage = new WSMessage('logout', userId, userName, undefined, undefined);

		/* sending messages */
		WebSocket.sendAllClients(logoutMessage);
		WebSocket.deleteMessage(userId);
		
		return;
	}

}

module.exports = UsersController;

