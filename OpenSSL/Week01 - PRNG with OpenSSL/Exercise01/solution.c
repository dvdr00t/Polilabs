#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#define STRING_LENGTH 16

/**
 * EXERCISE 01
 * Write a program in C that, using the OpenSSL library, generates two 128 bit random strings. 
 * Then, it XOR them (bitwise) and prints the result on the standard output as an hex string.
*/

void xor_string(char *stringA, char *stringB, char *stringC, int length);
void print_hex_string(char *string, int length, char *title);
int main (int args, char *argv[]) {

    fprintf(stdout, "------ WEEK01 - PRGNGs with OpenSLL ------\n");
    fprintf(stdout, "               [ exe01 ]                  \n");
    fprintf(stdout, "                                          \n");

    /* initializing seed */
    if (RAND_load_file("/dev/random", 64) != 64) {
        fprintf(stderr, "[ERROR] function RAND_load_file failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }

    
    unsigned char stringA[STRING_LENGTH];
    unsigned char stringB[STRING_LENGTH];
    unsigned char stringC[STRING_LENGTH];

    /* generating random string of 16 bytes (128 bits) */
    if (RAND_bytes(stringA, STRING_LENGTH) != 1) {
        fprintf(stderr, "[ERROR] function RAND_bytes failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }
    if (RAND_bytes(stringB, STRING_LENGTH) != 1) {
        fprintf(stderr, "[ERROR] function RAND_bytes failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }
    print_hex_string(stringA, STRING_LENGTH, "stringA");
    print_hex_string(stringB, STRING_LENGTH, "stringB");

    /* computing xor */
    xor_string(stringA, stringB, stringC, STRING_LENGTH);
    print_hex_string(stringC, STRING_LENGTH, "stringC");
    
    exit(EXIT_SUCCESS);
}

void xor_string(char *stringA, char *stringB, char *stringC, int length) {
    for (int i = 0; i < length; i++) {
        stringC[i] = stringA[i] ^ stringB[i];
    }
}

void print_hex_string(char *string, int length, char *title) {
    fprintf(stdout, "Printing %s: ", title);
    for (int i = 0; i < length; i++) {
        fprintf(stdout, "%02x", string[i]);
    }
    fprintf(stdout, "\n");
}