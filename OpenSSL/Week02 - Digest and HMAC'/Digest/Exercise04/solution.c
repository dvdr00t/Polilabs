#include <stdio.h>
#include <unistd.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#define MAX_BUFFER 1024

/**
 *                                             Exercise04
 * ---------------------------------------------------------------------------------------------------
 * Using OpenSSL, compute the digest of the file passed as first parameter from the command line with 
 * both SHA256 and SHA512.
 * Then, name sha512_low and sha512_high the first 256 bits and the last 256 of the SHA512 digest, 
 * respectively, and sha256 the digest computed with SHA356, you have to compute print on the standard 
 * output the following operation:
 *      
 *                                sha256 XOR (sha512_low AND SHA512_high)
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
        fprintf(stderr, "[USAGE] ./exe04 <file>n");
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

    /* context initialization */
    EVP_MD_CTX *md_context_sha256 = EVP_MD_CTX_new();
    EVP_MD_CTX *md_context_sha512 = EVP_MD_CTX_new();
    if (md_context_sha256 == NULL || md_context_sha512 == NULL) {
        handle_errors();
    }

    /* digest initialization */
    if (!EVP_DigestInit(md_context_sha256, EVP_sha256()) || !EVP_DigestInit(md_context_sha512, EVP_sha512())) {
        handle_errors();
    }

    /* digest update */
    int n_read;
    unsigned char buffer[MAX_BUFFER];
    while ((n_read = fread(buffer, 1, MAX_BUFFER, fp)) > 0) {
        if (!EVP_DigestUpdate(md_context_sha256, buffer, n_read) || !EVP_DigestUpdate(md_context_sha512, buffer, n_read)) {
            handle_errors();
        }
    }

    /* digest finalization */
    unsigned char hash_value_sha256[EVP_MD_size(EVP_sha256())];
    unsigned char hash_value_sha512[EVP_MD_size(EVP_sha512())];
    int hash_length_sha256;
    int hash_length_sha512;
    if (!EVP_DigestFinal_ex(md_context_sha256, hash_value_sha256, &hash_length_sha256) || 
        !EVP_DigestFinal_ex(md_context_sha512, hash_value_sha512, &hash_length_sha512)) {
            handle_errors();
    }
    EVP_MD_CTX_free(md_context_sha256);
    EVP_MD_CTX_free(md_context_sha512);

    /* printing the result */
    fprintf(stdout, "The digest is: ");
    for (int i = 0; i < hash_length_sha256; i++) {
        fprintf(stdout, "%02x", hash_value_sha256[i] ^ (hash_value_sha512[i] & hash_value_sha512[i+hash_length_sha256]));
    }
    fprintf(stdout, "\n");

    /* Freeing some memory */
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    exit(EXIT_SUCCESS);
}