/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file InvitationsService.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * InvitationsService.js collects all the queries that are useful to the
 * controller in order to implement the logic. It will contact the DAO class,
 * passing the completed query and returning the result to the controller.
 * ----------------------------------------------------------------------------
*/
'use strict';

class InvitationsService {


	/**
     * Constructor of the class FilmService
     * @param {Object} dao
    */
	constructor (dao) {
        this.dao = dao;
    }

	/*
     *  + -------------------- +
	 *  |      FUNCTIONS       |
	 *  + -------------------- +
	*/

	/**
	 * Get the list of all invitations for a single
	 * user from the DB, given the userId.
	 * ---------------------------------------------
	 * @param userId user identifier
	 */
	getInvitations = async (userId) => {
		const querySQL = "SELECT * FROM reviews WHERE reviewerId = ? AND completed = 0";
		try {
            const result = await this.dao.all(
                querySQL,
                [
					userId
                ]
            );
            return result;
        } catch (error) {
            throw error;
        }  
	};

	/**
	 * Add an invitation to the DB, given the filmId
	 * and the userId
	 * ---------------------------------------------
	 * @param filmId film identifier
	 * @param userId user identifier
	 */
	addInvitation = async (filmId, userId) => {
		const querySQL = "INSERT INTO reviews (filmId, reviewerId) VALUES (?, ?)";
		try {
            const result = await this.dao.run(
                querySQL,
                [
                    filmId,
					userId
                ]
            );
            return result;
        } catch (error) {
            throw error;
        }  
	};

	/**
	 * Add a review to the DB
	 * ----------------------
	 * @param review object
	 */
	addReview = async (review) => {
		const querySQL = "UPDATE reviews SET completed = ?, reviewDate = ?, rating = ?, review = ? where filmId = ? AND reviewerId = ?";
		try {
            const result = await this.dao.run(
                querySQL,
                [
					review.completed,
					review.reviewDate,
					review.rating,
					review.review,
					review.filmId,
					review.reviewerId
                ]
            );
            return result;
        } catch (error) {
            throw error;
        }  
	};

	/**
	 * Remove an invitation from the DB, given the filmId
	 * and the userId
	 * --------------------------------------------------
	 * @param filmId film identifier
	 * @param userId user identifier
	 */
	removeInvitation = async (filmId, userId) => {
		const querySQL = "DELETE FROM reviews WHERE filmId = ? AND reviewerId = ?";
		try {
            const result = await this.dao.run(
                querySQL,
                [
                    filmId,
					userId
                ]
            );
            return result;
        } catch (error) {
            throw error;
        }  
	};


	/**
	 * Computes the number of reviews of the given 
	 * filmId
	 * --------------------------------------------
	 * @returns a number
	 */
	 getNumberOfReviews = async (filmId) => {
		const querySQL = "SELECT COUNT(*) as totalNumberOfReviews FROM reviews WHERE filmId = ? AND completed = 1";
		try {
			const result = await this.dao.get(
				querySQL,
				[
					filmId
				]
			);
			return result.totalNumberOfReviews;
		} catch (error) {
			throw error;
		}
	};

	/**
	 * Retrieve the list of reviews of the given 
	 * filmId
	 * --------------------------------------------
	 * @returns a list of reviews
	 */
	getReviews = async (filmId) => {
		const querySQL = "SELECT * FROM reviews WHERE filmId = ? AND completed = 1";
		try {
			const result = await this.dao.all(
				querySQL,
				[
					filmId
				]
			);
			return result;
		} catch (error) {
			throw error;
		}
	};

	/**
	 * Retrieve the specific review from the user 
	 * of the given filmId
	 * --------------------------------------------
	 * @returns a list of reviews
	 */
	getReview = async (filmId, userId) => {
		const querySQL = "SELECT * FROM reviews WHERE (filmId = ? AND reviewerId = ?) AND completed = 1";
		try {
			const result = await this.dao.get(
				querySQL,
				[
					filmId,
					userId
				]
			);
			return result;
		} catch (error) {
			throw error;
		}
	};

	/**
	 * Retrieve the specific invitation from the user 
	 * of the given filmId
	 */
	getInvitation = async (filmId, userId) => {
		const querySQL = "SELECT * FROM reviews WHERE (filmId = ? AND reviewerId = ?) AND completed = 0";
		try {
			const result = await this.dao.get(
				querySQL,
				[
					filmId,
					userId
				]
			);
			return result;
		} catch (error) {
			throw error;
		}
	};

}

module.exports = InvitationsService;
