'use strict';

/* --------- ERROR MESSAGES --------- */
const constants = require("../utils/constants");

/* --------- EXPRESS VALIDATOR --------- */
const { validationResult }  = require('express-validator');

/**
 * Middleware to validate the request body
 * ------------------------------------------------
 * @param {Object} request HTTP request 
 * @param {Object} response HTTP response
 * @param {Function} next move to the next validation
 */
function validationHandler(request, response, next) {

    const errors = validationResult(request);
    if (!errors.isEmpty()) {
        return response.status(constants.ERROR_400.code).json(constants.ERROR_400.message);
    }

    next();
}

module.exports = { validationHandler };