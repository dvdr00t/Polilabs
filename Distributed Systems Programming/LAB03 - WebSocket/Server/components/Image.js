/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file Image.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * Image.js contains the constructor prototype of a single image. It is used 
 * in order to create a Image JSON Object that will be serialized and returned 
 * to the user.
*/
'use strict';

class Image {

    /**
     * Constructor of an Image Object
     * @param {number} imageId 
     * @param {number} filmId 
     * @param {string} name 
     */
    constructor (imageId, filmId, name) {
        this.id = imageId;
        this.name = name;
        this.fileURI = `/api/films/public/${filmId}/images/${imageId}/imageFile`,
        this.self = `/api/films/public/${filmId}/images/${imageId}`
    }
}

module.exports = Image;