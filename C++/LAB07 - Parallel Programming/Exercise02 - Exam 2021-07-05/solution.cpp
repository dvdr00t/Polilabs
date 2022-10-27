/**
 * @file solution.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Exam 2022-07-05
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iostream>
#include <future>
#include <thread>

/**
 * @brief Thread: routine: print the result in the stdout
 * 
 * @param future get the result
 */
void give_answer(std::future<bool> &future) {
    bool result = future.get();
    if (result) {
        std::cout << "IS PRIME!" << std::endl;
    } else {
        std::cout << "NOOOO!" << std::endl;
    }

    return;
}

/**
 * @brief Thread routine: evaluates whether a value is prime
 * 
 * @param future get the value
 * @param promise stores the result
 */
void check_prime(std::future<int> &future, std::promise<bool> &promise) {

    /* RECEIVING THE VALUE */
    int value = future.get();

    /* TRIVIAL CASES */
    if (value <= 3) {
        promise.set_value(true);
        return;
    }

    /* CHECKING PRIME */
    bool isPrime = true;
    for (int i = 2; i < value; i++) {
        if (value % i == 0) {
            isPrime = false;
        }
    }

    /* RETURN VALUE */
    promise.set_value(isPrime);
    return;
}

/**
 * @brief Thread routine: receive a value in input
 * 
 * @param promise stores the received value
 */
void take_value(std::promise<int> &promise) {
    int value;
    std::cout << "> Insert a value: ";
    std::cin >> value;
    promise.set_value(value);
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
        std::cerr << "[!] usage: ./exe02" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* DEFINING PROMISES */
    std::promise<int> promise_take_value;
    std::future<int> future_check_prime = promise_take_value.get_future();
    std::promise<bool> promise_computed_result;
    std::future<bool> future_result = promise_computed_result.get_future();

    /* CREATING THREADS */
    std::thread t1(take_value, std::ref(promise_take_value));
    std::thread t2(check_prime, std::ref(future_check_prime), std::ref(promise_computed_result));
    std::thread t3(give_answer, std::ref(future_result));

    /* JOINING THREADS */
    t1.join();
    t2.join();
    t3.join();

    /* TASK COMPLETED SUCCESSFULLY */
    exit(EXIT_SUCCESS);
}