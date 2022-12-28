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
const FilmService 			= require('../service/FilmService');
const ReviewsService		= require('../service/ReviewsService');

/* --- WEBSOCKET IMPORTS --- */
const WebSocket				= require('../components/WebSocket');
const WSMessage 			= require('../components/WSMessage');
const MQTT					= require('../components/MQTT');
const MQTTMessage 			= require('../components/MQTTMessage');

class FilmsCollectionController {

	/**
     * Constructor of the class FilmsCollectionController
     * --------------------------------------------------
     * @param {Object} dao 
     */
 	constructor (dao) {
		this.filmService = new FilmService(dao);
		this.reviewsService = new ReviewsService(dao);
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
			if (body.private !== undefined && oldFilm.private != body.private) {
				return constants.ERROR_400;
			} else if (oldFilm.owner != userId) {
				return constants.ERROR_403;
			}

			/* building the film */
			const film = {
				title: body.title,
				owner: userId,
				private: true,
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
				return constants.ERROR_403;
			}

			/* private films can be directly deleted */
			if (film.private) {
				await this.filmService.removeFilm(filmId);
			} else {

				/* public films required to publish a message to the MQTT broker */

				/* searching active reviewer for this particular film */
				const allReviews = await this.reviewsService.getAllReviews(filmId);
				const activeReviewer = null;
				for (let review of allReviews) {
					if (review.active) {
						activeReviewer = review.reviewerId;
					}
				}

				/* removing the reviews for this particular film */
				await this.reviewsService.removeAllReviews(filmId);

				/* removing the film */
				await this.filmService.removeFilm(filmId);

				/* remove the corresponding MQTT message */
				const message = new MQTTMessage('deleted', undefined, undefined);
				MQTT.publishFilmMessage(filmId, message);
				MQTT.deleteMessage(filmId);
				
				/* in case the film was active for a user, informs the other clients */
				if (activeReviewer) {
					const oldMessage = WebSocket.getMessage(activeReviewer);
					const updatedMessage = new WSMessage('update', parseInt(activeReviewer), oldMessage.userName, null, null);
					WebSocket.sendAllClients(updatedMessage);
					WebSocket.saveMessage(activeReviewer, new WSMessage('login', parseInt(activeReviewer), oldMessage.userName, null, null));
				}
			}
			
			return constants.MESSAGE_204;

		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	}

	
}

module.exports = FilmsCollectionController;