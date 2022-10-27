/**
 * @file solution_noclass.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Exam 2021-10-18
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iostream>
#include <mutex>    
#include <thread>

#define TARGET_TEMP     18.0        /* temperature to be reached by the heating control system  */
#define STARTING_TEMP   18.5        /* starting temperature of the heating control system       */ 
#define TIME_CHECK      5           /* time between two consequent checks of the control system */

std::mutex controlAccess;
bool active = true;
bool terminator = false;

void targetTemp_routine(float &currentTemp, float &targetTemp) {

    while (true) {

        {   // scoping lock on mutex

            /* READING DESIRED TEMPERATURE FROM INPUT EVERY TIME_CHECK SECONDS */
            std::this_thread::sleep_for(std::chrono::seconds(TIME_CHECK));

            std::cout << "[current temperature: " << currentTemp << "] -- Insert desired temperature: ";
            std::unique_lock<std::mutex> targetAccess{controlAccess};
            std::cin >> targetTemp;
            if (targetTemp == -1) {
                terminator = true;
                return ;
            }

        }   // out of scope: lock is destructed here
    }

    return ;
}

void currentTemp_routine(float &currentTemp, float &targetTemp) {  

    while (true) {

        {   // scoping lock on mutex
            
            std::unique_lock<std::mutex> currentAccess{controlAccess};
            if (terminator) {
                std::cout << "[!] currentTemp_thread ends here..." << std::endl;
                return ;
            }

            /* READING DESIRED TEMPERATURE FROM INPUT EVERY TIME_CHECK SECONDS */
            currentAccess.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(TIME_CHECK-1));

            currentAccess.lock();
            currentTemp += (active) ? 0.3 : -0.3;
            std::cout << "[current temperature: " << currentTemp << "] -- current set the temperature." << std::endl;

        }   // out of scope: lock is destructed here
    }

    return ;
}

void admin_routine(float &currentTemp, float &targetTemp) {

    while (true) {

        {   // scoping lock on mutex

            std::unique_lock<std::mutex> adminAccess{controlAccess};
            if (terminator) {
                std::cout << "[!] adminTemp_thread ends here..." << std::endl;
                return ;
            }

            /* READING DESIRED TEMPERATURE FROM INPUT EVERY TIME_CHECK SECONDS */
            adminAccess.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(TIME_CHECK-2));

            adminAccess.lock();
            if (active) {
                active = (currentTemp > targetTemp + 0.2) ? false : true;
            } else {
                active = (currentTemp <= targetTemp + 0.2) ? true : false;
            }

            if (active) {
                std::cout << "[current temperature: " << currentTemp << "] -- admin set the heating system ON" << std::endl;
            } else {
                std::cout << "[current temperature: " << currentTemp << "] -- admin set the heating system OFF" << std::endl;   
            }

        }   // out of scope: lock is destructed here
    }

    return ;
}

/**
 * @brief DRIVER CODE
 * 
 * @return int 
 */
int main(int argc, char *argv[]) {

    /* CHECKING INPUT PARAMETERS */
    if (argc != 1) {
        std::cerr << "[!] usage: ./exe04" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* GENERATING THREADS */
    float currentTemp = STARTING_TEMP;
    float targetTemp = TARGET_TEMP;
    std::thread targetTemp_thread(targetTemp_routine, std::ref(currentTemp), std::ref(targetTemp));
    std::thread currentTemp_thread(currentTemp_routine, std::ref(currentTemp), std::ref(targetTemp));
    std::thread admin_thread(admin_routine, std::ref(currentTemp), std::ref(targetTemp));

    /* JOINING THREADS */
    targetTemp_thread.join();
    currentTemp_thread.join();
    admin_thread.join();

    /* TASK COMPLETED SUCCESSFULLY */
    exit(EXIT_SUCCESS);
}   