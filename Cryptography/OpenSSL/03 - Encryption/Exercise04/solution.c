#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define ENCRYPT 1
#define DECRYPT 0
#define MAX_BUFFER 1024

#define ENC_ALGO "aes-192-cbc"

/**
 *                                             Exercise04
 * ---------------------------------------------------------------------------------------------------
 * Find a way in the OpenSSL documentation to encrypt, using a block cipher, a message passed as first 
 * parameter from the command line, without padding the last block (and manage the possible errors).
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

    /* checking input arguments */
    if (argc != 2) {
        fprintf(stderr, "[USAGE] ./exe04 <message>\n");
        exit(EXIT_FAILURE);
    }

    /* checking cipher algorithm */
    const EVP_CIPHER *cipher_algorithm = EVP_get_cipherbyname(ENC_ALGO);
    if (cipher_algorithm == NULL) {
        fprintf(stderr, "[USAGE] ENC_ALGO should be a valid algorithm in openssl\n");
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
    fprintf(stdout, "\n>>> Key generated for encryption is: ");
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
    EVP_CIPHER_CTX_set_padding(cipher_context, EVP_CIPH_NO_PADDING);

    /* cipher initialization */
    if (!EVP_CipherInit(cipher_context, cipher_algorithm, key, iv, ENCRYPT)) {
        handle_errors();
    }

    /* cipher update */
    unsigned char *ciphertext = (unsigned char *) malloc(100*MAX_BUFFER * sizeof(unsigned char));
    unsigned int ciphertext_length = 0;
    unsigned int update_length;
    if (!EVP_CipherUpdate(cipher_context, ciphertext, &update_length, argv[1], strlen(argv[1]))) {
        handle_errors();
    }
    ciphertext_length += update_length;

    /* cipher finalization */
    if (!EVP_CipherFinal(cipher_context, ciphertext+ciphertext_length, &update_length)) {
        handle_errors();
    }
    ciphertext_length += update_length;

    /* displaying result */
    fprintf(stderr, ">>> Ciphertext lenght: %d\n", ciphertext_length);
    fprintf(stderr, ">>> Ciphertext: ");
    for(int i = 0; i < ciphertext_length; i++)
        printf("%02x", ciphertext[i]);
    printf("\n");

    /* some cleaning up */
    EVP_CIPHER_CTX_free(cipher_context);
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    exit(EXIT_SUCCESS);
}