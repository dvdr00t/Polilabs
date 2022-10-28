#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#define MAX_BUFFER 1024

/**
 *                                             Exercise01
 * ---------------------------------------------------------------------------------------------------
 * Write a program in C that, using the OpenSSL library, computes the HMAC of the content of a file 
 * using SHA256 (or or SHA 512 or SHA3).
 * The filename is passed as first parameter from the command line.
*/

/**
 * @brief Handle errors generated with the openssl functions
 * 
 */
void handle_errors() {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

    /* checking input arguments */
    if (argc != 2) {
        fprintf(stderr, "[USAGE] ./exe01 <file>\n");
        exit(EXIT_FAILURE);
    }

    /* initializing errors and algorithms */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_digests();

    /* KEY */
    unsigned char key[] = "deadbeefdeadbeef";

    /* retrieving filename and file pointer */
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] fopen failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* context initialization */
    HMAC_CTX *hmac_context = HMAC_CTX_new();
    if (hmac_context == NULL) {
        handle_errors();
    }

    /* digest initialization */
    if (!HMAC_Init_ex(hmac_context, key, strlen(key), EVP_sha256(), NULL)) {
        handle_errors();
    }

    /* digest update */
    int n_read;
    unsigned char buffer[MAX_BUFFER];
    while ((n_read = fread(buffer, 1, MAX_BUFFER, fp)) > 0) {
        if (!HMAC_Update(hmac_context, buffer, n_read)) {
            handle_errors();
        }
    }

    /* digest finalization */
    unsigned char hmac_value[HMAC_size(hmac_context)];
    int hmac_length;
    if (!HMAC_Final(hmac_context, hmac_value, &hmac_length)) {
        handle_errors();
    }
    HMAC_CTX_free(hmac_context);

    /* printing the result */
    fprintf(stdout, "The hmac is: ");
    for (int i = 0; i < hmac_length; i++) {
        fprintf(stdout, "%02x", hmac_value[i]);
    }
    fprintf(stdout, "\n");

    /* Freeing some memory */
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    exit(EXIT_SUCCESS);
}