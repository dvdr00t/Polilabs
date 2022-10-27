/**
 * @file solutio_class.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Exam 2021-10-18
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iostream>
#include <thread>
#include "HeatingSystem.h"
#include "HeatingSystem.cpp"


/**
 * @brief Thread routine: input the desired temperature
 * 
 * @param controller the HeatingSystem object
 */
void targetTemp_routine(HeatingSystem &controller) {

    float target;
    while (!controller.checkShutdown()) {

        {   // scoping lock on mutex

            /* READING DESIRED TEMPERATURE FROM INPUT EVERY TIME_CHECK SECONDS */
            std::this_thread::sleep_for(std::chrono::seconds(controller.getTimeInput()));

            std::cout << "[current temperature: " << controller.getCurrentTemp() << "] -- Insert desired temperature: " << std::endl;

            /* ACQUIRING INPUT AND CHECK */
            std::cin >> target;
            if (target == -1) {
                controller.shutdown();
                return ;
            }
            controller.setTargetTemp(target);

        }   // out of scope: lock is destructed here
    }

    return ;
}

/**
 * @brief Thread routine: change the current temperature
 * 
 * @param controller the HeatingSystem object
 */
void currentTemp_routine(HeatingSystem &controller) {

    float current;
    while (!controller.checkShutdown()) {
        
        {   // scoping lock on mutex
            
            /* READING DESIRED TEMPERATURE FROM INPUT EVERY TIME_CHECK SECONDS */
            std::this_thread::sleep_for(std::chrono::seconds(controller.getTimeChange()));

            if (controller.isActive()) {
                current = controller.getCurrentTemp() + 0.3;
            } else {
                current = controller.getCurrentTemp() - 0.3;
            }
            controller.setCurrentTemp(current);
            std::cout << "[current temperature: " << controller.getCurrentTemp() << "] -- current set the temperature." << std::endl;

        }   // out of scope: lock is destructed here
        
    }

    std::cout << "[!] currentTemp_thread ends here..." << std::endl;
    return ;
}

/**
 * @brief Thread routine: turns ON/OFF the controller
 * 
 * @param controller the HeatingSystem object
 */
void admin_routine(HeatingSystem &controller) {

    while (!controller.checkShutdown()) {

        {   // scoping lock on mutex

            /* READING DESIRED TEMPERATURE FROM INPUT EVERY TIME_CHECK SECONDS */
            std::this_thread::sleep_for(std::chrono::seconds(controller.getTimeAdmin()));

            // std::unique_lock<std::mutex> adminAccess{controlAccess};
            if (controller.isActive()) {
                bool newActive = (controller.getCurrentTemp() > controller.getTargetTemp() + 0.2) ? false : true;
                controller.setActive(newActive);
            } else {
                bool newActive = (controller.getCurrentTemp() <= controller.getTargetTemp() + 0.2) ? true : false;
                controller.setActive(newActive);
            }

            if (controller.isActive()) {
                std::cout << "[current temperature: " << controller.getCurrentTemp() << "] -- admin set the heating system ON" << std::endl;
            } else {
                std::cout << "[current temperature: " << controller.getCurrentTemp() << "] -- admin set the heating system OFF" << std::endl;   
            }

        }   // out of scope: lock is destructed here
    }

    std::cout << "[!] adminTemp_thread ends here..." << std::endl;
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
    HeatingSystem controller;
    std::thread targetTemp_thread(targetTemp_routine, std::ref(controller));
    std::thread currentTemp_thread(currentTemp_routine, std::ref(controller));
    std::thread admin_thread(admin_routine, std::ref(controller));

    /* JOINING THREADS */
    targetTemp_thread.join();
    currentTemp_thread.join();
    admin_thread.join();

    /* TASK COMPLETED SUCCESSFULLY */
    exit(EXIT_SUCCESS);
}   