/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file FilmsCollectionService.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * FilmsCollectionService.js collects all the queries that are useful to the
 * controller in order to implement the logic. It will contact the DAO class,
 * passing the completed query and returning the result to the controller.
 * ----------------------------------------------------------------------------
*/
'use strict';

class FilmsCollectionService {


	/**
     * Constructor of the class FilmsCollectionService
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
	 * Computes the total number of films in the DB
	 * --------------------------------------------
	 * @returns a number
	 */
	getNumberOfPublicFilms = async () => {
		const querySQL = "SELECT COUNT(*) as totalItems FROM films WHERE private = 0";
		try {
			const result = await this.dao.get(querySQL);
			return result.totalItems;
		} catch (error) {
			throw error;
		}
	}

	/**
	 * Computes the total number of private films in the DB
	 * ----------------------------------------------------
	 * @returns a number
	 */
	 getNumberOfCreatedFilms = async (userId) => {
		const querySQL = "SELECT COUNT(*) as totalItems FROM films WHERE owner = ?";
		try {
			const result = await this.dao.get(
				querySQL,
				[
					userId
				]
			);
			return result.totalItems;
		} catch (error) {
			throw error;
		}
	}

	/**
	 * Computes the total number of invited films in the DB
	 * ----------------------------------------------------
	 * @returns a number
	 */
	getNumberOfInvitedFilms = async (userId) => {
		const querySQL = "SELECT COUNT(*) as totalItems FROM reviews WHERE reviewerId = ? AND completed = 0";
		try {
			const result = await this.dao.get(
				querySQL,
				[
					userId
				]
			);
			return result.totalItems;
		} catch (error) {
			throw error;
		}
	}


	/**
	 * Retrieve the list of films in a given pagination schema
	 * --------------------------------------------------------
	 * @returns the list of films
	 */
	getPublicFilms = async () => {
		const querySQL = "SELECT * FROM films WHERE private = 0";
		try {
			const result = await this.dao.all(querySQL);
			return result;
		} catch (error) {
			throw error;
		}
	}

	/**
	 * Retrieve the list of films created by the user
	 * --------------------------------------------------------
	 * @params userId the identifier of the logged in user
	 * @returns the list of films
	 */
	getCreatedFilms = async (userId) => {
		const querySQL = "SELECT * FROM films WHERE owner = ?";
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
	}

	/**
	 * Insert a film in the DB
	 * -----------------------
	 * @param {object} film 
	*/
	addFilm = async (film) => {
		const querySQL = "INSERT INTO films(title, owner, private, watchDate, rating, favorite) VALUES (?, ?, ?, ?, ?, ?)";
		try {
			const result = await this.dao.all(
				querySQL,
				[
					film.title,
					film.owner,
					film.private,
					film.watchDate,
					film.rating,
					film.favorite
				]
			);
			return result;
		} catch (error) {
			throw error;
		}
	}

}

module.exports = FilmsCollectionService;

