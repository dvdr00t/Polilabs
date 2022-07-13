#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>

#include <string.h>

#define KEY_LENGTH 2048
#define MAX_BUFFER 1024


/**
 * @brief Handle errors generated with the openssl functions
 * 
 */
void handle_errors() {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

    /* Load the human readable error strings for libcrypto */
    ERR_load_crypto_strings();
    /* Load all digest and cipher algorithms */
    OpenSSL_add_all_algorithms();    


    /* checking user input */
    if (argc != 2) {
        fprintf(stderr, "[USAGE] ./exe01 <file>\n");
        exit(EXIT_FAILURE);
    }

    /**
     * -------------------------------------------------------
     *              1. Generating RSA Key Pair
     * -------------------------------------------------------
     */
    RSA *RSA_KeyPair = RSA_new();
    BIGNUM *BN_Exponent = BN_new();

    /* Generating RSA exponent */
    if (!BN_set_word(BN_Exponent, RSA_F4)) {  // RSA_F4 is a long integer with value 65537 (i.e. 0x10001L)
        handle_errors();
    }

    /* generating key pair */
    if (!RSA_generate_key_ex(RSA_KeyPair, KEY_LENGTH, BN_Exponent, NULL)) {
        handle_errors();
    }
    fprintf(stdout, "> Generated key pair\n");

    /* retrieving public and private keys */
    FILE *fp_public = fopen("public.pem", "w");
    FILE *fp_private = fopen("private.pem", "w");
    if (fp_public == NULL || fp_private == NULL) {
        fprintf(stderr, "[ERROR] fopen() failed execution\n");
        exit(EXIT_FAILURE);
    }
    if (!PEM_write_RSA_PUBKEY(fp_public, RSA_KeyPair) || !PEM_write_RSAPrivateKey(fp_private, RSA_KeyPair, NULL, NULL, 0, NULL, NULL)) {
        handle_errors();
    }
    fclose(fp_private);
    fclose(fp_public);
    fprintf(stdout, "> Retrieved public and private keys\n");
    


    /**
     * -------------------------------------------------------
     *              2. Signature of the file 
     * -------------------------------------------------------
     */
    FILE *fp_input = fopen(argv[1], "r");
    fp_private = fopen("private.pem", "r");
    if (fp_input == NULL || fp_private == NULL) {
        fprintf(stderr, "[ERROR] fopen() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* storing private key in a variable */
    EVP_PKEY *KEY_private = PEM_read_PrivateKey(fp_private, NULL, NULL, NULL);
    if (KEY_private == NULL) {
        handle_errors();
    }

    /* creating context and init digest */
    EVP_MD_CTX *SIGN_ctx = EVP_MD_CTX_new();
    if (!EVP_DigestSignInit(SIGN_ctx, NULL, EVP_sha256(), NULL, KEY_private)) {
        handle_errors();
    }

    /* updating digest */
    unsigned char buffer[MAX_BUFFER];
    size_t n_read;
    while ((n_read = fread(buffer, 1, MAX_BUFFER, fp_input)) > 0) {
        if (!EVP_DigestSignUpdate(SIGN_ctx, buffer, n_read)) {
            handle_errors();
        }
    }

    /* retreiving signature */
    char *signature = (char *) malloc(EVP_PKEY_size(KEY_private) * sizeof(char));
    if (signature == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed executio\n");
        exit(EXIT_FAILURE);
    }
    size_t sign_length;
    if (!EVP_DigestSignFinal(SIGN_ctx, NULL, &sign_length)) {
        handle_errors();
    }   
    if (!EVP_DigestSignFinal(SIGN_ctx, signature, &sign_length)) {
        handle_errors();
    }
    EVP_MD_CTX_free(SIGN_ctx);
    fclose(fp_input);
    fclose(fp_private);

    /* saving signature to a file */
    FILE *fp_output = fopen("signature.bin", "w");
    if (fp_output == NULL) {
        fprintf(stderr, "[ERROR] fopen() failed execution\n");
        exit(EXIT_FAILURE);
    }
    if (fwrite(signature, 1, sign_length, fp_output) < sign_length) {
        handle_errors();
    }
    fclose(fp_output);   
    fprintf(stdout, "> File signed\n");


    /**
     * -------------------------------------------------------
     *              3. Verification of the signature
     * -------------------------------------------------------
     * 
     * Suppose to have in input:
     *      - the signed file (file.txt)
     *      - the signature (signature.bin)
     *      - the public key (public.pem)
     */
    
    /* opening file */
    FILE *fp_file = fopen(argv[1], "r");
    FILE *fp_sign = fopen("signature.bin", "r");
    FILE *fp_keyp = fopen("public.pem", "r");
    if (fp_file == NULL || fp_sign == NULL || fp_keyp == NULL) {
        fprintf(stderr, "[ERROR] fopen() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* storing public key to variable */
    EVP_PKEY *KEY_public = PEM_read_PUBKEY(fp_keyp, NULL, NULL, NULL);
    if (KEY_public == NULL) {
        handle_errors();
    }
    fclose(fp_keyp);

    /* creating context and init verification */
    EVP_MD_CTX *VERF_ctx = EVP_MD_CTX_new();
    if (!EVP_DigestVerifyInit(VERF_ctx, NULL, EVP_sha256(), NULL, KEY_public)) {
        handle_errors();
    }

    /* update verification */
    unsigned char verf_buffer[MAX_BUFFER];
    while ((n_read = fread(verf_buffer, 1, MAX_BUFFER, fp_file)) > 0) {
        if (!EVP_DigestVerifyUpdate(VERF_ctx, verf_buffer, n_read)) {
            handle_errors();
        }
    }

    /* retreiving verification signature */
    char *verf_signature = (char *) malloc(EVP_PKEY_size(KEY_private) * sizeof(char));
    if (verf_signature == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed executio\n");
        exit(EXIT_FAILURE);
    }
    size_t verf_length = 0;
    while((n_read = fread(verf_signature, 1, MAX_BUFFER, fp_sign)) > 0){
        verf_length += n_read;
    }
    
    fclose(fp_file);

    /* verification */
    if (EVP_VerifyFinal(VERF_ctx, verf_signature, verf_length, KEY_public)) {
        fprintf(stdout, ">>> SIGNATURE OK\n");
    } else {
        fprintf(stdout, ">>> SIGNATURE FAILED\n");
    }


    /* free data used */
    EVP_MD_CTX_free(VERF_ctx);
    RSA_free(RSA_KeyPair);
    BN_free(BN_Exponent);

    // completely free all the cipher data
    CRYPTO_cleanup_all_ex_data();
    /* Remove error strings */
    ERR_free_strings();

    /* exit on success */
    exit(EXIT_SUCCESS);
}