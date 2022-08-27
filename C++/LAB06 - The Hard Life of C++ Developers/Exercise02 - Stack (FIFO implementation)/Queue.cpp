/**
 * @file Queue.cpp
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the defintions of the Queue class for the LAB06
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iosfwd>
#include <iostream>
#include <exception>
#include "Queue.h"

/**
 * @brief Construct a new Queue:: Queue object
 * 
 */
Queue::Queue() {
    this->_size = 0;
    this->_dataContainer = nullptr;
}

/**
 * @brief Destroy the Queue:: Queue object
 * 
 */
Queue::~Queue() {
    delete[] this->_dataContainer;
    this->_size = 0;
    this->_dataContainer = nullptr;
}

/**
 * @brief Get size of the queue
 * 
 * @return size of the queue
 */
int Queue::getSize(void) {
    return this->_size;
}

/**
 * @brief Get the index of the last client with priority type
 * 
 * @return the index of the last client with priority type
 */
int Queue::_getLastPriority() {

    if (this->_size != 0 && this->_dataContainer[0].getType() != PRIORITY_CLIENT) {
        return -1;
    }

    for (int i = 0; i < this->_size-1; i++) {
        if (this->_dataContainer[i+1].getType() != PRIORITY_CLIENT) {
            return i;   
        }
    }
    return this->_size-1;
}

int Queue::_getLastMoney() {
    int counterMoney = 0;
    for (int i = this->_getLastPriority()+1; i < this->_size-1; i++) {

        if (this->_dataContainer[i].getType() == MONEY_SERVICE_CLIENT) {
            counterMoney++;
        }

        /* CHECK IF WE HAVE ENCOUNTERED 3 MONEY CLIENT */
        if (counterMoney == 3) {
            if (this->_dataContainer[i+1].getType() == MONEY_SERVICE_CLIENT) {
                return i;
            } else {
                counterMoney = 0;
            }
        }
    }

    return this->_size-1;
}

/**
 * @brief Insert a client in the specified position
 * 
 * @param client the client to insert in the queue
 * @param position the position at which the client will be inserted
 */
void Queue::_pushAtPosition(Client &client, int position) {

    /* MAKING ROOM FOR NEW CLIENT */
    this->_size++;
    Client *oldDataContainer = this->_dataContainer;
    this->_dataContainer = new Client[this->_size];

    /* INSERTING BACK CLIENTS PRIOR OF POSITION */
    for (int i = 0; i <= position; i++) {
        this->_dataContainer[i] = oldDataContainer[i];
    }

    /* INSERTING NEW CLIENT */
    this->_dataContainer[position+1] = client;

    /* INSERTING BACK CLIENTS AFTER POSITION */
    for (int i = position+2; i < this->_size; i++) {
        this->_dataContainer[i] = oldDataContainer[i-1];
    }

    delete[] oldDataContainer;  
}

/**
 * @brief pop() operation
 * 
 * @return the next client in the queue (FIFO policy)
 */
Client Queue::serveNext(void) {

    /* CHECKING SIZE OF THE STACK */
    if (this->_size == 0) {
        throw QueueEmptyException();
    }

    /* DUPLICATING POINTER TO DATA CONTAINER */
    Client *oldDataContainer = this->_dataContainer;

    /* RETRIEVING VALUE */
    Client client = this->_dataContainer[0];

   /* CREATING NEW CONTAINER OF THE RIGHT SIZE */
   this->_size--;
   this->_dataContainer = (this->_size == 0) ? nullptr : new Client[this->_size];
   
   /* COPYING BACK VALUES IN THE NEW CONTAINER */
   for (int i = 0; i < this->_size; i++) {
        this->_dataContainer[i] = oldDataContainer[i+1];
   }

    /* RETURN VALUE WITH SUCCESS */
    delete[] oldDataContainer;
    return client;

}

/**
 * @brief push() operation
 * 
 * @param client the client to be inserted in the queue
 */
void Queue::insertNext(Client &client) {

    
    /* POSITION IN THE QUEUE IS BASED ON THE TYPE OF THE CLIENT */
    int index;
    switch(client.getType()) {
        case PRIORITY_CLIENT:
            index = this->_getLastPriority();
            this->_pushAtPosition(client, index);
            break;

        case POSTAL_SERVICE_CLIENT:
            index = this->_getLastMoney();
            this->_pushAtPosition(client, index);
            break;

        case MONEY_SERVICE_CLIENT:
            this->_pushAtPosition(client, this->_size-1);
            break;

        default:
            throw ClientInvalidType();
    }

}

ostream& operator<< (ostream &os, const Queue &queue) {
    os << "QUEUE: " << endl;
    for (int i = 0; i < queue._size; i++) {
        os << queue._dataContainer[i];
    }
    os << endl;
    return os;
}