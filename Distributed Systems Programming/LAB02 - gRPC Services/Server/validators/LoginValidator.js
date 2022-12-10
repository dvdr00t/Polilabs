/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file LoginValidator.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * LoginValidator is a middleware what checks whether the requested comes from 
 * a logged in user or not.
*/
'use strict';

/* --- MODULE IMPORT --- */
const constants = require('../utils/constants');

/**
 * Middleware to authenticate the request body
 * ------------------------------------------------
 * @param {Object} request HTTP request 
 * @param {Object} response HTTP response
 * @param {Function} next move to the next validation
 */
function isLoggedIn(request, response, next) {

    if (request.isAuthenticated()) {
        next();
    } else {
        return response.status(constants.ERROR_401.code).json(constants.ERROR_401.message);
    }
}

module.exports = { isLoggedIn }