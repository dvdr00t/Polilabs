#include <stdio.h>
#include <unistd.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#define MAX_BUFFER 1024

/**
 *                                             Exercise02
 * ---------------------------------------------------------------------------------------------------
 * Write a program in C that, using the OpenSSL library, computes the hash of the concatenation of two 
 * files using SHA256 (or or SHA 512 or SHA3). 
 * The filenames are passed as first and second parameters from the command line.
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
    if (argc != 3) {
        fprintf(stderr, "[USAGE] ./exe02 <file_A> <file_B>\n");
        exit(EXIT_FAILURE);
    }

    /* initializing errors and algorithms */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();


    /* retrieving filename and file pointer */
    FILE *fp_A = fopen(argv[1], "r");
    FILE *fp_B = fopen(argv[2], "r");
    if (fp_A == NULL || fp_B == NULL) {
        fprintf(stderr, "[ERROR] fopen failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* context initialization */
    EVP_MD_CTX *md_context = EVP_MD_CTX_new();
    if (md_context == NULL) {
        handle_errors();
    }

    /* digest initialization */
    if (!EVP_DigestInit(md_context, EVP_sha256())) {
        handle_errors();
    }

    /* digest update */
    int n_read;
    unsigned char buffer[MAX_BUFFER];
    while ((n_read = fread(buffer, 1, MAX_BUFFER, fp_A)) > 0) {
        if (!EVP_DigestUpdate(md_context, buffer, n_read)) {
            handle_errors();
        }
    }

    while ((n_read = fread(buffer, 1, MAX_BUFFER, fp_B)) > 0) {
        if (!EVP_DigestUpdate(md_context, buffer, n_read)) {
            handle_errors();
        }
    }

    /* digest finalization */
    unsigned char hash_value[EVP_MD_size(EVP_sha256())];
    int hash_length;
    if (!EVP_DigestFinal_ex(md_context, hash_value, &hash_length)) {
        handle_errors();
    }
    EVP_MD_CTX_free(md_context);

    /* printing the result */
    fprintf(stdout, "The digest is: ");
    for (int i = 0; i < hash_length; i++) {
        fprintf(stdout, "%02x", hash_value[i]);
    }
    fprintf(stdout, "\n");

    /* Freeing some memory */
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    exit(EXIT_SUCCESS);
}