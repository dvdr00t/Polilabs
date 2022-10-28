/**
 * @file Queue.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the declaration of the Queue class for the LAB06
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#ifndef _QUEUE_H_
#define _QUEUE_H_


#include <vector>
#include <iosfwd>
#include "Client.h"

/* DEFINITION OF THE FRIEND ENCLOSURE FOR THE STACK CLASS INSTANTIATIONS */
class Queue;
ostream& operator << (ostream &os, const Queue &queue);

class Queue {

    public:
        /* DEFAULT CONSTRUCTOR AND DESTRUCTOR */
        Queue();
        ~Queue();

        /* GETTERS AND SETTERS */
        int getSize();

        /* OPERATIONS OF THE POST OFFICE */
        Client serveNext(void);
        void insertNext(Client &client);

        /* OPERATOR OVERLOADING */
        friend ostream& operator<< (ostream &os, const Queue &queue);

    private:
        void _pushAtPosition(Client &client, int position);
        int _getLastPriority();
        int _getLastMoney();
        Client *_dataContainer;
        int _size;
};

/**
 * @brief Defines errors related to an attempt to access an empty queue
 * 
 */
class QueueEmptyException {

    public:

        /* DEFAULT CONSTRUCTOR AND DESTRUCTOR */
        QueueEmptyException() : message_("\nTweedle dee dee... There is no one waiting here...\n\n") {}
        ~QueueEmptyException() {}

        /* GETTER */
        string getMessage(void) const {
            return (this->message_);
        }

    private:
        string message_;         /* Error message                */
};

#endif /* _QUEUE_H_ */