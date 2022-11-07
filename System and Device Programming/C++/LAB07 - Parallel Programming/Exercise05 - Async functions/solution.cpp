/**
 * @file solution.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Async
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <future>

#define MAX_VALUE 4        /* max value that a element of the matrix can assume */

/**
 * @brief Compute the sum of products of the two given arrays.
 *      
 *      - v1.begin() and v2.begin() indicates the starting point of the two arrays
 *      - v1.end() indicates when the sum should end (v1.end() - v1.begin() elements)
 *      - 0 indicates the starting value of the accumulator
 * 
 * @param v1 first array
 * @param v2 second array
 * @return the sum of products
 */
int arrayMul(const std::vector<int> &v1, const std::vector<int> &v2) {
    return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
}

/**
 * @brief Generate a random matrix of size row x column and stores it in matrix.
 * 
 * @param matrix stores the result
 * @param row size of the matrix
 * @param column size of the matrix
 */
void generateRandomMatrix(std::vector<std::vector<int>> &matrix, const int &row, const int &column) {
    for (int i = 0; i < row; i++) {
        std::vector<int> row;
        for (int j = 0; j < column; j++) {
            row.push_back(rand() % MAX_VALUE);
        }
        matrix.push_back(row);
    }
}

/**
 * @brief Display a matrix
 * 
 * @param matrix the matrix
 */
void printMatrix(std::vector<std::vector<int>> &matrix) {
    for (std::vector<int> &row : matrix){
        for (int element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief DRIVER CODE
 * 
 * @return int 
 */
int main(int argc, char *argv[]) {

    /* CHECKING INPUT PARAMETERS */
    if (argc != 1) {
        std::cerr << "[!] usage: ./exe05" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* TAKING THE SIZE OF THE MATRICES */
    int rowA, columnA, rowB, columnB;
    std::cout << "> Insert rowA: ";
    std::cin >> rowA;
    std::cout << "> Insert columnA = rowB: ";
    std::cin >> columnA;
    rowB = columnA;
    std::cout << "> Insert columnB: ";
    std::cin >> columnB;

    /* GENERATES RANDOM MATRICES */
    srand((unsigned int) time(NULL));
    std::vector<std::vector<int>> A;
    std::vector<std::vector<int>> B;
    generateRandomMatrix(A, rowA, columnA);
    generateRandomMatrix(B, columnB, rowB);     // rows and columns inverted (transposed)

    /* COMPUTE RESULT USING FUTURES */
    std::vector<std::vector<std::future<int>>> C; // a matrix where each element is a "future" int, i.e. the result
    for (int i = 0; i < rowA; i++) {
        std::vector<std::future<int>> row;
        for (int j = 0; j < columnB; j++) {
            std::future<int> result = std::async(std::launch::async | std::launch::deferred, arrayMul, A[i], B[j]);
            row.push_back(std::move(result));   // futures can't be copied, so move
        }
        C.push_back(std::move(row));    // same thing here, use move
    }

    /* SHOW DATA */
    printMatrix(A);
    std::cout << std::endl;
    printMatrix(B);
    std::cout << std::endl;

    /* PRINT RESULT */
    std::ofstream outputFile;
    outputFile.open("output.txt");
    outputFile << "RESULT: " << std::endl;
    for (std::vector<std::future<int>> &row : C) {
        for (std::future<int> &result : row) {
            outputFile << result.get() << " ";
        }
        outputFile << std::endl;
    }

    /* TASK COMPLETED SUCCESSFULLY */
    outputFile.close();
    exit(EXIT_SUCCESS);
}   