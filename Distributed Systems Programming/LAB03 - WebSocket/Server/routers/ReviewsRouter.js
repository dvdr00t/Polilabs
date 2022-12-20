/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file ReviewsRouter.js
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * ReviewsRouter.js will dispatch all the requests to the specific 
 * controller, performing moreover, their validation.
*/
'use strict';

/* --- MODULE IMPORT --- */
const express                   = require('express');
const ReviewsController         = require('../controllers/ReviewsController');
const DAO                       = require('../database/dao');

/* --- ROUTER INITIALIZATION --- */
const router = express.Router();

/* --- CONTROLLER INITIALIZATION --- */
const dao = new DAO();
const reviewsController = new ReviewsController(dao);

/* --- VALIDATORS --- */
const fs                     = require('fs');
const { param, query, body } = require('express-validator');
const { validationHandler }  = require('../validators/RequestValidator');
const { isLoggedIn }         = require('../validators/LoginValidator');
const { schemaHandler }      = require('../validators/SchemaValidator');


/* --- API DEFINITIONS --- */


/* GET /api/films/:filmId/reviews */
router.get(
    '/:filmId/reviews',
    [
        query('pageNo').optional().isNumeric(),
        body().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // body should be empty 
        }),
        param('filmId').exists().isNumeric()
    ],
    validationHandler,
    async (request, response) => {
        try {
            const result = await reviewsController.getReviews(request.params.filmId, request.query.pageNo);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message); 
        }
    }
);

/* GET /api/films/:filmId/reviews/:userId */
router.get(
    '/:filmId/reviews/:userId',
    [
        query().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // query should be empty 
        }),
        body().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // body should be empty 
        }),
        param('filmId').exists().isNumeric(),
        param('userId').exists().isNumeric(),
    ],
    validationHandler,
    async (request, response) => {
        try {
            const result = await reviewsController.getReview(request.params.filmId, request.params.userId);
            return response.status(result.code).json(result.message);
        } catch (error) {
            console.log(error);
            return response.status(error.code).json(error.message); 
        }
    }
);

/* POST /api/films/:filmId/invitations */
router.post(
    '/:filmId/invitations',
    [
        param('filmId').exists().isNumeric(),
        query().custom(value => {  /* query should be empty */
            return (Object.keys(value).length !== 0) ? false : true;
        }),
        body('users').isArray()
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await reviewsController.addInvitations(request.session.passport.user.id, request.params.filmId, request.body);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);

/* put /api/films/:filmId/invitations */
router.put(
    '/:filmId/invitations',
    [
        param('filmId').exists().isNumeric(),
        query().custom(value => {  /* query should be empty */
            return (Object.keys(value).length !== 0) ? false : true;
        }),
        body('review').isObject()
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await reviewsController.addReview(request.params.filmId, request.session.passport.user.id, request.body.review);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);

/* DELETE /api/films/:filmId/invitations/:userId */
router.delete(
    '/:filmId/invitations/:userId',
    [
        param('filmId').exists().isNumeric(),
        param('userId').exists().isNumeric(),
        query().custom(value => {  /* query should be empty */
            return (Object.keys(value).length !== 0) ? false : true;
        }),
        body().custom(value => {  /* body should be empty */
            return (Object.keys(value).length !== 0) ? false : true;
        })
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await reviewsController.removeInvitations(request.session.passport.user.id, request.params.filmId, request.params.userId);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);


module.exports = router;