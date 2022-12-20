import React, { useState, useEffect, useContext } from 'react';
import { Row, Col, Button, Toast } from 'react-bootstrap';
import { Link, useParams, useLocation, Outlet } from 'react-router-dom';

import PrivateFilmForm from './PrivateFilmForm';
import PublicFilmForm from './PublicFilmForm';
import ReviewForm from './ReviewForm';
import PrivateFilmTable from './PrivateFilmLibrary';
import PublicFilmTable from './PublicFilmLibrary';
import FilmToReviewTable from './FilmToReviewLibrary';
import FilmReviewTable from './FilmReviewLibrary';
import IssueReviewTable from './IssueReviewLibrary';
import { LoginForm } from './Auth';
import { RouteFilters } from './Filters';

import MessageContext from '../messageCtx';
import API from '../API';

import OnlineList from './OnlineList';
import MiniOnlineList from './MiniOnlineList';

/**
 * Except when we are waiting for the data from the server, this layout is always rendered.
 * <Outlet /> component is replaced according to which route is matching the URL.
 */
function DefaultLayout(props) {

  const location = useLocation();

  var filterId = false;
  if(location.pathname == "/private"){
    filterId = "private";
  } else if(location.pathname == "/public"){
    filterId = "public";
  } else if(location.pathname == "/public/to_review"){
    filterId = "public/to_review";
  } else if(location.pathname == "/online"){
    filterId = "online";
  }

  return (
    <Row className="vh-100">
      <Col md={4} bg="light" className="below-nav" id="left-sidebar">
        <RouteFilters items={props.filters} selected={filterId} />
        <MiniOnlineList onlineList={props.onlineList} />
      </Col>
      <Col md={8} className="below-nav">
        <Outlet />
      </Col>
    </Row>
  );

}

function PrivateLayout() {

  const [films, setFilms] = useState([]);
  const [dirty, setDirty] = useState(true);

  const location = useLocation();

  const {handleErrors} = useContext(MessageContext);


  const { filterLabel } = useParams();
  const filterId = filterLabel || (location.pathname === "/" && 'filter-all');

  // Without this we do not pass the if(dirty) test in the [filterId, dirty] useEffect
  useEffect(() => {
    setDirty(true);
  }, [filterId])


  useEffect(() => {
    if (dirty) {
      API.getPrivateFilms()
        .then(films => {
          setFilms(films);
          setDirty(false);
        })
        .catch(e => handleErrors(e)); 
    }
  }, [dirty]);

  const deleteFilm = (film) => {
    API.deleteFilm(film)
      .then(() => { setDirty(true);})
      .catch(e => handleErrors(e)); 
  }

  // update a film into the list
  const updateFilm = (film) => {
    API.updateFilm(film)
      .then(() => {setDirty(true); })
      .catch(e => {handleErrors(e)}); 
  }

  const refreshFilms = pageNumber => {
    API.getPrivateFilms(pageNumber)
    .then(films => {
      setFilms(films);
      setDirty(false);
    })
    .catch(e => handleErrors(e));
  }

  return (
    <>
      <h1 className="pb-3">Private Films</h1>
      <PrivateFilmTable films={films}
        deleteFilm={deleteFilm} updateFilm={updateFilm} refreshFilms={refreshFilms} />
      <Link to="/private/add" state={{ nextpage: location.pathname }}>
        <Button variant="primary" size="lg" className="fixed-right-bottom" > &#43; </Button>
      </Link>
    </>
  )
}

function AddPrivateLayout(props) {

  const {handleErrors} = useContext(MessageContext);

  // add a film into the list
  const addFilm = (film) => {
    API.addFilm(film)
      .catch(e => handleErrors(e)); 
  }
  return (
    <PrivateFilmForm addFilm={addFilm} />
  );
}

function EditPrivateLayout() {

  const {handleErrors} = useContext(MessageContext);

  const { filmId } = useParams();
  const [film, setFilm] = useState(null);

  useEffect(() => {
    API.getFilm(filmId)
      .then(film => {
        if(film.owner == parseInt(localStorage.getItem('userId')))
          setFilm(film);
      })
      .catch(e => {
        handleErrors(e); 
      }); 
  }, [filmId]);

  // update a film into the list
  const editFilm = (film) => {
    API.updateFilm(film)
      .catch(e => handleErrors(e)); 
  }

  return (
    film ? <PrivateFilmForm film={film} editFilm={editFilm} /> : <><h4 className="pb-3">This task cannot be modified or it does not exists.</h4></>
  );
}

