/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file FilmRouter.js
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * FilmRouter.js will dispatch all the requests to the specific controller, 
 * performing, moreover, their validation.
*/
'use strict';

/* --- MODULE IMPORT --- */
const express                   = require('express');
const FilmController            = require('../controllers/FilmController');
const DAO                       = require('../database/dao');
const { param }                 = require('express-validator');
const { query }                 = require('express-validator');
const { body }                  = require('express-validator');
const { validationHandler }     = require("../validators/RequestValidator");
const { isLoggedIn }            = require("../validators/LoginValidator");
const { schemaHandler }         = require('../validators/SchemaValidator');
const fs                        = require('fs');
const filmSchema                = JSON.parse(fs.readFileSync('validators/Schemas/FilmSchema.json').toString());

/* --- ROUTER INITIALIZATION --- */
const router = express.Router();

/* --- CONTROLLER INITIALIZATION --- */
const dao = new DAO();
const filmController = new FilmController(dao);


/* --- API DEFINITIONS --- */

/* GET /api/films/:filmId */
router.get(
    '/:filmId',
    [
        query().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // query should be empty 
        }),
        body().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // body should be empty 
        }),
        param('filmId').exists().isNumeric()
    ],
    validationHandler,
    async (request, response) => {
        try {
            const result = await filmController.getFilm(request.params.filmId, (request.session.passport) ? request.session.passport.user.id : undefined);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);

/* PUT /api/films/:filmId */
router.put(
    '/:filmId',
    [
        query().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // query should be empty 
        }),
        param('filmId').exists().isNumeric()
    ],
    (request, response, next) => {
        if (request.body.rating) request.body.private = 1;
        request.body.favorite = (request.body.favorite) ? 1 : 0;
        request.schema = filmSchema;
        next();
    },
    schemaHandler,
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            console.log("HERE");
            const result = await filmController.editFilm(request.session.passport.user.id, request.params.filmId, request.body);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);

/* DELETE /api/films/:filmId */
router.delete(
    '/:filmId',
    [
        query().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // query should be empty 
        }),
        body().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // body should be empty 
        }),
        param('filmId').exists().isNumeric()
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await filmController.removeFilm(request.session.passport.user.id, request.params.filmId);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);



module.exports = router;