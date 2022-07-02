#include <stdio.h>
#include <unistd.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#define MAX_BUFFER 1024

/**
 *                                             Exercise03
 * ---------------------------------------------------------------------------------------------------
 * Writes a program in C that, using the OpenSSL library, computes the hash of the content of a file, 
 * passed as first parameter from the command line, using the algorithm passed as second parameter. 
 * HINT: check the EVP_get_digestbyname function (or the EVP_MD_fetch if you are 
 *       using OpenSSL 3.0+ https://www.openssl.org/docs/man3.0/man7/crypto.html).
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
        fprintf(stderr, "[USAGE] ./exe03 <file> <digest_algorithm>\n");
        exit(EXIT_FAILURE);
    }

    /* initializing errors and algorithms */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();


    /* retrieving filename and file pointer */
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] fopen failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* retrieving digest algorithm */
    const EVP_MD *digest_algorithm = EVP_get_digestbyname(argv[2]);
    if (digest_algorithm == NULL) {
        handle_errors();
    }

    /* context initialization */
    EVP_MD_CTX *md_context = EVP_MD_CTX_new();
    if (md_context == NULL) {
        handle_errors();
    }

    /* digest initialization */
    if (!EVP_DigestInit(md_context, digest_algorithm)) {
        handle_errors();
    }

    /* digest update */
    int n_read;
    unsigned char buffer[MAX_BUFFER];
    while ((n_read = fread(buffer, 1, MAX_BUFFER, fp)) > 0) {
        if (!EVP_DigestUpdate(md_context, buffer, n_read)) {
            handle_errors();
        }
    }

    /* digest finalization */
    unsigned char hash_value[EVP_MD_size(digest_algorithm)];
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