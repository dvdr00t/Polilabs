/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file FilmsCollectionController.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * FilmsCollectionController.js implements all the logic related to the film
 * collection. It exploits the filmsCollectionService to query the DB. The
 * filmsCollectionService acts as an interface to the DB and it has a set of
 * defined queries.
*/
'use strict';

/* --- MODULE IMPORT --- */
const utils 					= require('../utils/writer.js');
const constants 				= require('../utils/constants.js');
const FilmsCollectionService 	= require('../service/FilmsCollectionService');
const ReviewsService 			= require('../service/ReviewsService');
const FilmService 				= require('../service/FilmService');
const MQTT 						= require('../components/MQTT');
const MQTTMessage 				= require('../components/MQTTMessage');

class FilmsCollectionController {

	/**
     * Constructor of the class FilmsCollectionController
     * --------------------------------------------------
     * @param {Object} dao 
     */
 	constructor (dao) {
		this.filmsCollectionService = new FilmsCollectionService(dao);
		this.reviewsService = new ReviewsService(dao);
		this.filmService = new FilmService(dao);
	}

	/*
	 *	+ -------------------- +
     *  |      FUNCTIONS       |
     *  + -------------------- +
	*/

	/**
     * Retrieve the list of public films from the DB.
	 * It returns a subset of the whole list, based on
	 * the pagination mechanism.
     * ------------------------------------------------
     *          API: GET /api/films/public
     * ================================================
    */
	getPublicFilms = async (pageNo) => {

		try {
	
			/* retrieving total number of films from the DB in order to compute pagination */
			let numberOfPublicFilms = await this.filmsCollectionService.getNumberOfPublicFilms();

			/* computing pageNo and total page */
			var pageNo = (pageNo !== undefined) ? parseInt(pageNo) : 1;
			var pageTot = Math.ceil(numberOfPublicFilms / constants.PAGE_OFFSET);
			var next = Number(pageNo) + 1;
	
			/* retrieving films in the selected pagination schema */
			let films = await this.filmsCollectionService.getPublicFilms();
	
			/* crafting return values */
			if (pageNo > pageTot) {
				return constants.ERROR_404;
			} else if (pageNo === pageTot) {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: numberOfPublicFilms,
					films: films.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
				})
			} else {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: numberOfPublicFilms,
					films: films.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
					next: '/api/films/public?pageNo=' + next
				})
			}
	
		} catch (error) {
	
			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	
	};

	/**
     * Retrieve the list of created films by the given
	 * user from the DB. It returns a subset of the 
	 * whole list, based on the pagination mechanism.
     * ------------------------------------------------
     *          API: GET /api/films/created
     * ================================================
    */
	getCreatedFilms = async (userId, pageNo) => {
		try {	

			/* retrieving total number of films from the DB in order to compute pagination */
			let numberOfCreatedFilms = await this.filmsCollectionService.getNumberOfCreatedFilms(userId);

			/* computing pageNo and total page */
			var pageNo = (pageNo !== undefined) ? parseInt(pageNo) : 1;
			var pageTot = Math.ceil(numberOfCreatedFilms / constants.PAGE_OFFSET);
			var next = Number(pageNo) + 1;

			/* retrieving films */
			var films = await this.filmsCollectionService.getCreatedFilms(userId);

			/* crafting return values */
			if (pageNo > pageTot) {
				return constants.ERROR_404;
			} else if (pageNo === pageTot) {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: numberOfCreatedFilms,
					films: films.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
				})
			} else {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: numberOfCreatedFilms,
					films: films.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
					next: '/api/films/public?pageNo=' + next
				})
			}

		} catch (error) {
			
			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	}

	/**
     * Retrieve the list of invited films by the given
	 * user from the DB. It returns a subset of the 
	 * whole list, based on the pagination mechanism.
     * ------------------------------------------------
     *          API: GET /api/films/invited
     * ================================================
    */
	getInvitedFilms = async (userId, pageNo) => {
		try {	

			/* retrieving total number of films from the DB in order to compute pagination */
			let numberOfInvitedFilms = await this.filmsCollectionService.getNumberOfInvitedFilms(userId);

			/* computing pageNo and total page */
			var pageNo = (pageNo !== undefined) ? parseInt(pageNo) : 1;
			var pageTot = Math.ceil(numberOfInvitedFilms / constants.PAGE_OFFSET);
			var next = Number(pageNo) + 1;

			/* retrieving invitations */
			const invitations = await this.reviewsService.getInvitations(userId);

			/* retrieving all films */
			const films = [];
			for (let review of invitations) {
				let film = await this.filmService.getFilm(review.filmId);
				if (film !== undefined) {
					films.push(film);
				}
			}

			/* crafting return values */
			if (pageNo > pageTot) {
				return constants.ERROR_404;
			} else if (pageNo === pageTot) {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: numberOfInvitedFilms,
					films: films.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
				})
			} else {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: numberOfInvitedFilms,
					films: films.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
					next: '/api/films/public?pageNo=' + next
				})
			}

		} catch (error) {
			
			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	}

	/**
     * An authenticated user can add a new film to the
	 * list. The film is validated against the JSON 
	 * schema in the Router. 
     * ------------------------------------------------
     *          API: POST /api/films
     * ================================================
    */
	addFilm = async (userId, body) => {
		try {

			/* building the film */
			const film = {
				title: body.title,
				owner: userId,
				private: body.private,
				watchDate: body.watchDate ? body.watchDate : null,
				rating: body.rating ? body.rating : null,
				favorite: body.favorite ? body.favorite : null
			}

			/* adding film to the database */
			const filmId = await this.filmsCollectionService.addFilm(film);

			/* publish MQTT message to the broker */
			const message = new MQTTMessage('inactive', undefined, undefined);
			MQTT.saveMessage(filmId, message);
			MQTT.publishFilmMessage(filmId, message);

			return constants.MESSAGE_201;

		} catch (error) {
			
			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	}
}

module.exports = FilmsCollectionController;