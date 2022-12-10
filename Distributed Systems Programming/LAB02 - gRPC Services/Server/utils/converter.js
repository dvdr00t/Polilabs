/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file converter.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * converter.js manages the conversion of the image file from the original 
 * type to the specified type. In particular, it opens a gRPC channel with a
 * JavaServer instance and waits for the stream response.
*/
'use strict';

/* --- MODULE IMPORT --- */
const fs            = require('fs');
const grpc          = require('@grpc/grpc-js');
const protoLoader   = require('@grpc/proto-loader');
const constants     = require('./constants');

/* --- GRPC SETUP --- */
const packageDefinition = protoLoader.loadSync(
    constants.PROTO_PATH,
    {
        keepCase: true,
        longs: String,
        enums: String,
        defaults: true,
        oneofs: true
    }
);
const service = grpc.loadPackageDefinition(packageDefinition).conversion;

/**
 * gRPC module used to contact the JavaServer in order to convert an image.
 * ------------------------------------------------------------------------
 * @param {String} src_path original path to the file
 * @param {String} dst_path new path to the file
 * @param {String} src_type original extension of the file
 * @param {String} dst_type new extension of the file
 * @returns 
 */
exports.convertImage = (src_path, dst_path, src_type, dst_type) => {
    return new Promise((resolve, reject) => {
        
        /* creating the gRPC client (we do not care about security) */
        const client = new service.Converter(constants.REMOTE_URL, grpc.credentials.createInsecure());
        const channel = client.fileConvert();

        /* setting callback to receive the stream of data */
        const wstream = fs.createWriteStream(dst_path);
        channel.on('data', (data) => {

            /* receiving metadata */
            if (data.meta !== undefined) {
                if (!data.meta.success) {
                    reject(data.meta.error);
                }
            }

            /* receiving file chunk */
            if (data.file !== undefined) {
                wstream.write(data.file);
            }
        });

        
        /* setting callback to terminate the stream */
        channel.on('end', () => {
            wstream.end();
        });

        
        /* send metadata */
        channel.write({ "meta": {"file_type_origin": src_type, "file_type_target": dst_type}});

        /* send file */
        const imageDataStream = fs.createReadStream(src_path, {highWaterMark: constants.MAX_CHUNK_SIZE});
        imageDataStream.on('data', (chunk) => {
            channel.write({"file": chunk });
        });

        // When all the chunks of the image have been sent, the clients stops to use the gRPC channel from the sender side
        imageDataStream.on('end', () => {
            channel.end();
        });

        // Only after the write stream is closed,the promise is resolved (otherwise race conditions might happen)
        wstream.on('close', () => {
            resolve();
        })

    });
}