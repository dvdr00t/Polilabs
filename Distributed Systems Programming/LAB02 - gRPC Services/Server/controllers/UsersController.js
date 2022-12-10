/**
 *				          REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file UsersController.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * UsersController.js implements all the logic related to the single user. 
 * It exploits the UsersService to query the DB. The UsersService acts as an 
 * interface to the DB and it has a set of defined queries.
*/
'use strict';

/* --- MODULE IMPORT --- */
const utils 			 	= require('../utils/writer.js');
const bcrypt        		= require('bcrypt');
const constants 			= require('../utils/constants.js');
var UsersService 			= require('../service/UsersService');


class UsersController {

	/**
     * Constructor of the class UsersController
     * ----------------------------------------
     * @param {Object} dao 
     */
 	constructor (dao) {
		this.usersService = new UsersService(dao);
	}

	/*
	 *	+ -------------------- +
     *  |      FUNCTIONS       |
     *  + -------------------- + 
	*/

	/**
     * Retrieves the specific user from the DB, given 
	 * its username and password.
     * ------------------------------------------------
    */
   	getUser = async (username, password) => {
		try {

			/* retrieving user from DB */
			const result = await this.usersService.getUser(username);
			if (result === undefined) {
                return false;
            }

			/* user exists, let's check the password */
			return new Promise((resolve, reject) => {
				const user = {
					id: 		result.id,
					username:	result.email,
					name: 		result.name
				}
				resolve((bcrypt.compareSync(password, result.hash)) ? user : false);
			});

		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
   	};

	/**
     * Retrieve the list of users from the DB with the
	 * given pagination mechanism.
     * ------------------------------------------------
     *             API: GET /api/users
     * ================================================
    */
	getUsers = async (pageNo) => {
		try {

			/* retrieving user from DB */
			const result = await this.usersService.getUsers();
			if (result === undefined) {
                return constants.ERROR_404;
            }

			/* computing pageNo and total page */
			var pageNo = (pageNo !== undefined) ? parseInt(pageNo) : 1;
			var pageTot = Math.ceil(result.length / constants.PAGE_OFFSET);
			var next = Number(pageNo) + 1;
	
			/* crafting return values */
			if (pageNo > pageTot) {
				return constants.ERROR_404;
			} else if (pageNo === pageTot) {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: result.length,
					users: result.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
				})
			} else {
				return utils.writeJson(200, {
					totalPages: pageTot,
					currentPage: pageNo,
					totalItems: result.length,
					users: result.slice((pageNo-1) * constants.PAGE_OFFSET, pageNo * constants.PAGE_OFFSET),
					next: '/api/films/public?pageNo=' + next
				})
			}

		} catch (error) {

			/* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
		}
   	};

}

module.exports = UsersController;

