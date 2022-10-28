/**
 * @file solution.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Exam 2022-01-17
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iostream>
#include <mutex>    
#include <thread>

#define RUNNING_TIME 10         /* number of seconds used to run the simulation                 */
#define PARK_SIZE 2            /* number of places in the parking lot                          */
#define MIN_ENTER 1             /* lower bound of range in which a car enters the parking lot   */
#define MAX_ENTER 3             /* upper bound of range in which a car enters the parking lot   */
#define MIN_LEAVE 4             /* lower bound of range in which a car leaves the parking lot   */
#define MAX_LEAVE 7             /* upper bound of range in which a car leaves the parking lot   */

/* GLOBAL VARIABLE */
int parkingLot = 0;
int countdown = RUNNING_TIME;
bool terminated = false;
std::mutex parkAccess;

void managerRoutine(void) {

    while (true) {

        {
            int leavingTime = rand() % (MAX_LEAVE - MIN_LEAVE + 1) + MIN_LEAVE;

            /* checking simulation time */
            std::unique_lock<std::mutex> managerLock{parkAccess};
            if (terminated) {
                if (leavingTime <= countdown) {
                    std::cout << "A car leaves. [current space: " << PARK_SIZE - parkingLot - 1 << "]" << std::endl;
                }
                return ; 
            }
            std::cout << "--- (manager waiting for " << leavingTime << ") ---" << std::endl;
            managerLock.unlock();

            /* simulate time */
            std::this_thread::sleep_for(std::chrono::seconds(leavingTime));       

            /* freeing up space */
            managerLock.lock();
            if (parkingLot) {
                parkingLot--;
                std::cout << "A car leaves. [current space: " << PARK_SIZE - parkingLot << "]" << std::endl;
            }

        }       // out of scope: lock dies here
    }

    return;
}

void spawnerRoutine(void) {

    /* keeps spawning cars at a random rate */
    while (true) {

        {
            int arrivingTime = rand() % (MAX_ENTER - MIN_ENTER + 1) + MIN_ENTER;

            /* checking simulation time */
            std::unique_lock<std::mutex> spawnerLock{parkAccess};
            if (countdown - arrivingTime <= 0) {
                terminated = true;
                return ; 
            }
            std::cout << "--- (spawner waiting for " << arrivingTime << ") ---" << std::endl;
            spawnerLock.unlock();

            /* simulate time */
            std::this_thread::sleep_for(std::chrono::seconds(arrivingTime));

            /* checking space in the parking lot */
            spawnerLock.lock();
            if (parkingLot == PARK_SIZE) {
                std::cout << "A car arrived but the parking lot is full..." << std::endl;
            } else {
                parkingLot++;
                std::cout << "A car arrived and entered. [current space: " << PARK_SIZE - parkingLot << "]" << std::endl;
            }

            countdown -= arrivingTime;

        }       // out of scope: lock dies here
    }

    return;
}

/**
 * @brief DRIVER CODE
 * 
 * @return int 
 */
int main(int argc, char *argv[]) {

    /* CHECKING INPUT PARAMETERS */
    if (argc != 1) {
        std::cerr << "[!] usage: ./exe03" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "... starting simulation ..." << std::endl;

    /* GENERATING THREADS */
    srand((unsigned int) time(NULL));
    std::thread manager(managerRoutine);
    std::thread spawner(spawnerRoutine);

    /* JOINING THREADS */
    manager.join();
    spawner.join(); 

    /* TASK COMPLETED SUCCESSFULLY */
    std::cout << "[!] simulation terminated" << std::endl;
    exit(EXIT_SUCCESS);
}   