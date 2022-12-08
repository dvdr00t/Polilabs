/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file FilmsCollectionRouter.js
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * FilmsCollectionRouter.js will dispatch all the requests to the specific 
 * controller, performing moreover, their validation.
*/
'use strict';

/* --- MODULE IMPORT --- */
const express                   = require('express');
const FilmsCollectionController = require('../controllers/FilmsCollectionController');
const DAO                       = require('../database/dao');

/* --- ROUTER INITIALIZATION --- */
const router = express.Router();

/* --- CONTROLLER INITIALIZATION --- */
const dao = new DAO();
const filmsCollectionController = new FilmsCollectionController(dao);

/* --- VALIDATORS --- */
const fs                     = require('fs');
const filmSchema             = JSON.parse(fs.readFileSync('validators/Schemas/FilmSchema.json').toString());
const { param, query, body } = require('express-validator');
const { validationHandler }  = require('../validators/RequestValidator');
const { isLoggedIn }         = require('../validators/LoginValidator');
const { schemaHandler }      = require('../validators/SchemaValidator');


/* --- API DEFINITIONS --- */

/* GET /api/films/public */
router.get(
    '/public',
    [
        query('pageNo').optional().isNumeric(),
        body().custom(value => {  /* body should be empty */
            return (Object.keys(value).length !== 0) ? false : true;
        })
    ],
    validationHandler,
    async (request, response) => {
        try {
            const result = await filmsCollectionController.getPublicFilms(request.query.pageNo);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);

/* GET /api/films/created */
router.get(
    '/created',
    [
        query('pageNo').optional().isNumeric(),
        body().custom(value => {  /* body should be empty */
            return (Object.keys(value).length !== 0) ? false : true;
        })
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await filmsCollectionController.getCreatedFilms(request.session.passport.user.id, request.query.pageNo);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);


/* GET /api/films/invited */
router.get(
    '/invited',
    [
        query('pageNo').optional().isNumeric(),
        body().custom(value => {  /* body should be empty */
            return (Object.keys(value).length !== 0) ? false : true;
        })
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await filmsCollectionController.getInvitedFilms(request.session.passport.user.id, request.query.pageNo);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);


/* POST /api/films */
router.post(
    '',
    [
        query().custom(value => {  /* query should be empty */
            return (Object.keys(value).length !== 0) ? false : true;
        })
    ],
    (request, response, next) => {
        request.schema = filmSchema
        next();
    },
    schemaHandler,
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await filmsCollectionController.addFilm(request.session.passport.user.id, request.body);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);

module.exports = router;