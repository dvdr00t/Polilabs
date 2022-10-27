#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define ENCRYPT 1
#define DECRYPT 0
#define MAX_BUFFER 1024

#define ENC_ALGO "chacha20"

/**
 *                                             Exercise03
 * ---------------------------------------------------------------------------------------------------
 * Implement a program that encrypts a file, passed as the first parameter from the command line, using 
 * a stream cipher. Using the C XOR function, apply a mask 128bit mask (of your choice, or just 
 * select '11..1'), decrypt and check the result.
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
        fprintf(stderr, "[USAGE] ./exe03 <file>\n");
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

    /* retrieve the file passed as argument */
    FILE *fp_in = fopen(argv[1], "r");
    if (fp_in == NULL) {
        fprintf(stderr, "[ERROR] fopen failed execution\n");
        exit(EXIT_FAILURE);
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

    /* diplaying information */
    fprintf(stdout, "\n>>> Ciphertext before mask: ");
    for (int i = 0; i< ciphertext_length; i++) {
        fprintf(stdout, "%02x", ciphertext[i]);
    }
    fprintf(stdout, "\n");

    /* applying mask */
    unsigned char *mask = "deadbeef";
    unsigned int mask_length = 8;
    for (int i = 0; i < ciphertext_length; i++) {
        ciphertext[i] = ciphertext[i] ^ mask[i % mask_length];
    }

    /* diplaying information */
    fprintf(stdout, "\n>>> Ciphertext after mask: ");
    for (int i = 0; i< ciphertext_length; i++) {
        fprintf(stdout, "%02x", ciphertext[i]);
    }
    fprintf(stdout, "\n");



    /* NOW START DECRYPT */

    /* context initalization */
    EVP_CIPHER_CTX *cipher_context_dec = EVP_CIPHER_CTX_new();
    if (cipher_context_dec == NULL) {
        handle_errors();
    }

    /* cipher initialization */
    if (!EVP_CipherInit(cipher_context_dec, cipher_algorithm, key, iv, DECRYPT)) {
        handle_errors();
    }

    /* cipher update */
    unsigned char *plaintext = (unsigned char *) malloc(100*MAX_BUFFER * sizeof(unsigned char));
    if (plaintext == NULL) {
        fprintf(stderr, "[ERROR] malloc failed execution\n");
        exit(EXIT_FAILURE);
    }
    unsigned int plaintext_length = 0;
    if (!EVP_CipherUpdate(cipher_context_dec, plaintext, &update_length, ciphertext, ciphertext_length)) {
        handle_errors();
    }
    plaintext_length += update_length;

    /* cipher finalization */
    if(!EVP_CipherFinal(cipher_context_dec, plaintext+plaintext_length, &final_length))
        handle_errors();
    
    plaintext_length += final_length;

    /* applying mask again */
    for (int i = 0; i < plaintext_length; i++) {
        plaintext[i] = plaintext[i] ^ mask[i % mask_length];
    }

    /* diplaying information */
    fprintf(stdout, "\n>>> Plaintext after mask: ");
    for (int i = 0; i< plaintext_length; i++) {
        fprintf(stdout, "%c", plaintext[i]);
    }
    fprintf(stdout, "\n");
    
    /* some cleaning up */
    fclose(fp_in);
    EVP_CIPHER_CTX_free(cipher_context);
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    exit(EXIT_SUCCESS);
}