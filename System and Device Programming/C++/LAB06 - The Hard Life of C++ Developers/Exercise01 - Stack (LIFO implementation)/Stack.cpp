/**
 * @file Stack.cpp
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the logic of the Stack class for the LAB06
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iosfwd>
#include <iostream>
#include <exception>
#include "Stack.h"

using namespace std;

/**
 * @brief Construct a new Stack< T>:: Stack object
 * 
 * @tparam T type of the elements in the stack
 */
template <typename T> 
Stack<T>::Stack() {
    this->_dataContainer = nullptr;
    this->_size = 0;
}

/**
 * @brief Destroy the Stack< T>:: Stack object
 * 
 * @tparam T type of the elements in the stack
 */
template <typename T> 
Stack<T>::~Stack() {
    delete[] this->_dataContainer;
    this->_dataContainer = nullptr;
    this->_size = 0;
}

/**
 * @brief Copy constructor
 * 
 * @tparam T type of the objects in the stack
 * @param other object to copy from 
 */
template <typename T>
Stack<T>::Stack(const Stack<T> &other) {

    this->_size = other._size;
    this->_dataContainer = new T[this->_size];
    for (int i = 0; i < this->_size; i++) {
        this->_dataContainer[i] = other._dataContainer[i];
    }

}

/**
 * @brief Copy assignment
 * 
 * @tparam T type of the objects in the stack
 * @param other object to copy from 
 * @return Stack<T>& the new Stack
 */
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T> &other) {

    /* CHECKING TWO OBJECTS TO BE ACTUALLY DIFFERENT */
    if (this == &other) {
        return *this;
    }
    
    /* CLEANING UP ALREADY EXISTING OBJECT */
    delete[] this->_dataContainer;
    
    /* ASSIGNMENT OF NEW VALUES */
    this->_size = other._size;
    this->_dataContainer = new T[this->_size];
    for (int i = 0; i < this->_size; i++) {
        this->_dataContainer[i] = other._dataContainer[i];
    }

    return *this;
}

/**
 * @brief Move constructor
 * 
 * @tparam T type of the objects in the stack
 * @param other object to copy from 
 */
template <typename T>
Stack<T>::Stack(Stack<T> &&other) noexcept {

    this->_size = other._size;
    this->_dataContainer = other._dataContainer;
    other._size = 0;
    other._dataContainer = nullptr;
    
}

/**
 * @brief Move assignment
 * 
 * @tparam T type of the objects in the stack
 * @param other object to copy from 
 * @return Stack<T>& the new Stack
 */
template <typename T>
Stack<T>& Stack<T>::operator=(Stack<T> &&other) noexcept {

    /* CHECKING TWO OBJECTS TO BE ACTUALLY DIFFERENT */
    if (this == &other) {
        return *this;
    }
    
    /* CLEANING UP ALREADY EXISTING OBJECT */
    delete[] this->_dataContainer;
    
    /* ASSIGNMENT OF NEW VALUES */
    this->_size = other._size;
    this->_dataContainer = other._dataContainer;
    other._size = 0;
    other._dataContainer = nullptr;

    return *this;

}



/**
 * @brief Get the size of the stack
 * 
 * @return int the size of the stack
 */
template <typename T> 
int Stack<T>::getSize(void) {
    return this->_size;
}

/**
 * @brief Insert the object value in the first position of the stack (LIFO)
 * 
 * @tparam T type of the elements in the stack
 * @param value object to be inserted in the stack
 */
template <typename T> 
void Stack<T>::push(const T &value) {

    /* DUPLICATING POINTER TO DATA CONTAINER */
    T *oldDataContainer = this->_dataContainer;

    /* CREATING NEW CONTAINER OF THE RIGHT SIZE */
    this->_size++;
    this->_dataContainer = new T[this->_size];   // allocate _size space in memory of T values

    /* NEW VALUE IS PLACED AT THE BEGINNING OF THE STACK */
    this->_dataContainer[0] = value;
    for (int i = 1; i < this->_size; i++) {
        this->_dataContainer[i] = oldDataContainer[i-1];
    }

    /* CLEANING UP SOME MEMORY */
    delete[] oldDataContainer;

}

