/**
 *				   REST API IMPLEMENTATION - LAB 01 
 * ----------------------------------------------------------------------------
 *  Course: Distributed System Programming
 *  Teacher: Riccardo Sisto 
 *  Date: 2022-11-24
 * ----------------------------------------------------------------------------
 * @file dao.js manages the accesses to the DB
 * @author Davide Arcolini
 * ----------------------------------------------------------------------------
 * Data Access Object (DAO) is used as the lowest level of logic implementation
 * to access directly the DB. It implements all the utility functions required
 * by the controller to access the DB and manage data. To do so, it exploits 
 * three functions:
 *      - run(): it executes a query without returning any result
 *      - get(): it executes a query, returning the first element of the result
 *      - all(): it executes a query, returning all the elements of the result
 * ----------------------------------------------------------------------------
*/
'use strict';

/* --------- IMPORT MODULES --------- */
const sqlite =  require('sqlite3');
 
/**
 * Data Access Object DAO()
 */
class DAO {
 
    /* Attributes */
    static database;
 
    /**
     *  Constructor of the class DAO
     */
    constructor () {
        this.database = new sqlite.Database('database.db', (error) => {
            if (error) {
                console.log(error);
                throw new Error(error.message);
            }
        });
    }
    
    /**
     * Run the query without result in return
     * @param {String} querySQL query that will be run
     * @param {Array} params list of parameters of the query
     * @returns a Promise either resolving with the lastID of the entry in the DB or rejecting with error
     */
    run = (querySQL, params = []) => {
        return new Promise((resolve, reject) => {
            this.database.run(querySQL, params, function (error) {
                if (error) {
                    console.log("[!] ERROR: error running SQL: " + querySQL + "\n" + error);
                    reject(error);
                } else {
                    resolve({id: this.lastID});
                }
            });
        });
    }
 
    /**
     * Run the query returning the first row as result
     * @param {String} querySQL query that will be run
     * @param {Array} params list of parameters of the query
     * @returns a Promise either resolving with the first row of the result or rejecting with error
     */
    get = (querySQL, params = []) => {
        return new Promise((resolve, reject) => {
            this.database.get(querySQL, params, (error, row) => {
                if (error) {
                    console.log("[!] ERROR: error running SQL: " + querySQL + "\n" + error);
                    reject(error);
                } else {
                    resolve(row);
                }
            });
        });
    }
 
    /**
     * Run the query returning the all the rows as result
     * @param {String} querySQL query that will be run
     * @param {Array} params list of parameters of the query
     * @returns a Promise either resolving with all the rows of the result or rejecting with error
     */
    all = (querySQL, params = []) => {
        return new Promise((resolve, reject) => {
            this.database.all(querySQL, params, (error, rows) => {
                if (error) {
                    console.log("[!] ERROR: error running SQL: " + querySQL + "\n" + error);
                    reject(error);
                } else {
                    resolve(rows);
                }
            });
        });
    }

}
 
module.exports = DAO;