/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file UserRouter.js
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * UserRouter.js will dispatch all the requests to the specific controller, 
 * performing, moreover, their validation.
*/
'use strict';

/* --- MODULE IMPORT --- */
const express                   = require('express');
const passport                  = require('passport');
const UsersController           = require('../controllers/UsersController');
const ReviewsController         = require('../controllers/ReviewsController');
const DAO                       = require('../database/dao');
const { param }                 = require('express-validator');
const { query }                 = require('express-validator');
const { body }                  = require('express-validator');
const { validationHandler }     = require("../validators/RequestValidator");
const { isLoggedIn }            = require('../validators/LoginValidator');
const constants                 = require('../utils/constants');

/* --- ROUTER INITIALIZATION --- */
const router = express.Router();

/* --- CONTROLLER INITIALIZATION --- */
const dao = new DAO();
const usersController = new UsersController(dao);
const reviewsController = new ReviewsController(dao);



/* --- API DEFINITIONS --- */

/* POST /api/users/authenticator */
router.post(
    '/authenticator',
    passport.authenticate('local'),
    async (request, response) => {

        /**
         * LAB03 - WEBSOCKET
         * -----------------
         * When a new user logs in, we need to update the current state of logged in users
         * and notify all the other users that a new user has logged in.
        */
        try {
            await usersController.getActiveFilmUser(request.user.id, request.user.name);
            return (request.user) ? response.status(201).json(request.user) : response.status(401).json({message: constants.ERROR_401});   
        } catch (error) {
            return response.status(error.code).json(error.message); 
        }
    }
);

/* DELETE /api/users/authenticator */
router.delete(
    '/authenticator',
    isLoggedIn,
    async (request, response) => {
        await usersController.logoutUser(request.session.passport.user.id, request.session.passport.user.name);
        request.logOut(() => {
            response.status(200).json({});
        });
    }
);

/* GET /api/users */
router.get(
    '',
    [
        query('pageNo').optional().isNumeric(),
        body().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // body should be empty 
        }),
        param().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // param should be empty 
        })
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await usersController.getUsers();
            return response.json(result);
        } catch (error) {
            return response.status(error.code).json(error.message); 
        }
    }
);


/* PUT /api/users/:userId/selection */
router.put(
    '/:userId/selection',
    [
        param('userId').exists().isNumeric(),
        query().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // param should be empty 
        })
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await reviewsController.selectFilm(request.params.userId, request.session.passport.user.name, request.body);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message); 
        }
    }
);

module.exports = router;