/**
 * @brief Remove the object value in the first position of the stack (POP)
 * 
 * @tparam T type of the object in the stack
 * @return T object retrieved from the stack
 */
template <typename T> 
T Stack<T>::pop(void) {

    /* CHECKING SIZE OF THE STACK */
    if (this->_size == 0) {
        throw StackEmptyException();
    }

    /* DUPLICATING POINTER TO DATA CONTAINER */
    T *oldDataContainer = this->_dataContainer;

    /* RETRIEVING VALUE */
    T value = this->_dataContainer[0];

   /* CREATING NEW CONTAINER OF THE RIGHT SIZE */
   this->_size--;
   if (this->_size == 0) {
        this->_dataContainer = nullptr;
   } else { 
        this->_dataContainer = new T[this->_size];
   }
   
   /* COPYING BACK VALUES IN THE NEW CONTAINER */
   for (int i = 0; i < this->_size; i++) {
        this->_dataContainer[i] = oldDataContainer[i+1];
   }

    /* RETURN VALUE WITH SUCCESS */
    delete[] oldDataContainer;
    return value;
}

/**
 * @brief Transform the stack into an array of objects
 * 
 * @tparam T type of the objects in the stack
 * @return vector<T> the array of objects
 */
template <typename T>
vector<T> Stack<T>::getVector(void) {
    
    /* CREATE THE ARRAY */
    vector<T> result;

    /* FILLING THE ARRAY */
    for (int i = 0; i < this->_size; i++) {
        result.push_back(this->_dataContainer[i]);
    }
    
    return result;
}

/**
 * @brief Reverse the order of objects in the stack
 * 
 * @tparam T type of the objects in the stack
 */
template <typename T>
void Stack<T>::reverse(void) {

    /* REVERSE STACK WITH TMP VALUE */
    T tmp;
    for (int i = 0, j = this->_size-1; i < this->_size/2; i++, j--) {
        tmp = this->_dataContainer[j];
        this->_dataContainer[j] = this->_dataContainer[i];
        this->_dataContainer[i] = tmp;
    }
}

/**
 * @brief Overloads the + operator between two stacks
 * 
 * @tparam T type of the objects in the stack
 * @param toAdd stack to concatenate
 * @return Stack<T> resulting stack
 */
template <typename T>
Stack<T> Stack<T>::operator+ (const Stack<T> &toAdd) {

    /* CREATE A NEW STACK OF THE PROPER SIZE */
    Stack<T> result;
    result._size = this->_size + toAdd._size;
    result._dataContainer = new T[result._size];

    /* ADDING VALUES */
    for (int i = 0; i < this->_size; i++) {
        result._dataContainer[i] = this->_dataContainer[i];
    }
    for (int j = 0, i = this->_size; j < toAdd._size; i++, j++) {
        result._dataContainer[i] = toAdd._dataContainer[j];
    }

    return result;
}

/**
 * @brief Overloads the << operator
 * 
 * @tparam T type of the values in the stack
 * @param os reference to the output stream
 * @param stack stack to be printed in the output stream
 * @return ostream& reference to the output stream
 */
template <typename T>
ostream& operator << (ostream &os, const Stack<T> &stack) {

    /* IF THE STACK IS EMPTY */
    if (stack._size == 0) {
        os << "> STACK: nullptr" << endl;
        return os;
    } 


    /*
     *          + --- + --- + --- +
     * > STACK: |  6  |  1  |  7  |
     *          + --- + --- + --- +
    */

    /* FIRST LINE */
    os << "         +";
    for (int i = 0; i < stack._size; i++) {
        os << " --- +";
    }
    os << endl;

    /* SECOND LINE */
    os << "> STACK: |";
    for (int i = 0; i < stack._size; i++) {
        os << "  " << stack._dataContainer[i] << "  |";
    }
    os << endl;

    /* THIRD LINE */
    os << "         +";
    for (int i = 0; i < stack._size; i++) {
        os << " --- +";
    }
    os << endl;

    return os;
}
