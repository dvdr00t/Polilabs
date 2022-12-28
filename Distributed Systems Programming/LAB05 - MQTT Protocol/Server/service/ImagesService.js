/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file ImagesService.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * ImagesService.js collects all the queries that are useful to the
 * controller in order to implement the logic. It will contact the DAO class,
 * passing the completed query and returning the result to the controller.
 * ----------------------------------------------------------------------------
*/
'use strict';

class ImagesService {


    /**
     * Constructor of the class ImagesService
     * @param {Object} dao
    */
	constructor (dao) {
        this.dao = dao;
    }


    /*
     *  + -------------------- +
	 *  |      FUNCTIONS       |
	 *  + -------------------- +
	*/

	/**
	 * Retrieve a single image from the DB
	 * --------------------------------------------
     * @param filmId film identifier
	 * @param imageId image identifier
	*/
    getImage = async (filmId, imageId) => {
        const querySQL = "SELECT name FROM images as I, filmImages as FI WHERE I.id = FI.imageId AND FI.filmId = ? AND I.id = ?";
        try {
            const result = await this.dao.get(
                querySQL,
                [
                    filmId,
                    imageId
                ]
            );
            return result;
        } catch (error) {
            throw error;
        }
    };

    /**
	 * Add a specific image to the DB
	 * -------------------------------
	 * @param imageName name of the image
	*/
    addImage = async (imageName) => {
        const querySQL = "INSERT INTO images (name) VALUES (?)";
        try {
            const result = await this.dao.run(
                querySQL,
                [
                    imageName
                ]
            );
            return result.id;
        } catch (error) {
            throw error;
        }
    }

    /**
	 * Add a specific image to the film
	 * ---------------------------------
	 * @param imageId image identifier
     * @param filmId film identifier
	*/
    pairImage = async (imageId, filmId) => {
        const querySQL = "INSERT INTO filmImages VALUES (?, ?)";
        try {
            const result = await this.dao.run(
                querySQL,
                [
                    filmId,
                    imageId
                ]
            );
            return result;
        } catch (error) {
            throw error;
        }
    }


    /**
	 * Delete a specific image from the DB
	 * -----------------------------------
	 * @param imageId image identifier
	*/
    deleteImage = async (imageId) => {
        const querySQL = "DELETE FROM images WHERE id = ?";
        try {
            await this.dao.run(
                querySQL,
                [
                    imageId
                ]
            );
        } catch (error) {
            throw error;
        }
    }

    /**
	 * Remove a specific image from the film
	 * -------------------------------------
	 * @param imageId image identifier
     * @param filmId film identifier
	*/
    unpairImage = async (imageId, filmId) => {
        const querySQL = "DELETE FROM filmImages WHERE filmId = ? AND imageId = ?";
        try {
            await this.dao.run(
                querySQL,
                [
                    filmId,
                    imageId
                ]
            );
        } catch (error) {
            throw error;
        }
    }
    
}

module.exports = ImagesService;