#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#define INTEGER_SIZE 4                      /* 4 bytes = 32 bits */

/**
 * EXERCISE 02
 * Using OpenSSL, generate two 32 bit integers (int), multiply them (modulo 2^32) and print the result.
*/

void print_hex_value(char *number, int length, char *title);
int convert_bytes_to_int(char *number, int nbytes);
int main (int args, char *argv[]) {

    fprintf(stdout, "------ WEEK01 - PRGNGs with OpenSLL ------\n");
    fprintf(stdout, "               [ exe03 ]                  \n");
    fprintf(stdout, "                                          \n");

    /* initializing seed */
    if (RAND_load_file("/dev/random", 64) != 64) {
        fprintf(stderr, "[ERROR] function RAND_load_file failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }

    unsigned char A_hex[INTEGER_SIZE];       /* 32 bits integer */
    unsigned char B_hex[INTEGER_SIZE];       /* 32 bits integer */
    unsigned long long int C_int = 0;        /* result of the operation */

    /* generate random numbers (just as for strings) */
    if (RAND_bytes(A_hex, INTEGER_SIZE) != 1 || RAND_bytes(B_hex, INTEGER_SIZE) != 1) {
        fprintf(stderr, "[ERROR] function RAND_bytes failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }

    /* converting generated sequence in integer */
    print_hex_value(A_hex, INTEGER_SIZE, "A (hex)");
    print_hex_value(B_hex, INTEGER_SIZE, "B (hex)");
    unsigned int A_int = convert_bytes_to_int(A_hex, INTEGER_SIZE);
    unsigned int B_int = convert_bytes_to_int(B_hex, INTEGER_SIZE);
    fprintf(stdout, "Printing A (int): %d\n", A_int);
    fprintf(stdout, "Printing B (int): %d\n", B_int);

    /* computing result */
    unsigned long long int mod = ((unsigned long) 1 << 32);
    C_int = (unsigned long long) (A_int * B_int) % mod;
    fprintf(stdout, "Printing C (int): %lld\n", C_int);
    
    
    exit(EXIT_SUCCESS);
}

int convert_bytes_to_int(char *number, int nbytes) {
    unsigned int result = 0;
    for (int i = 0; i < nbytes; i++) {
        result += ((int) number[i]) << i*8;
    }
    return result;
}

void print_hex_value(char *number, int length, char *title) {
    fprintf(stdout, "Printing %s: ", title);
    for (int i = 0; i < length; i++) {
        fprintf(stdout, "%02x", number[i]);
    }
    fprintf(stdout, "\n");
}