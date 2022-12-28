import { Film } from './models/Film';
import { Review } from './models/Review';
import { User } from './models/User';

const SERVER_URL = 'http://localhost:8080/api/';


/**
 * A utility function for parsing the HTTP response.
 */
function getJson(httpResponsePromise) {
  // server API always return JSON, in case of error the format is the following { error: <message> } 
  return new Promise((resolve, reject) => {
    httpResponsePromise
      .then((response) => {
        if (response.ok) {
         // the server always returns a JSON, even empty {}. Never null or non json, otherwise the method will fail
         response.json()
            .then( json => resolve(json) )
            .catch( err => reject({ error: "Cannot parse server response" }))

        } else {
          // analyzing the cause of error
          response.json()
            .then(obj => 
              reject(obj)
              ) // error msg in the response body
            .catch(err => reject({ error: "Cannot parse server response" })) // something else
        }
      })
      .catch(err => 
        reject({ error: "Cannot communicate"  })
      ) // connection error
  });
}

/**
 * Getting from the server side and returning the list of private films.
 */
const getPrivateFilms = async (pageNumber) => {
  // film.watchDate could be null or a string in the format YYYY-MM-DD
  var path = SERVER_URL + 'films/created';
  if(pageNumber != undefined) path += '?pageNo=' + pageNumber;
  return getJson(fetch(path, { credentials: 'include' })).then( json => { 
    localStorage.setItem('totalPages',  json.totalPages);
    localStorage.setItem('currentPage', json.currentPage);
    localStorage.setItem('totalItems',  json.totalItems);
    localStorage.setItem('filmsType',  'private');
    return json.films.map((film) => new Film(film))
  })
}

/**
 * Getting from the server side and returning the list of private films.
 */
 const getPublicFilms = async (pageNumber) => {
  // film.watchDate could be null or a string in the format YYYY-MM-DD
  var path = SERVER_URL + 'films/public';
  if(pageNumber != undefined) path += '?pageNo=' + pageNumber;
  return getJson(fetch(path, { credentials: 'include' })).then( json => { 
    localStorage.setItem('totalPages',  json.totalPages);
    localStorage.setItem('currentPage', json.currentPage);
    localStorage.setItem('totalItems',  json.totalItems);
    localStorage.setItem('filmsType',  'public');
    return json.films.map((film) => new Film(film))
  })
}


/**
 * Getting from the server side and returning the list of private films.
 */
 const getPublicFilmsToReview = async (pageNumber) => {
  // film.watchDate could be null or a string in the format YYYY-MM-DD
  var path = SERVER_URL + 'films/invited';
  if(pageNumber != undefined) path += '?pageNo=' + pageNumber;
  return getJson(fetch(path, { credentials: 'include' })).then( json => { 
    localStorage.setItem('totalPages',  json.totalPages);
    localStorage.setItem('currentPage', json.currentPage);
    localStorage.setItem('totalItems',  json.totalItems);
    localStorage.setItem('filmsType',  'public');
    return json.films.map((film) => {
      if (film !== null) {
        return new Film(film);
      }
    })
  })
}

/**
 * Getting from the server side and returning the list of film reviews.
 */
 const getFilmReviews = async (filmId, pageNumber) => {
  // film.watchDate could be null or a string in the format YYYY-MM-DD
  var path = SERVER_URL + 'films/' + filmId + '/reviews';
  if(pageNumber != undefined) path += '?pageNo=' + pageNumber;
  return getJson(fetch(path, { credentials: 'include' })).then( json => { 
    localStorage.setItem('totalPages',  json.totalPages);
    localStorage.setItem('currentPage', json.currentPage);
    localStorage.setItem('totalItems',  json.totalItems);
    return json.reviews.map((review) => new Review(review))
  })
}


/** 
 * Getting and returning the definition of the filters from the server-side.
 * This functionality was not requested in the requirements but allows to dinamically change the filters without modifying the front-end.
 */ 
const getFilters = async () => {
  return getJson(
    fetch(SERVER_URL + 'filters', { credentials: 'include' })
  ).then( json => {
    return json;
  })
}

/**
 * Getting and returing a film, specifying its filmId.
 */
const getFilm = async (filmId) => {
  return getJson( fetch(SERVER_URL + 'films/' + filmId, { credentials: 'include' }))
    .then( film => {film.privateFilm = film.private; return new Film(film);} )
}

/**
 * This function wants a film object as parameter. If the filmId exists, it updates the film in the server side.
 */
