/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file ImagesController.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * ImagesController.js implements all the logic related to the images. 
 * It exploits the ImagesService to query the DB. The ImagesService acts as an 
 * interface to the DB and it has a set of defined queries.
*/
'use strict';

/* --- MODULE IMPORT --- */
const utils         = require('../utils/writer.js');
const constants     = require('../utils/constants.js');
const ImagesService = require('../service/ImagesService');
const FilmService   = require('../service/FilmService');
const ReviewService = require('../service/ReviewsService');
const Image         = require('../components/Image');
const fs            = require('fs');

/* --- gRPC MODULE --- */
const converter = require('../utils/converter');


class ImagesController {


    /**
     * Constructor of the class ImagesController
     * -----------------------------------------
     * @param {Object} dao 
     */
 	constructor (dao) {
		this.imagesService = new ImagesService(dao);
        this.filmService = new FilmService(dao);
        this.reviewsService = new ReviewService(dao);
	}


	/*
	 *	+ -------------------- +
     *  |      FUNCTIONS       |
     *  + -------------------- + 
	*/

	/**
     * Retrieves the specific image of a specific film from the 
     * DB, given its imageId.
     * ----------------------------------------------------------
     *    API: GET /api/films/public/:filmId/images/:imageId
     * ==========================================================
    */
    getImage = async (filmId, imageId, userId) => {
        
        try {

            /* retrieving film and checking film constraints */
            const film = await this.filmService.getFilm(filmId);
            const review = await this.reviewsService.getReview(filmId, userId);
            if (film === undefined) {
                return constants.ERROR_404;
            } else if (film.private === 1) {
                return constants.ERROR_403;
            } else if (film.owner !== userId && review === undefined) {
                return constants.ERROR_401;
            }


            /* retrieving the image name */
            const result = await this.imagesService.getImage(filmId, imageId);
            if (result === undefined) {
                return constants.ERROR_404;
            }

            const image = new Image(imageId, filmId, result.name);
            return utils.writeJson(200, image);
            
        } catch (error) {
            
            /* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
        }
    };


    /**
     * Get the specific image file from the server. 
     * ------------------------------------------------------------------
     *    API: GET /api/films/public/:filmId/images/:imageId/imageFile
     * ==================================================================
    */
    getImageFile = async (filmId, imageId, userId, accept) => {

        try {

            /* checking accepted mediatype */
            if (accept != 'image/png' && accept != 'image/jpg' && accept != 'image/gif') {
                return constants.ERROR_415;
            }
            const extension = accept.substring(accept.lastIndexOf("/")).replace('/', '');

            /* retrieving film and checking film constraints */
            const film = await this.filmService.getFilm(filmId);
            const review = await this.reviewsService.getReview(filmId, userId);
            if (film === undefined) {
                return constants.ERROR_404;
            } else if (film.private === 1) {
                return constants.ERROR_403;
            } else if (film.owner !== userId && review === undefined) {
                return constants.ERROR_401;
            }

            /* retrieving the image name */
            const result = await this.imagesService.getImage(filmId, imageId);
            if (result === undefined) {
                return constants.ERROR_404;
            }

            /* retrieving filename */
            const filename = `${constants.IMAGES_PATH}/${result.name}.${extension}`;
            
            /* sending back file */
            if (fs.existsSync(filename)) {
                return utils.writeJson(200, `${result.name}.${extension}`);
            } else {

                /* gRPC --> Time to convert the file */

                /* searching existing file */
                let src_path = "";
                for (let ext of constants.VALID_EXTENSIONS) {
                    src_path = `${constants.IMAGES_PATH}/${result.name}.${ext}`;
                    if (fs.existsSync(src_path)) {
                        await converter.convertImage(src_path, filename, extension, ext);
                        return utils.writeJson(200, `${result.name}.${ext}`);
                    }
                }
                
                return constants.ERROR_404;
            }
            
        } catch (error) {
            
            /* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
        }
    }


    /**
     * Add the given image to the list of images in the DB, pairing
     * it with the specified film. Film must be public and user
     * must own the specific film.
     * ------------------------------------------------------------
     *        API: POST /api/films/public/:filmId/images/
     * ============================================================
    */
    addImage = async (filmId, ownerId, file) => {

        try {

            /* retrieving film and checking film constraints */
            const film = await this.filmService.getFilm(filmId);
            if (film === undefined) {
                return constants.ERROR_404;
            } else if (film.private === 1) {
                return constants.ERROR_403;
            } else if (film.owner !== ownerId) {
                return constants.ERROR_401;
            }

            /* parsing data and checking */
            const name = file.originalname;
            const extension = name.substring(name.lastIndexOf(".")).replace('.', '');
            if (!(constants.VALID_EXTENSIONS.includes(extension))) {
                return constants.ERROR_415;
            }

            /* inserting image into the DB */
            const imageId = await this.imagesService.addImage(name.substring(0, name.lastIndexOf(".")));
            await this.imagesService.pairImage(imageId, filmId);

            /* creating new image Object and return */
            const image = new Image(imageId, filmId, name);
            return utils.writeJson(201, image);
            
        } catch (error) {
            
            /* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
        }
    }


    /**
     * Remove a specific image from the DB. Film must be public and 
     * user must own the specific film.
     * ------------------------------------------------------------
     *    API: DELETE /api/films/public/:filmId/images/:imageId
     * ============================================================
    */
    removeImage = async (filmId, imageId, ownerId) => {

        try {

            /* retrieving film and checking film constraints */
            const film = await this.filmService.getFilm(filmId);
            if (film === undefined) {
                return constants.ERROR_404;
            } else if (film.private === 1) {
                return constants.ERROR_403;
            } else if (film.owner !== ownerId) {
                return constants.ERROR_401;
            }

            /* retrieving name of the image */
            const result = await this.imagesService.getImage(filmId, imageId);
            if (result === undefined) {
                return constants.MESSAGE_204;
            }

            /* removing image from DB */
            await this.imagesService.deleteImage(imageId);
            await this.imagesService.unpairImage(imageId, filmId);

            /* removing image from server */
            for (let extension of constants.VALID_EXTENSIONS) {
                if (fs.existsSync(`${constants.IMAGES_PATH}/${result.name}.${extension}`)) {
                    fs.unlinkSync(`${constants.IMAGES_PATH}/${result.name}.${extension}`);
                }
            }

            return constants.MESSAGE_204;
            
        } catch (error) {
            
            /* some errors occurred */
			console.log(error);
			throw constants.ERROR_500;	
        }
    }

}

module.exports = ImagesController;
