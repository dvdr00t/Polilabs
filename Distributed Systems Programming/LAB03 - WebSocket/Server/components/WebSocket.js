/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file WebSocker.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * WebSocket.js manages the communication through a Web Socket between the 
 * server and a specific client.
*/
'use strict';

/* --- MODULE IMPORT --- */
const constants = require('../utils/constants');
const WebSocket = require('ws');

/* opening server connection */
var wss = new WebSocket.Server({port: constants.WEBSOCKET_PORT});
console.log(`[+] WebSocket server listening on localhost:${constants.WEBSOCKET_PORT}`);

/* configuring web socket */
var loginMessagesMap = new Map();
wss.on('connection', ws => {

    /* on a new connection, inform the newly connected client about all the users who are currently logged in the service */
    loginMessagesMap.forEach(function each(message) {
        ws.send(JSON.stringify(message));
    });
});



/* --- MODULE EXPORT --- */
module.exports.sendAllClients = function sendAllClients(message) {
    wss.clients.forEach(function each(client) {
        client.send(JSON.stringify(message));
    });
};

module.exports.saveMessage = function saveMessage(userId, message) {
    loginMessagesMap.set(userId, message);
}

module.exports.getMessage = function getMessage(userId) {
    loginMessagesMap.get(userId);
};

module.exports.deleteMessage = function deleteMessage(userId) {
    loginMessagesMap.delete(userId);
};