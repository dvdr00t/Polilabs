/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file WSMessage.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * WSMessage.js contains the constructor prototype of a single WebSocket 
 * message.
*/
'use strict';

class WSMessage {

    /**
     * Constructor of an WSMessage Object
     * @param {string} type
     * @param {number} userId
     * @param {string} userName
     * @param {number} filmId
     * @param {string} filmName
     */
    constructor (type, userId, userName, filmId, filmName) {
        this.typeMessage = type;
        this.userId = userId;
        if (userName) this.userName = userName;
        if (filmId) this.filmId = filmId;
        if (filmName) this.filmName = filmName;
    }
}

module.exports = WSMessage;