function PublicLayout() {

  const [films, setFilms] = useState([]);
  const [dirty, setDirty] = useState(true);

  const location = useLocation();

  const {handleErrors} = useContext(MessageContext);


  const { filterLabel } = useParams();
  const filterId = filterLabel || (location.pathname === "/" && 'filter-all');

  // Without this we do not pass the if(dirty) test in the [filterId, dirty] useEffect
  useEffect(() => {
    setDirty(true);
  }, [filterId])


  useEffect(() => {
    if (dirty) {
      API.getPublicFilms()
        .then(films => {
          setFilms(films);
          setDirty(false);
        })
        .catch(e => { handleErrors(e);  } ); 
    }
  }, [dirty]);

  const deleteFilm = (film) => {
    API.deleteFilm(film)
      .then(() => { setDirty(true);})
      .catch(e => handleErrors(e)); 
  }

  const updateFilm = (film) => {
    API.updateFilm(film)
      .then(() => { setDirty(true); })
      .catch(e => handleErrors(e)); 
  }

  const refreshFilms = pageNumber => {
    API.getPublicFilms(pageNumber)
    .then(films => {
      setFilms(films);
      setDirty(false);
    })
    .catch(e => handleErrors(e));
  }

  return (
    <>
      <h1 className="pb-3">Public Films</h1>
      <PublicFilmTable films={films}
        deleteFilm={deleteFilm} updateFilm={updateFilm} refreshFilms={refreshFilms} />
      <Link to="/public/add" state={{ nextpage: location.pathname }}>
        <Button variant="primary" size="lg" className="fixed-right-bottom" > &#43; </Button>
      </Link>
    </>
  )
}

function PublicToReviewLayout(props) {

  const [films, setFilms] = useState([]);
  const [dirty, setDirty] = useState(true);

  const location = useLocation();

  const {handleErrors} = useContext(MessageContext);


  const { filterLabel } = useParams();
  const filterId = filterLabel || (location.pathname === "/" && 'filter-all');


  // Without this we do not pass the if(dirty) test in the [filterId, dirty] useEffect
  useEffect(() => {
    setDirty(true);
  }, [filterId])


  useEffect(() => {
    if (dirty) {
      API.getPublicFilmsToReview()
        .then(films => {
          setFilms(films);
          setDirty(false);
        })
        .catch(e => { handleErrors(e);  } ); 
    }
  }, [dirty]);

  const deleteFilm = (film) => {
    API.deleteFilm(film)
      .then(() => { setDirty(true);})
      .catch(e => handleErrors(e)); 
  }

  const updateFilm = (film) => {
    API.updateFilm(film)
      .then(() => { setDirty(true); })
      .catch(e => handleErrors(e)); 
  }

  const selectFilm = (film) => {
    API.selectFilm(film)
      .then(() => { setDirty(true); })
      .catch(e => handleErrors(e)); 
  }

  const refreshFilms = pageNumber => {
    API.getPublicFilmsToReview(pageNumber)
    .then(films => {
      setFilms(films);
      setDirty(false);
    })
    .catch(e => handleErrors(e));
  }

  return (
    <>
      <h1 className="pb-3">Public Films</h1>
      <FilmToReviewTable films={films}
        deleteFilm={deleteFilm} updateFilm={updateFilm} refreshFilms={refreshFilms} selectFilm={selectFilm} onlineList={props.onlineList} />
    </>
  )
}

function AddPublicLayout() {

  const {handleErrors} = useContext(MessageContext);


  // add a film into the list
  const addFilm = (film) => {
    API.addFilm(film)
      .catch(e => handleErrors(e)); 
  }
  return (
    <PublicFilmForm addFilm={addFilm} />
  );
}

function EditPublicLayout() {

  const {handleErrors} = useContext(MessageContext);

  const { filmId } = useParams();
  const [film, setFilm] = useState(null);

  useEffect(() => {
    API.getFilm(filmId)
      .then(film => {
        if(film.owner == parseInt(localStorage.getItem('userId')))
          setFilm(film);
      })
      .catch(e => {
        handleErrors(e); 
      }); 
  }, [filmId]);

  // update a film into the list
  const editFilm = (film) => {
    API.updateFilm(film)
      .catch(e => handleErrors(e)); 
  }


  return (
    film ? <PublicFilmForm film={film} editFilm={editFilm} /> : <><h4 className="pb-3">This task cannot be modified or it does not exists.</h4></>
  );
}


function ReviewLayout() {

  const [reviews, setReviews] = useState([]);
  const [dirty, setDirty] = useState(true);
  const [film, setFilm] = useState()
  const { filmId } = useParams();

  const location = useLocation();

  const {handleErrors} = useContext(MessageContext);


  const { filterLabel } = useParams();
  const filterId = filterLabel || (location.pathname === "/" && 'filter-all');


  // Without this we do not pass the if(dirty) test in the [filterId, dirty] useEffect
  useEffect(() => {
    setDirty(true);
  }, [filterId])


  useEffect(() => {
    if (dirty) {
      API.getFilm(filmId).then(filmObj => {
        setFilm(filmObj)
        API.getFilmReviews(filmId)
        .then(reviews => {
          setReviews(reviews);
          setDirty(false);
          })

      })
      .catch(e => { handleErrors(e);  } ); 
    }
  }, [dirty]);

  const deleteReview = (review) => {
    API.deleteReview(review)
      .then(() => { setDirty(true);})
      .catch(e => handleErrors(e)); 
  }

  const updateReview = (review) => {
    API.updateReview(review)
      .then(() => { setDirty(true); })
      .catch(e => handleErrors(e)); 
  }

  const refreshReviews = (filmId, pageNumber) => {
    API.getFilmReviews(filmId, pageNumber)
    .then(review => {
      setReviews(review);
      setDirty(false);
    })
    .catch(e => handleErrors(e));
  }

    return (
      <>
        <h1 className="pb-3">Review for Film with ID {filmId}</h1>
        {film && 
          <h2>Title: {film.title}</h2>
        }
        <FilmReviewTable reviews={reviews} film={film}
          deleteReview={deleteReview} updateReview={updateReview} refreshReviews={refreshReviews} />
      </>
    );
}

