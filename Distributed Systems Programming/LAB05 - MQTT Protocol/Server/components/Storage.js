/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file Storage.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * Storage.js manages the upload of an Image to the storage disk. 
*/
'use strict';

/* --- MODULE IMPORT --- */
const multer    = require('multer');
const constant  = require('../utils/constants');

/* creating storage engine */
const storageEngine = multer.diskStorage({
    
    /* where to store the file */
    destination: (request, file, callback) => {
        callback(null, constant.IMAGES_PATH);
    },

    /* name of the file to store */
    filename: (request, file, callback) => {
        callback(null, file.originalname);
    }
});

const uploadImage = multer({storage: storageEngine}).single('image');

module.exports.uploadImage = uploadImage;