/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file MQTT.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * MQTT.js manages the communication from the Film Service application to the
 * Mosquitto MQTT Broker.
*/
'use strict';

/* --- MODULE IMPORT --- */
const MQTT = require('mqtt');
const MQTTMessage = require('./MQTTMessage');
const constants = require('../utils/constants');
const DAO = require('../database/dao');
const ReviewsService = require('../service/ReviewsService');

/* --- CONNECTING TO THE MQTT BROKER --- */
const clientId = 'mqttjs_' + Math.random().toString(16).substr(2, 8);
const HOST = `ws://${constants.MQTTBROKER_URL}`;
const OPTIONS = {
    keepalive: 30,  // NB: measured in seconds, not milliseconds!
    clientId: clientId,
    clean: true,    // client wants to start fresh with a new session, rather than resume an existing session.
    reconnectPeriod: 60 * 1000,
    connectTimeout: 30 * 1000,
    will: {
        topic: 'will_message',
        payload: 'connection closed abnormally',
        qos: 0,
        retain: false
    },
    rejectUnauthorized: false
}
const connection = MQTT.connect(HOST, OPTIONS);

/* managing operations */
const dao = new DAO();
const reviewsService = new ReviewsService(dao);
const filmMessageMap = new Map();
connection.on('error', (error) => {
    console.log(error);
    connection.end();
});
connection.on('connect', async () => {
    console.log(`client ${clientId} connected.`);

    /* getting film selections */
    try {
        const selections = await reviewsService.getSelectedFilm();
        selections.forEach(selection => {
            const status = (selection.userId) ? "active" : "inactive";
            const message = new MQTTMessage(status, selection.userId, selection.userName);
            filmMessageMap.set(selection.filmId, message);
            connection.publish(String(selection.filmId), JSON.stringify(message), { qos: 0, retain: true });
        });
    } catch (error) {
        console.log(error);
        connection.end();
    }
});
connection.on('close', () => {
    console.log(`client ${clientId} disconnected.`);
});


/* --- MODULE EXPORT --- */
module.exports.publishFilmMessage = function publishFilmMessage(filmId, message) {
    connection.publish(String(filmId), JSON.stringify(message), { qos: 0, retain: true })
};

module.exports.saveMessage = function saveMessage(filmId, message) {
    filmMessageMap.set(filmId, message);
};

module.exports.getMessage = function getMessage(filmId) {
    filmMessageMap.get(filmId);
};

module.exports.deleteMessage = function deleteMessage(filmId) {
    filmMessageMap.delete(filmId);
};