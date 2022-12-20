/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file ImagesRouter.js
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * ImagesRouter.js will dispatch all the requests to the specific controller, 
 * performing, moreover, their validation.
*/
'use strict';


/* --- MODULE IMPORT --- */
const express                   = require('express');
const ImagesController          = require('../controllers/ImagesController');
const DAO                       = require('../database/dao');
const storage                   = require("../components/Storage");
const { param }                 = require('express-validator');
const { query }                 = require('express-validator');
const { body }                  = require('express-validator');
const { validationHandler }     = require("../validators/RequestValidator");
const { isLoggedIn }            = require("../validators/LoginValidator");
const { schemaHandler }         = require('../validators/SchemaValidator');
const fs                        = require('fs');
const constants = require('../utils/constants');

/* --- ROUTER INITIALIZATION --- */
const router = express.Router();

/* --- CONTROLLER INITIALIZATION --- */
const dao = new DAO();
const imagesController = new ImagesController(dao);

/* --- API DEFINITIONS --- */

/* GET /api/films/public/:filmId/images/:imageId */
router.get(
    '/:filmId/images/:imageId',
    [
        query().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // query should be empty 
        }),
        body().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // body should be empty 
        }),
        param('filmId').exists().isNumeric(),
        param('imageId').exists().isNumeric()
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await imagesController.getImage(request.params.filmId, request.params.imageId, request.session.passport.user.id);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);


/* GET /api/films/public/:filmId/images/:imageId/imageFile */
router.get(
    '/:filmId/images/:imageId/imageFile',
    [
        query().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // query should be empty 
        }),
        body().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // body should be empty 
        }),
        param('filmId').exists().isNumeric(),
        param('imageId').exists().isNumeric()
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await imagesController.getImageFile(request.params.filmId, request.params.imageId, request.session.passport.user.id, request.get('Accept'));
            if (result.code === 200) {
                // console.log(result);
                // return response.status(200).json("test");
                return response.sendFile(result.message, {root: constants.IMAGES_PATH});
            } else {
                return response.status(result.code).json(result.message);
            }
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);


/* POST /api/films/public/:filmId/images */
router.post(
    '/:filmId/images',
    [
        query().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // query should be empty 
        }),
        param('filmId').exists().isNumeric(),
    ],
    validationHandler,
    isLoggedIn,
    storage.uploadImage,
    async (request, response) => {
        try {
            const result = await imagesController.addImage(request.params.filmId, request.session.passport.user.id, request.file);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);


/* DELETE /api/films/public/:filmId/images/:imageId */
router.delete(
    '/:filmId/images/:imageId',
    [
        query().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // query should be empty 
        }),
        body().custom(value => {
            return (Object.keys(value).length !== 0) ? false : true;    // body should be empty 
        }),
        param('filmId').exists().isNumeric(),
        param('imageId').exists().isNumeric(),
    ],
    validationHandler,
    isLoggedIn,
    async (request, response) => {
        try {
            const result = await imagesController.removeImage(request.params.filmId, request.params.imageId, request.session.passport.user.id);
            return response.status(result.code).json(result.message);
        } catch (error) {
            return response.status(error.code).json(error.message);
        }
    }
);

module.exports = router;