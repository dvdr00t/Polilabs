/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file ReviewsController.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * ReviewsController.js implements all the logic related to the invitations and
 * reviews. It exploits the reviewsService to query the DB. The
 * filmsCollectionService acts as an interface to the DB and it has a set of
 * defined queries.
*/
'use strict';

/* --- MODULE IMPORT --- */
const utils 				= require('../utils/writer.js');
const constants 			= require('../utils/constants.js');
const ReviewsService 		= require('../service/ReviewsService');
const FilmService 			= require('../service/FilmService');

class ReviewsController {

	/**
     * Constructor of the class FilmsCollectionController
     * --------------------------------------------------
     * @param {Object} dao 
     */
 	constructor (dao) {
		this.reviewsService = new ReviewsService(dao);
		this.filmService = new FilmService(dao);
	}

	/*
	 *	+ -------------------- +
     *  |      FUNCTIONS       |
     *  + -------------------- +
	*/


	/**
     * Retrieves the list of reviews for the specific film retrieved from 
	 * the DB, given its filmId.
     * -------------------------------------------------------------------
     *      		API: GET /api/films/:filmId/reviews
     * ===================================================================
    */
	getReviews = async (filmId, pageNo) => {

		try {

			/* retrieving the total number of reviews for the specified film */
			let numberOfReviews = await this.reviewsService.getNumberOfReviews(filmId);

			/* computing pageNo and total page */
			var pageNo = (pageNo !== undefined) ? parseInt(pageNo) : 1;
			var pageTot = Math.ceil(numberOfReviews / constants.PAGE_OFFSET);
			var next = Number(pageNo) + 1;

			/* retrieving reviews in the selected pagination schema */
			let reviews = await this.reviewsService.getReviews(filmId)

			/* crafting return values */
			if (pageNo > pageTot) {
				return constants.ERROR_404;
			} else if (pageNo === pageTot) {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: numberOfReviews,
					reviews: reviews.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
				})
			} else {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: numberOfReviews,
					reviews: reviews.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
					next: '/api/films/:filmId/reviews?pageNo=' + next
				})
			}

			
		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;
		}

	};

	/**
     * A reviewer invited for a film can mark the review as completed, 
	 * also updating the review date, the rating and the textual description 
	 * of the review.
     * -------------------------------------------------------------------
     *      	   API: PUT /api/films/:filmId/invitations
     * ===================================================================
    */
	addReview = async (filmId, reviewerId, body) => {

		try {

			/* checking review state */
			const result = await this.reviewsService.getInvitation(filmId, reviewerId);
			if (result === undefined) {
				return constants.ERROR_400;
			}

			/* checking review constraint */
			if (result.reviewerId !== reviewerId) {
				return constants.ERROR_403;
			}

			/* building up film */
			const reviewObject = {
				filmId: filmId,
				reviewerId: reviewerId,
				completed: true,
				reviewDate: body.reviewDate,
				rating: body.rating,
				review: body.review
			};
			await this.reviewsService.addReview(reviewObject);
			return constants.MESSAGE_201;
			
		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;
		}

	}

	/**
     * Retrieves the single review of the user for the specific film 
	 * retrieved from the DB, given its filmId.
     * -------------------------------------------------------------------
     *      		API: GET /api/films/:filmId/reviews
     * ===================================================================
    */
	getReview = async (filmId, userId) => {

		try {

			/* retrieving reviews in the selected pagination schema */
			let review = await this.reviewsService.getReview(filmId, userId);
			if (review == undefined) {
				return constants.ERROR_404;
			}

			return utils.writeJson(200, review);
			
		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;
		}

	}

	/**
     * The owner of a public film can issue a review invitation to 
	 * a user (the reviewer may be the owner herself). Review 
	 * invitations for a film can be issued to multiple users at 
	 * the same time.
     * -----------------------------------------------------------
     *          API: POST /api/films/:filmId/invitations
     * ===========================================================
    */
	addInvitations = async (userId, filmId, body) => {
		try {

			/* checking film */
			const film = await this.filmService.getFilm(filmId);
			if (film === undefined) {
				return constants.ERROR_404;
			} else if (film.owner !== userId) {
				return constants.ERROR_403;
			} else if (film.private === 1) {
				return constants.ERROR_400;
			}

			/* issuing invitations */
			for (let userId of body.users) {
				const result = await this.reviewsService.addInvitation(filmId, userId);
				console.log(result);	
			}
			
			return constants.MESSAGE_201;

		} catch (error) {
			
			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	}

	/**
     * The owner of a film can remove a review invitation, if the 
	 * review has not yet been completed by the reviewer.
     * -----------------------------------------------------------
     *    API: DELETE /api/films/:filmId/invitations/:userId
     * ===========================================================
    */
	removeInvitations = async (ownerId, filmId, userId) => {
		try {

			/* checking film */
			const film = await this.filmService.getFilm(filmId);
			if (film === undefined) {
				return constants.ERROR_404;
			} else if (film.owner !== ownerId) {
				return constants.ERROR_403;
			} else if (film.private === 1) {
				return constants.ERROR_400;
			}

			/* checking review */
			const review = await this.reviewsService.getInvitation(filmId, userId);
			if (review && review.completed) {
				return constants.ERROR_400;
			}

			/* removing invitations */
			await this.reviewsService.removeInvitation(filmId, userId);
			return constants.MESSAGE_204;

		} catch (error) {
			
			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
	}
}

module.exports = ReviewsController;

