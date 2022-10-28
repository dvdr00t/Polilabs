#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#define INTEGER_SIZE 32                      /* 32 bytes = 256 bits */
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


/**
 * EXERCISE 04
 * Using OpenSSL, generate two 256 bit integers, sum them (modulo 2^256) and print the result.
 * -------------------------------------------------------------------------------------------
*/

void compute_sum(char *intA, char *intB, char *intC, int nbytes);
void print_bin_value(char *number, int length, char *title);
void print_hex_value(char *number, int length, char *title);
int convert_bytes_to_int(char *number, int nbytes);
int main (int args, char *argv[]) {

    fprintf(stdout, "------ WEEK01 - PRGNGs with OpenSLL ------\n");
    fprintf(stdout, "               [ exe04 ]                  \n");
    fprintf(stdout, "                                          \n");

    /* initializing seed */
    if (RAND_load_file("/dev/random", 64) != 64) {
        fprintf(stderr, "[ERROR] function RAND_load_file failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }

    unsigned char intA[INTEGER_SIZE];       /* 256 bits integer */
    unsigned char intB[INTEGER_SIZE];       /* 256 bits integer */
    unsigned char intC[INTEGER_SIZE];       /* result of the operation */

    /* generate random numbers (just as for strings) */
    if (RAND_bytes(intA, INTEGER_SIZE) != 1 || RAND_bytes(intB, INTEGER_SIZE) != 1) {
        fprintf(stderr, "[ERROR] function RAND_bytes failed execution.");
        ERR_print_errors((BIO *)stderr);
        exit(EXIT_FAILURE);
    }

    print_hex_value(intA, INTEGER_SIZE, "A (hex)");
    print_hex_value(intB, INTEGER_SIZE, "B (hex)");
    // print_bin_value(intA, INTEGER_SIZE, "A (bin)");
    // print_bin_value(intB, INTEGER_SIZE, "B (bin)");

    /* compute the sum byte-a-byte modulo 2^256 */
    compute_sum(intA, intB, intC, INTEGER_SIZE);
    
    exit(EXIT_SUCCESS);
}

int convert_bytes_to_int(char *number, int nbytes) {
    unsigned int result = 0;
    for (int i = 0; i < nbytes; i++) {
        result += ((int) number[i]) << i*8;
    }
    return result;
}

void print_bin_value(char *number, int length, char *title) {
    fprintf(stdout, "Printing %s: ", title);
    for (int i = 0; i < length; i++) {        
        fprintf(stdout, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(number[i]));
    }
    fprintf(stdout, "\n");
}

void print_hex_value(char *number, int length, char *title) {
    fprintf(stdout, "Printing %s: ", title);
    for (int i = 0; i < length; i++) {
        fprintf(stdout, "%02x", number[i]);
    }
    fprintf(stdout, "\n");
}

void compute_sum(char *intA, char *intB, char *intC, int nbytes) {

    unsigned int n1 = 0;
    unsigned int n2 = 0;
    unsigned int overflow = 0; 

    int sum = 0;

    for (int byte = nbytes-1; byte >= 0; byte--) {
        n1 = convert_bytes_to_int((char *) &intA[byte], 1);
        n2 = convert_bytes_to_int((char *) &intB[byte], 1);

        sum = overflow ? (n1 + n2 + 1) : (n1 + n2);
        if (sum > 255) {
            intC[byte] = sum - 255 - 1;
            overflow = 1;
        } else {
            intC[byte] = sum;
            overflow = 0;
        }
    }

    print_hex_value(intC, nbytes, "C (hex)");
}