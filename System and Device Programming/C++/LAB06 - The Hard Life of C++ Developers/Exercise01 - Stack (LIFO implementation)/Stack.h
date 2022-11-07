/**
 * @file Stack.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the declaration of the Stack class for the LAB06
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#ifndef _STACK_H_
#define _STACK_H_

#include <vector>
#include <iosfwd>

using namespace std;

/* DEFINITION OF THE FRIEND ENCLOSURE FOR THE STACK CLASS INSTANTIATIONS */
template <typename T> class Stack;
template <typename T> ostream& operator << (ostream &o, const Stack<T> &s);

/**
 * @brief Stack class declaration
 * 
 * @tparam T type of the elements in the stack
 */
template <typename T> class Stack {

    public:

        /* DEFAULT CONSTRUCTOR AND DESTRUCTOR */
        Stack();
        ~Stack();

        /* COPY AND MOVE CONSTRUCTOR AND ASSIGNMENT */   
        Stack(const Stack<T> &other);                           /* copy constructor     */
        Stack(Stack<T> &&other) noexcept;                       /* move constructor     */
        Stack<T>& operator=(const Stack<T> &other);             /* copy assignment      */
        Stack<T>& operator=(Stack<T> &&other) noexcept;         /* move assignment      */

        /* GETTERS */
        int getSize(void);

        /* PUSH AND POP OPERATIONS */
        void push(const T &value);
        T pop(void);

        /* DATA CONTAINER OPERATOR */
        vector<T> getVector(void);
        void reverse(void);

        /* OPERATORS OVERLOADING */
        Stack<T> operator+ (const Stack<T> &toAdd);
        friend ostream& operator<< <T>(ostream &os, const Stack<T> &stack); 
            /**
             * Some notes on the above declaration:
             * 
             *  - friend --> indicates that the operator "<<" (which will be overloaded) is a "friend" of this
             *               class, i.e. it can access all the private parameters inside.
             *  
             *  - ostream& --> reference to the output stream (why reference? because we work with pointers so 
             *                 that it is easier to concatenate outputs (cout << output1 << output2 << ... << endl).
             *                 The overloaded operator returns a reference to the output stream
             * 
             *  - operator --> keyword 
             * 
             *  - << --> operator to be overloaded
             * 
             *  - <T> --> this overloading refers only to this class
             * 
             *  - os --> a reference to the output stream
             * 
             *  - stack --> a reference to the Stack class
             */

    private:
        int _size;              /* Size of the stack            */
        T *_dataContainer;      /* Actual storage for values    */
};

/**
 * @brief Defines errors related to an attempt to access an empty stack
 * 
 */
class StackEmptyException {

    public:

        /* DEFAULT CONSTRUCTOR AND DESTRUCTOR */
        StackEmptyException() : message_("[!] Attempt to pop() with empty stack...") {}
        ~StackEmptyException() {}

        /* GETTER */
        string getMessage(void) const {
            return (this->message_);
        }

    private:
        string message_;         /* Error message                */
};


#endif /* _STACK_H_ */