#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define ENCRYPT 1
#define DECRYPT 0
#define MAX_BUFFER 1024


/**
 *                                             Exercise01
 * ---------------------------------------------------------------------------------------------------
 * Write a program in C that, using the OpenSSL library, encrypts the content of a file using a 
 * user-selected algorithm. 
 * The filename is passed as first parameter from the command line, the algorithm is passed as second 
 * parameter and must be an OpenSSL-compliant string (e.g., aes-128-cbc or aes-256-ecb).
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

    /* initializing some stuff */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    if (RAND_load_file("/dev/random", 64) != 64) {
        handle_errors();
    }

    /* checking input arguments */
    if (argc != 3) {
        fprintf(stderr, "[USAGE] ./exe01 <file> <algorithm>\n");
        exit(EXIT_FAILURE);
    }

    /* checking cipher algorithm passed as input */
    const EVP_CIPHER *cipher_algorithm = EVP_get_cipherbyname(argv[2]);
    if (cipher_algorithm == NULL) {
        fprintf(stderr, "[USAGE] <algorithm> should be a valid algorithm in openssl\n");
        exit(EXIT_FAILURE);
    }

    /* retrieve the file passed as argument */
    FILE *fp_in = fopen(argv[1], "r");
    if (fp_in == NULL) {
        fprintf(stderr, "[ERROR] fopen failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* create the output file for encryption */
    FILE *fp_out = fopen("exe01.enc", "w");
    if (fp_out == NULL) {
        fprintf(stderr, "[ERROR] fopen failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* generating a random key based on the algorithm passed as argument */
    unsigned char *key;
    unsigned int key_length;
    if (!(key_length = EVP_CIPHER_key_length(cipher_algorithm))) {
        handle_errors();
    }
    key = (unsigned char *) malloc(key_length * sizeof(unsigned char));
    if (key == NULL) {
        fprintf(stderr, "[ERROR] malloc failed execution\n");
        exit(EXIT_FAILURE);
    }
    if (!RAND_bytes(key, key_length)) {
        handle_errors();
    }
    fprintf(stdout, ">>> Key generated for encryption is: ");
    for (int i = 0; i < key_length; i++) {
        fprintf(stdout, "%02x", key[i]);
    }
    fprintf(stdout, " [length: %d bytes]\n", key_length);

    /* generating a random initialization vector based on the algorithm passed as argument */
    unsigned char *iv;
    unsigned int iv_length;
    if (!(iv_length = EVP_CIPHER_iv_length(cipher_algorithm))) {
        iv = 0;
    } else {
        iv = (unsigned char *) malloc(iv_length * sizeof(unsigned char));
        if (iv == NULL) {
            fprintf(stderr, "[ERROR] malloc failed execution\n");
            exit(EXIT_FAILURE);
        }
        if (!RAND_bytes(iv, iv_length)) {
            handle_errors();
        }
        fprintf(stdout, ">>> IV generated for encryption is:  ");
        for (int i = 0; i < iv_length; i++) {
            fprintf(stdout, "%02x", iv[i]);
        }
        fprintf(stdout, " [length: %d bytes]\n", iv_length);
    }


    /* context initalization */
    EVP_CIPHER_CTX *cipher_context = EVP_CIPHER_CTX_new();
    if (cipher_context == NULL) {
        handle_errors();
    }

    /* cipher initialization */
    if (!EVP_CipherInit(cipher_context, cipher_algorithm, key, iv, ENCRYPT)) {
        handle_errors();
    }

    /* cipher update (reading from file) */
    int n_read;
    char buffer[MAX_BUFFER];
    unsigned char *ciphertext = (unsigned char *) malloc(100*MAX_BUFFER * sizeof(unsigned char));
    if (ciphertext == NULL) {
        fprintf(stderr, "[ERROR] malloc failed execution\n");
        exit(EXIT_FAILURE);
    }
    unsigned int ciphertext_length = 0;
    unsigned int update_length;
    unsigned int final_length;
    while ((n_read = fread(buffer, 1, MAX_BUFFER, fp_in)) > 0) {
        if (!EVP_CipherUpdate(cipher_context, ciphertext, &update_length, buffer, n_read)) {
            handle_errors();
        }

        ciphertext_length += update_length;
    }

    /* cipher finalization */
    if (!EVP_CipherFinal(cipher_context, ciphertext+ciphertext_length, &final_length)) {
        handle_errors();
    }
    ciphertext_length += final_length;

    /* writing back result to file */
    for (int i = 0; i < ciphertext_length; i++) {
        fprintf(fp_out, "%c", ciphertext[i]);
    }
    
    /* some cleaning up */
    fclose(fp_in);
    fclose(fp_out);
    EVP_CIPHER_CTX_free(cipher_context);
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    exit(EXIT_SUCCESS);
}