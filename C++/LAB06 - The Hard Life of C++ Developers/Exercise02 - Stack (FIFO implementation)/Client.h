/**
 * @file Client.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the declaration of the Client class for the LAB06
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <iosfwd>

using namespace std;

/**
 * @brief Define the type of a client in the queue
 * 
 */
enum client_t {
    PRIORITY_CLIENT,
    POSTAL_SERVICE_CLIENT,
    MONEY_SERVICE_CLIENT,
    NONE
};

/* DEFINITION OF THE FRIEND ENCLOSURE FOR THE CLIENT CLASS INSTANTIATIONS */
class Client;
ostream& operator << (ostream &os, const Client &client);

/**
 * @brief Client class
 * 
 */
class Client {

    public:

        /* CONSTRUCTOR AND DESTRUCTOR */
        Client();
        Client(client_t type);
        ~Client();

        /* GETTERS AND SETTERS */
        int getNumber();
        void setNumber(int number);
        client_t getType();
        void setType(client_t type);

        /* OPERATORS OVERLOADING */
        friend ostream& operator<< (ostream &os, const Client &client);

    private:
        int _number;
        client_t _type;
};

class ClientInvalidType {

    public:

        /* DEFAULT CONSTRUCTOR AND DESTRUCTOR */
        ClientInvalidType() : message_("\nWhat kind of client are you???\n\n") {}
        ~ClientInvalidType() {}

        /* GETTER */
        string getMessage(void) const {
            return (this->message_);
        }

    private:
        string message_;         /* Error message                */
};

#endif /* _CLIENT_H_ */