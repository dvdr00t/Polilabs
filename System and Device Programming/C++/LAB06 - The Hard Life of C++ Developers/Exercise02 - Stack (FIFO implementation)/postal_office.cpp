/**
 * @file postal_office.cpp
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief LAB06 - Exercise02
 * @version 0.1
 * @date 2022-08-27
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iostream>
#include <vector>
#include "Client.h"
#include "Client.cpp"
#include "Queue.h"
#include "Queue.cpp"

using namespace std;

/**
 * @brief Display the main menu
 * 
 * @return choice of the user
 */
int menu(void) {

    int choice;

    /* PRINTING OPTIONS */
    cout << " --- POST OFFICE ---" << endl;
    cout << endl;
    cout << "What can I do for you?" << endl;
    cout << "1) Request a new number for a client" << endl;
    cout << "2) Serve next client" << endl;
    cout << "3) Serve all & close the office" << endl;
    cout << endl;
    cout << "Choose: ";
    cin >> choice;

    return choice;
}

/**
 * @brief Display the type menu
 * 
 * @return type of the client
 */
client_t menuType(void) {

    int choice;

    /* PRINTING OPTIONS */
    cout << endl;
    cout << "What client?" << endl;
    cout << "1) Priority Client" << endl;
    cout << "2) Postal Service Client" << endl;
    cout << "3) Money Service Client" << endl;
    cout << endl;
    cout << "Choose: ";
    cin >> choice;

    switch (choice) {
        case 1:
            return PRIORITY_CLIENT;
            break;
        case 2: 
            return POSTAL_SERVICE_CLIENT;
            break;
        case 3: 
            return MONEY_SERVICE_CLIENT;
            break;
        default:
            return NONE;
            break;
    }
}


/**
 * @brief main
 * 
 * @param argc number of arguments 
 * @param argv list of arguments
 * @return zero on success
 */
int main(int argc, char *argv[]) {

    /* CHECKING INPUT ARGUMENTS */
    if (argc != 1) {
        cout << "[USAGE] ./postal_office" << endl;
        exit(EXIT_FAILURE);
    }
    
    /* STACK DEFINTION */
    Queue queue;

    /* USER MENU */
    Client client;
    client_t type;
    int choice = 1;
    int nc = 0;
    while(choice != -1) {
        choice = menu();
        switch (choice) {
            case 1:
                type = menuType();
                try {
                    client = Client(type);
                    client.setNumber(nc++);
                    queue.insertNext(client);
                } catch (const ClientInvalidType &e) {
                    cout << e.getMessage();
                }
            break;
            
            case 2:
                try {
                    client = queue.serveNext();
                    cout << "SERVED: " << client << endl;
                } catch (const QueueEmptyException &e) {
                    cout << e.getMessage();
                }
            break;
            
            case 3:
                cout << "... serving " << queue.getSize() << " clients ..." << endl;
                try {
                    while (queue.getSize() != 0) {
                        client = queue.serveNext();
                        cout << "SERVED: " << client;
                    }
                } catch (const QueueEmptyException &e) {
                    cout << e.getMessage();
                }
                
                choice = -1;
                break;
            
            default:
                choice = -1;;
        }

        cout << queue;
    }
    
    exit(EXIT_SUCCESS);
}