function EditReviewLayout() {

  const {handleErrors} = useContext(MessageContext);

  const { filmId } = useParams();
  const [review, setReview] = useState(null);

  useEffect(() => {
    API.getReview(filmId, localStorage.getItem("userId"))
      .then(review => {
        setReview(review);
      })
      .catch(e => {
        handleErrors(e); 
      }); 
  }, [filmId]);

  // update a film into the list
  const editReview= (review) => {
    API.updateReview(review)
      .catch(e => handleErrors(e)); 
  }


  return (
    review ? <ReviewForm review={review} editReview={editReview} /> : <><h4 className="pb-3">This review cannot be modified or it does not exists.</h4></>
  );
}

function IssueLayout() {

  const [dirty, setDirty] = useState(true);
  const [film, setFilm] = useState()
  const [users, setUsers] = useState([]);
  const [issueMessage, setIssueMessage] = useState('');
  const { filmId } = useParams();

  const location = useLocation();

  const {handleErrors} = useContext(MessageContext);


  const { filterLabel } = useParams();
  const filterId = filterLabel || (location.pathname === "/" && 'filter-all');

  // Without this we do not pass the if(dirty) test in the [filterId, dirty] useEffect
  useEffect(() => {
    setDirty(true);
  }, [filterId])


  useEffect(() => {
    if (dirty) {
      API.getFilm(filmId).then(filmObj => {
        setFilm(filmObj);
        // API.getUsers()
        // .then(users => {
        //   setUsers(users);
        //   setDirty(false);
        // })

      })
      .catch(e => { handleErrors(e);  } ); 
    }
  }, [dirty]);

  const getUsers = async () => {
    try {
      const users = await API.getUsers();
      setUsers(users);
      setDirty(false);  
    } catch (error) {
      handleErrors(error) ;
    }
  }

  const issueReview = (filmId, user) => {
    API.issueReview (filmId, user)
    .then(review => {
      setIssueMessage("The review has been successfully issued.")
      setDirty(false);
    })
    .catch(e => {handleErrors(e);});
  }

    return (
      <>
        <h1 className="pb-3">Issue Review for Film with ID {filmId}</h1>
        {film && 
          <h2>Title: {film.title}</h2>
        }
        <IssueReviewTable filmId={filmId} users={users} getUsers={getUsers} issueReview={issueReview}/>
        <Toast show={issueMessage !== ''} onClose={() => setIssueMessage('')} delay={10000} autohide>
            <Toast.Body>{ issueMessage }</Toast.Body>
        </Toast>
      </>
    );
}



function NotFoundLayout() {
  return (
    <>
      <h2>This is not the route you are looking for!</h2>
      <Link to="/">
        <Button variant="primary">Go Home!</Button>
      </Link>
    </>
  );
}

function LoginLayout(props) {
  return (
    <Row className="vh-100">
      <Col md={12} className="below-nav">
        <LoginForm login={props.login} />
      </Col>
    </Row>
  );
}

/**
 * This layout shuld be rendered while we are waiting a response from the server.
 */
function LoadingLayout(props) {
  return (
    <Row className="vh-100">
      <Col md={4} bg="light" className="below-nav" id="left-sidebar">
      </Col>
      <Col md={8} className="below-nav">
        <h1>Film Manager ...</h1>
      </Col>
    </Row>
  )
}

function OnlineLayout(props) {

  
  

  const location = useLocation();

  const {handleErrors} = useContext(MessageContext);
  const { filterLabel } = useParams();
  const filterId = filterLabel || (location.pathname === "/" && 'filter-all');
  var onlineList = props.onlineList;

  

  // Without this we do not pass the if(dirty) test in the [filterId, dirty] useEffect
  useEffect(() => {
    //setDirty(true);
  }, [filterId])


  return (
    <>
      <h1 className="pb-3">Online Users</h1>
                    <div className="user">
                        <OnlineList  usersList={onlineList}/>
                   </div>  
    </>
  )
}



export { DefaultLayout, AddPrivateLayout, EditPrivateLayout, AddPublicLayout, EditPublicLayout, EditReviewLayout, NotFoundLayout, LoginLayout, PrivateLayout, PublicLayout, PublicToReviewLayout, ReviewLayout, IssueLayout, LoadingLayout, OnlineLayout }; 