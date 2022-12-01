/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file FilmController.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * FilmController.js implements all the logic related to the single film. 
 * It exploits the filmService to query the DB. The filmService acts as an 
 * interface to the DB and it has a set of defined queries.
*/
'use strict';

/* --- MODULE IMPORT --- */
const utils 				= require('../utils/writer.js');
const constants 			= require('../utils/constants.js');
var FilmService 			= require('../service/FilmService');

class FilmsCollectionController {

	/**
     * Constructor of the class FilmsCollectionController
     * --------------------------------------------------
     * @param {Object} dao 
     */
 	constructor (dao) {
		this.filmService = new FilmService(dao);
	}

	/*
	 *	+ -------------------- +
     *  |      FUNCTIONS       |
     *  + -------------------- + 
	*/

	/**
     * Retrieves the specific film from the DB, given 
	 * its filmId.
     * ------------------------------------------------
     *          API: GET /api/films/:filmId
     * ================================================
    */
	getFilm = async (filmId, ownerId) => {

		try {
			
			/* retrieving the specific film given the filmId */
			const film = await this.filmService.getFilm(filmId);			
  
			/* checking ownership */
			if (film.private === 1) {
				if (ownerId === undefined || film.owner !== ownerId) {
					return constants.ERROR_403;
				}
			}
			return utils.writeJson(200, film);
	
		} catch (error) {
	
			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	
	};

	/**
     * Edit the specified film (the owner must be the logged in user)
     * --------------------------------------------------------------
     *      		API: PUT /api/films/:filmId
     * ==============================================================
    */
    editFilm = async (userId, filmId, body) => {

		try {

			/* retrieve film from the DB */
			const oldFilm = await this.filmService.getFilm(filmId);
			if (oldFilm === undefined) {
				return constants.ERROR_404;
			}
			
			/* checking constraints for new film */
			if (oldFilm.private != body.private) {
				return constants.ERROR_400;
			} else if (oldFilm.owner != userId) {
				return constants.ERROR_403;
			}

			/* building the film */
			const film = {
				title: body.title,
				owner: userId,
				private: body.private,
				watchDate: body.watchDate ? body.watchDate : null,
				rating: body.rating ? body.rating : null,
				favorite: body.favorite ? body.favorite : null
			}
			
			await this.filmService.editFilm(filmId, film);
			return constants.MESSAGE_204;
			
		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	};


	/**
     * Remove the specified film (the owner must be the logged in user)
     * ----------------------------------------------------------------
     *      		   API: DELETE /api/films/:filmId
     * ================================================================
    */
	removeFilm = async (userId, filmId) => {

		try {			

			/* retrieve film from the DB */
			const film = await this.filmService.getFilm(filmId);
			if (film.owner !== userId) {
				return constants.ERROR_401;
			}

			
			await this.filmService.removeFilm(filmId);
			return constants.MESSAGE_204;

		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	}

	
}

module.exports = FilmsCollectionController;

// module.exports.addFilm = function addFilm (req, res, next, body) {
//   FilmsCollection.addFilm(body)
//     .then(function (response) {
//       utils.writeJson(res, response);
//     })
//     .catch(function (response) {
//       utils.writeJson(res, response);
//     });
// };

// module.exports.getCreatedFilms = function getCreatedFilms (req, res, next, pageNo) {
//   FilmsCollection.getCreatedFilms(pageNo)
//     .then(function (response) {
//       utils.writeJson(res, response);
//     })
//     .catch(function (response) {
//       utils.writeJson(res, response);
//     });
// };

// module.exports.getInvitedFilms = function getInvitedFilms (req, res, next, pageNo) {
//   FilmsCollection.getInvitedFilms(pageNo)
//     .then(function (response) {
//       utils.writeJson(res, response);
//     })
//     .catch(function (response) {
//       utils.writeJson(res, response);
//     });
// };






// module.exports.editFilm = function editFilm (req, res, next, body, filmId) {
//   Film.editFilm(body, filmId)
//     .then(function (response) {
//       utils.writeJson(res, response);
//     })
//     .catch(function (response) {
//       utils.writeJson(res, response);
//     });
// };

// module.exports.getFilm = function getFilm (req, res, next, filmId) {
//   Film.getFilm(filmId)
//     .then(function (response) {
//       utils.writeJson(res, response);
//     })
//     .catch(function (response) {
//       utils.writeJson(res, response);
//     });
// };

// module.exports.getReview = function getReview (req, res, next, filmId, userId) {
//   Film.getReview(filmId, userId)
//     .then(function (response) {
//       utils.writeJson(res, response);
//     })
//     .catch(function (response) {
//       utils.writeJson(res, response);
//     });
// };


// module.exports.removeFilm = function removeFilm (req, res, next, filmId) {
//   Film.removeFilm(filmId)
//     .then(function (response) {
//       utils.writeJson(res, response);
//     })
//     .catch(function (response) {
//       utils.writeJson(res, response);
//     });
// };
