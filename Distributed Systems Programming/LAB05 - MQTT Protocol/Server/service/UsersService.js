/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file UsersService.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * UsersService.js collects all the queries that are useful to the
 * controller in order to implement the logic. It will contact the DAO class,
 * passing the completed query and returning the result to the controller.
 * ----------------------------------------------------------------------------
*/
'use strict';

class UsersService {


	/**
     * Constructor of the class UsersService
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
	 * Retrieve a specific user form the DB
	 * ------------------------------------
	 * @param {string} username the email of the user 
	 * @returns the user
	 */
	getUser = async (username) => {
		const querySQL = "SELECT * FROM users WHERE email = ?";
		try {
			const result = await this.dao.get(
				querySQL,
				[
					username
				]
			);
			return result;
		} catch (error) {
			throw error;
		}
	}

	/**
	 * Retrieve the list of users
	 * --------------------------
	 * @returns the list of users
	*/
	getUsers = async () => {
		const querySQL = "SELECT id, email, name FROM users";
		try {
			const result = await this.dao.all(querySQL);
			return result;
		} catch (error) {
			throw error;
		}
	}

	/**
	 * Retrieve the film currently active for the 
	 * specified user.
	 * -------------------------------------------------
	 * @param {number} userId 
	 * @returns the film active for the current user
	 */
	getActiveFilmUser = async (userId) => {
		const querySQL = "SELECT F.id, F.title FROM films AS F, reviews AS R WHERE R.reviewerId = ? AND R.filmId = F.id AND R.active = 1";
		try {
			const result = await this.dao.get(
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

}

module.exports = UsersService;
