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
const DAO                       = require('../database/dao');
const { param }                 = require('express-validator');
const { query }                 = require('express-validator');
const { body }                  = require('express-validator');
const { validationHandler }     = require("../validators/RequestValidator");
const { isLoggedIn }            = require('../validators/LoginValidator');





/* --- ROUTER INITIALIZATION --- */
const router = express.Router();

/* --- CONTROLLER INITIALIZATION --- */
const dao = new DAO();
const usersController = new UsersController(dao);


/* --- API DEFINITIONS --- */

/* POST /api/users/authenticator */
router.post(
    '/authenticator',
    passport.authenticate('local'),
    async (request, response) => {
        response.status(201).json(request.user);
    }
);

/* DELETE /api/users/authenticator */
router.delete(
    '/authenticator',
    async (request, response) => {
        console.log(request.isAuthenticated());
        request.logOut(() => {
            response.end();
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
            const result = await usersController.getUsers(request.params.pageNo);
            return response.status(result.code).json(result.message);
        } catch (error) {
            console.log(error);
            return response.status(error.code).json(error.message); 
        }
    }
);

module.exports = router;