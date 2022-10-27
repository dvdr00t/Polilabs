/**
 * @file Client.cpp
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the definitions of the Client class for the LAB06
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iosfwd>
#include <iostream>
#include <exception>
#include "Client.h"

using namespace std;

/**
 * @brief Default constructor
 * 
 */
Client::Client() {
    this->_number = -1;
    this->_type = NONE;
}

/**
 * @brief Construct a new Client
 * 
 * @param _number number in the queue
 * @param type type of the client
 */
Client::Client(client_t type) {
    this->_number = -1;
    this->_type = type;
}   

/**
 * @brief Destroy the Client
 * 
 */
Client::~Client() {
    this->_number = -1;
    this->_type = NONE;
}

/* GETTERS AND SETTERS */
void Client::setNumber(int number) { this->_number = number; }
int Client::getNumber() { return this->_number; }
void Client::setType(client_t type) { this->_type = type; }
client_t Client::getType() { return this->_type; }


/* OPERATORS OVERLOADING */
ostream& operator<< (ostream &os, const Client &client) {
    os << "{ number: " << client._number << " - type: ";
    switch (client._type) {
    case PRIORITY_CLIENT:
        os << "PRIORITY_CLIENT }" << endl;
        break;
    
    case POSTAL_SERVICE_CLIENT:
        os << "POSTAL_SERVICE_CLIENT }" << endl;
        break;

    case MONEY_SERVICE_CLIENT:
        os << "MONEY_SERVICE_CLIENT }" << endl;
        break;

    default:
        break;
    }

    return os;
}