async function updateFilm(film) {
  delete film.hasDate;
  const response = await fetch(
      SERVER_URL + "films/" + film.id, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
      },
      credentials: 'include',
      body: JSON.stringify(film) // dayjs date is serialized correctly by the .toJSON method override
    })
    if(!response.ok){
      let err = { status: response.status, errObj: response.json };
      throw err; 
    }
    return response.ok;
}
/**
 * This function adds a new film in the back-end library.
 */
function addFilm(film) {
  delete film.hasDate;
  return getJson(
    fetch(SERVER_URL + "films/", {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      credentials: 'include',
      body: JSON.stringify(film) 
    })
  )
}

/**
 * This function deletes a film from the back-end library.
 */
async function deleteFilm(film) {
  const response = await fetch(SERVER_URL + "films/" + film.id, {
    method: 'DELETE',
    credentials: 'include'
  });
  if(!response.ok){
    let err = { status: response.status, errObj: response.json };
    throw err; 
  }
  return response.ok;
}


/**
 * This function issues a new review.
 */
 function issueReview(filmId, user) {
  const jsonUser = JSON.stringify({users: [user.userId]})
  return getJson(
    fetch(SERVER_URL + "films/" + filmId + "/invitations", {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      credentials: 'include',
      body: jsonUser
    })
  )
}


/**
 * This function deletes an issued review.
 */
 async function deleteReview(review) {
  const response = await fetch(SERVER_URL + "films/" + review.filmId + "/reviews/" + review.reviewerId, {
    method: 'DELETE',
    credentials: 'include'
  });
  if(!response.ok){
    let err = { status: response.status, errObj: response.json };
    throw err; 
  }
  return response.ok;
}

/**
 * Getting a review
 */
 const getReview = async (filmId, reviewerId) => {
  return getJson( fetch(SERVER_URL + 'films/' + filmId + "/reviews/" + reviewerId, { credentials: 'include' }))
    .then( review => {return review;} )
}


/**
 * This function updates a review
 */
 async function updateReview(review) {
  const response = await fetch(
      SERVER_URL + "films/public/" + review.filmId + "/reviews/" + review.reviewerId, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
      },
      credentials: 'include',
      body: JSON.stringify(review) // dayjs date is serialized correctly by the .toJSON method override
    })
    if(!response.ok){
      let err = { status: response.status, errObj: response.json };
      throw err; 
    }
    return response.ok;
}

/**
 * This function selects a filmn
 */
async function selectFilm(film) {
  const response = await fetch(
        SERVER_URL + "users/" + localStorage.getItem('userId') + '/selection', {
          method: 'PUT', headers: {'Content-Type': 'application/json',
        },
        credentials: 'include',
        body: JSON.stringify(film)});
  if(!response.ok){
    let err = { status: response.status, errObj: response.json };
    throw err; 
  }
}

/**
 * This function wants email and password inside a "credentials" object.
 * It executes the log-in.
 */
const logIn = async (credentials) => {
  return getJson(fetch(SERVER_URL + 'users/authenticator', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    credentials: 'include',
    body: JSON.stringify(credentials),
  })
  )
};

/**
 * This function is used to verify if the user is still logged-in.
 * It returns a JSON object with the user info.
 */
const getUserInfo = async () => {
  return getJson(fetch(SERVER_URL + 'sessions/current', {
    credentials: 'include',
  })
  )
};

/**
 * This function is used to retrieve the users of the service.
 * It returns a JSON object with the users.
 */


 async function getUsers() {

  const response = await fetch(SERVER_URL + "users", {
    credentials: 'include',
  });
  const responseJson = await response.json();
  if (response.ok) {
      return responseJson.map((u) => new User(u));
  } else {
      let err = { status: response.status, errObj: responseJson };
      throw err; // An object with the error coming from the server
  }

}

/**
 * This function destroy the current user's session and execute the log-out.
 */
const logOut = async() => {
  const jsonEmail = JSON.stringify({email: localStorage.getItem('email')});
  return getJson(fetch(SERVER_URL + 'users/authenticator', {
    method: 'DELETE',
    headers: {
      'Content-Type': 'application/json',
    },
    credentials: 'include',
    body: jsonEmail
  })
  )
}


const API = {logIn, getUserInfo, getUsers, logOut, getPrivateFilms, getPublicFilms, getFilmReviews, updateFilm, deleteFilm, addFilm, getFilm, getFilters, issueReview, deleteReview, getReview, updateReview, getPublicFilmsToReview, selectFilm};
export default API;
