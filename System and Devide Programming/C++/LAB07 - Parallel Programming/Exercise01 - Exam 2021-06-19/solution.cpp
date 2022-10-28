/**
 * @file solution.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Exam 2022-06-19
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
*/


/**
 * NB: code was already provided. Only requirement was to implement the 
 *     admin_f() and adder_f() functions.
*/

#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>

std::mutex m;
std::condition_variable adminCV, adderCV; 
int var = 0;
bool terminated = false;

void admin_f() {

    std::unique_lock<std::mutex> adminLock{m};               // creating a new lock and calling adminLock.lock()
    var = 10;
    std::cout << "> var is now 10..." << std::endl;

    /* waiting */
    adderCV.notify_all();       // when doing notify_all(), var is already changed so do not care about notify_one()
                                // to happen before wait()
    adminCV.wait(adminLock);    // performs: adminLock.unlock() + put the thread in the waiting list
                                // on wakeup: adminLock.lock(), i.e. reacquire the lock

    /* woken up */
    while (!terminated)
        adminCV.wait(adminLock);    // main thread will wake me up again once all the adders ended;
    std::cout << "> Final value is: " << var << std::endl;
}                                                           // out-of-scope: calling adminLock.unlock()

void adder_f(int id) {
        
    std::unique_lock<std::mutex> adderLock{m};   // creating a new lock and calling adderLock.lock()
    
    /* waiting until we are sure that the admin initialize the variable */
    while (var == 0) {
        adderCV.wait(adderLock);
    }

    int toAdd = rand() % 6; // computing value to be added

    /* checking if we still have to add the value */
    if (var == 15) {
        std::cout << "> thread " << id << " added 0 to var..." << std::endl;
        return;
    } 

    toAdd = (var + toAdd > 15) ? 15 - var : toAdd;
    var += toAdd;
    std::cout << "> thread " << id << " added " << toAdd << " to var..." << std::endl;

    /* checking if we need to notify the end of the program to the admin */
    if (var == 15) {
        adminCV.notify_one();
        return;
    }

    return;
}

int main() {

    std::vector<std::thread> adders; 
    std::thread admin(admin_f);
    for(int i = 0; i < 3; i++) {
        srand((unsigned)time(NULL)); //makes seed different for calling rand()
        adders.emplace_back(std::thread(adder_f, i)); 
    }
    
    for(auto& i : adders) {
        i.join(); 
    }

    terminated = true;
    adminCV.notify_one(); 
    admin.join();

    return 0; 
}