#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#define STRING_LENGTH 16

/**
 * EXERCISE 02
 * Writes a program in C that, using the OpenSSL library, generates randomly the private 
 * key to be used for encrypting data with AES128 in CBC and the needed IV. Pay attention to 
 * select the proper PRNG.
*/

void print_hex_string(char *string, int length, char *title);
int main (int args, char *argv[]) {

    fprintf(stdout, "------ WEEK01 - PRGNGs with OpenSLL ------\n");
    fprintf(stdout, "               [ exe02 ]                  \n");
    fprintf(stdout, "                                          \n");

    /* initializing seed */
    if (RAND_load_file("/dev/random", 64) != 64) {
        fprintf(stderr, "[ERROR] function RAND_load_file failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }

    
    unsigned char key[STRING_LENGTH];
    unsigned char iv[STRING_LENGTH];

    /* generating random string of 16 bytes (128 bits) */
    if (RAND_priv_bytes(key, STRING_LENGTH) != 1) {
        fprintf(stderr, "[ERROR] function RAND_bytes failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }
    if (RAND_priv_bytes(iv, STRING_LENGTH) != 1) {
        fprintf(stderr, "[ERROR] function RAND_bytes failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }
    print_hex_string(key, STRING_LENGTH, "stringA");
    print_hex_string(iv, STRING_LENGTH, "stringB");
    
    exit(EXIT_SUCCESS);
}

void print_hex_string(char *string, int length, char *title) {
    fprintf(stdout, "Printing %s: ", title);
    for (int i = 0; i < length; i++) {
        fprintf(stdout, "%02x", string[i]);
    }
    fprintf(stdout, "\n");
}