/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file FilmService.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * FilmService.js collects all the queries that are useful to the
 * controller in order to implement the logic. It will contact the DAO class,
 * passing the completed query and returning the result to the controller.
 * ----------------------------------------------------------------------------
*/
'use strict';

class FilmService {


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
	 * Retrieve a single film from the DB
	 * --------------------------------------------
	 * @param filmId film identifier
	 */
	getFilm = async (filmId) => {
		const querySQL = "SELECT * FROM films WHERE id = ?";
		try {
            const result = await this.dao.get(
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
	 * Update a film in the DB
	 * -----------------------------
	 * @param filmId film identifier
	 * @param film film object
	 */
	editFilm = async (filmId, film) => {
		const querySQL = "UPDATE films SET title = ?, owner = ?, private = ?, watchDate = ?, rating = ?, favorite = ? WHERE id = ?";
		try {
			const result = await this.dao.run(
				querySQL,
				[
					film.title,
					film.owner,
					film.private,
					film.watchDate,
					film.rating,
					film.favorite,
					filmId
				]
			);
			return result;
		} catch (error) {
			throw error;
		}
	}

	/**
	 * Remove a film from the DB
	 * -----------------------------
	 * @param filmId film identifier
	 */
	removeFilm = async (filmId) => {
		const querySQL = "DELETE FROM films WHERE films.id = ?";
		try {
			const result = await this.dao.run(
				querySQL,
				[
					filmId
				]
			);
			return result;
		} catch (error) {
			throw error;
		}
	}
}

module.exports = FilmService;

