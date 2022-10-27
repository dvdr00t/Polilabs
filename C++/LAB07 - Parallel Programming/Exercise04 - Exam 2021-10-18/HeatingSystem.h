/**
 * @file HeatingSystem.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Exam 2021-10-18
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#ifndef _HEATINGSYSTEM_H_
#define _HEATINGSYSTEM_H_

#include <iostream>
#include <mutex>

#define TIME_CHECK 5        /* ticks on the HeatingSystem */

class HeatingSystem {

    public:

        /* CONSTRUCTORS AND DESTRUCTORS */
        HeatingSystem();
        ~HeatingSystem();

        /* GETTERS AND SETTERS */
        float getCurrentTemp(void);
        void setCurrentTemp(float value);
        float getTargetTemp(void);
        void setTargetTemp(float value);
        bool isActive(void);
        void setActive(bool newActive);
        bool checkShutdown(void);
        void shutdown(void);
        const unsigned int getTimeInput(void);
        const unsigned int getTimeChange(void);
        const unsigned int getTimeAdmin(void);

        /* OBTAIN THE MUTEX */
        std::mutex getMutex(void);


    private:
        float _targetTemp;          /* temperature to be reached by the heating control system          */
        float _currentTemp;         /* current temperature of the heating control system                */
        bool _active;               /* indicates whether the heating system is active (ON) or not       */
        bool _shutdown;             /* indicates whether the heating system requires shutdown or not    */
        std::mutex _access;         /* manages access to the heating system                             */

        const unsigned int _timeInput = TIME_CHECK;
        const unsigned int _timeChange = TIME_CHECK-1;
        const unsigned int _timeAdmin = TIME_CHECK-2;
};

#endif /* _HEATINGSYSTEM_H_ */