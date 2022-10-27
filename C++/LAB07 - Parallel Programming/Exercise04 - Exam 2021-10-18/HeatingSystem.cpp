/**
 * @file HeatingSystem.cpp
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Exam 2021-10-18
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iostream>
#include "HeatingSystem.h"

#define TARGET_TEMP     18.0        /* temperature to be reached by the heating control system  */
#define STARTING_TEMP   18.5        /* starting temperature of the heating control system       */ 

/**
 * @brief Construct a new Heating System:: Heating System object
 * 
 */
HeatingSystem::HeatingSystem() {
    this->_targetTemp = TARGET_TEMP;
    this->_currentTemp = STARTING_TEMP;
    this->_shutdown = false;
    this->_active = true;
}

HeatingSystem::~HeatingSystem() {
    this->_targetTemp = 0;
    this->_currentTemp = 0;
    this->_shutdown = true;
    this->_active = false;
}

/* GETTERS AND SETTERS */
float HeatingSystem::getCurrentTemp(void) { 
    std::unique_lock<std::mutex> lock{this->_access};
    return this->_currentTemp; 
};
void HeatingSystem::setCurrentTemp(float value) { 
    std::unique_lock<std::mutex> lock{this->_access};
    this->_currentTemp = value; 
};
float HeatingSystem::getTargetTemp(void) {
    std::unique_lock<std::mutex> lock{this->_access};
    return this->_targetTemp; 
};
void HeatingSystem::setTargetTemp(float value) { 
    std::unique_lock<std::mutex> lock{this->_access};
    this->_targetTemp = value; 
};
bool HeatingSystem::isActive(void) { 
    std::unique_lock<std::mutex> lock{this->_access};
    return this->_active; 
};
void HeatingSystem::setActive(bool newActive) { 
    std::unique_lock<std::mutex> lock{this->_access};
    this->_active = newActive; 
};
bool HeatingSystem::checkShutdown(void) { 
    std::unique_lock<std::mutex> lock{this->_access};
    return this->_shutdown; 
};
void HeatingSystem::shutdown(void) { 
    std::unique_lock<std::mutex> lock{this->_access};
    this->_shutdown = true; 
};

const unsigned int HeatingSystem::getTimeInput(void) { 
    std::unique_lock<std::mutex> lock{this->_access};
    return this->_timeInput; 
};
const unsigned int HeatingSystem::getTimeChange(void) { 
    std::unique_lock<std::mutex> lock{this->_access};
    return this->_timeChange; 
};
const unsigned int HeatingSystem::getTimeAdmin(void) { 
    std::unique_lock<std::mutex> lock{this->_access};
    return this->_timeAdmin; 
};
