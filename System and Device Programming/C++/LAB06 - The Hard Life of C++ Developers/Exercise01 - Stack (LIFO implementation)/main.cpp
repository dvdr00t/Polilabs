/**
 * @file main.cpp
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Test driver of the Stack class
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iostream>
#include <vector>
#include "Stack.h"
#include "Stack.cpp"

using namespace std;

int main (int argc, char *argv[]) {

    /* CHECKING INPUT ARGUMENTS */
    if (argc != 1) {
        cerr << "[USAGE] ./test_stack" << endl;
        exit(EXIT_FAILURE);
    }

    /* DEFINING THE STACK */
    Stack<int> myStack;

    /* ADDING SOME VALUE TO THE STACK */
    myStack.push(1);
    myStack.push(2);
    myStack.push(3);
    cout << myStack << endl;

    /* TRY TO POP VALUES */
    int value;
    for (int i = 0; i < 4; i++) {
        try {

            value = myStack.pop();
            cout << "Extracted value is: " << value << endl;
            cout << myStack << endl;

        } catch(const StackEmptyException &e) {
            
            // last pop should catch this exception
            cout << e.getMessage() << endl;

        }
        
    }

    /* CONCATENATE ANOTHER STACK */
    Stack<int> toAdd;
    myStack.push(1);
    myStack.push(2);
    myStack.push(3);
    toAdd.push(4);
    toAdd.push(5);
    toAdd.push(6);
    cout << "myStack: " << endl;
    cout << myStack << endl;
    cout << "toAdd: " << endl;
    cout << toAdd << endl;
    Stack<int> result = myStack + toAdd;
    cout << "result: " << endl;
    cout << result << endl;

    try {
        int value = result.pop();
        cout << "Extracted value is: " << value << endl;
        cout << result << endl;
    } catch (const StackEmptyException &e) {
        cout << e.getMessage() << endl;
    }


    /* TRANSFORM STACK INTO VECTOR */
    vector<int> vectoredStack = result.getVector();
    cout << "> Stack as vector: [" << vectoredStack[0];
    for (int i = 1; i < result.getSize(); i++) {
        cout << " - " << vectoredStack[i];
    }
    cout << "]" << endl;

    /* COPY AND MOVE CONSTRUCTOR AND ASSIGNMENT OPERATOR */
    cout << "-------------------------------------" << endl << endl;
    Stack<int> stack1;
    stack1.push(9);
    stack1.push(8);
    cout << "[!] stack1" << endl;
    cout << stack1;

    Stack<int> stack2 = stack1;         /* copy constructor */
    Stack<int> stack3;
    stack3 = stack1;                    /* copy operator    */
    cout << "[!] stack1" << endl;
    cout << stack1;
    cout << "[!] stack2" << endl;
    cout << stack2;
    cout << "[!] stack3" << endl;
    cout << stack3;

    Stack<int> stack4 = move(stack1);     /* move constructor */   
    stack1.push(1);
    cout << "[!] stack1" << endl;
    cout << stack1;
    cout << "[!] stack4" << endl;
    cout << stack4;
    Stack<int> stack5;
    stack5 = move(stack4);                /* move operator    */
    cout << "[!] stack4" << endl;
    cout << stack4;
    cout << "[!] stack5" << endl;
    cout << stack5;

    cout << "-------------------------------------" << endl << endl;

    /* REVERSE A STACK */
    cout << endl << endl;
    cout << result;
    result.reverse();
    cout << result;

    /* TEST COMPLETED SUCCESSFULLY */
    exit(EXIT_SUCCESS);
}