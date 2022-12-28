/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file MQTTMessage.js 
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * MQTTMessage.js contains the constructor prototype of a single MQTT message.
*/
'use strict';

class MQTTMessage {

    /**
     * Constructor of a MQTTMessage Object
     * @param {String} status 
     * @param {Number} userId 
     * @param {String} userName 
     */
    constructor (status, userId, userName) {
        this.status = status;
        if (userId) this.userId = userId;
        if (userName) this.userName = userName;
    }
}

module.exports = MQTTMessage;