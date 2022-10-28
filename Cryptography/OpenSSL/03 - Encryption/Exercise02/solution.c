#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define ENCRYPT 1
#define DECRYPT 0
#define MAX_BUFFER 1024

#define ENC_ALGO "aes-128-cbc"

/**
 *                                             Exercise02
 * ---------------------------------------------------------------------------------------------------
 * Write a program that decrypts the content of a file, passed as the first parameter from the command 
 * line, using the key and IV passed as the second and third parameters. The program must save the 
 * decrypted file into a file whose name is the fourth parameter (i.e., decrypt the result of the 
 * encrpytion of enc4.c on GitHub).
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
    if (argc != 5) {
        fprintf(stderr, "[USAGE] ./exe02 <file.enc> <key> <iv> <file.plain>\n");
        exit(EXIT_FAILURE);
    }

    /* checking cipher algorithm */
    const EVP_CIPHER *cipher_algorithm = EVP_get_cipherbyname(ENC_ALGO);
    if (cipher_algorithm == NULL) {
        fprintf(stderr, "[USAGE] ENC_ALGO should be a valid algorithm in openssl\n");
        exit(EXIT_FAILURE);
    }

    /* checking key */
    if (strlen(argv[2]) != 32){
        fprintf(stderr, "[USAGE] <key> should be 32 bytes\n");
        exit(EXIT_FAILURE);
    }   
    unsigned char key[strlen(argv[2])/2];
    for (int i = 0; i < strlen(argv[2])/2; i++) {
        sscanf(&argv[2][2*i], "%2hhx", &key[i]);
    }

    /* checking iv */
    if (strlen(argv[3]) != 32){
        fprintf(stderr, "[USAGE] <iv> should be 32 bytes\n");
        exit(EXIT_FAILURE);
    }
    unsigned char iv[strlen(argv[3])/2];
    for (int i = 0; i < strlen(argv[3])/2; i++) {
        sscanf(&argv[3][2*i], "%2hhx", &iv[i]);
    }

    /* retrieve the file passed as argument */
    FILE *fp_in = fopen(argv[1], "r");
    if (fp_in == NULL) {
        fprintf(stderr, "[ERROR] fopen failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* create the output file for encryption */
    FILE *fp_out = fopen(argv[4], "w");
    if (fp_out == NULL) {
        fprintf(stderr, "[ERROR] fopen failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* context initalization */
    EVP_CIPHER_CTX *cipher_context = EVP_CIPHER_CTX_new();
    if (cipher_context == NULL) {
        handle_errors();
    }

    /* cipher initialization */
    if (!EVP_CipherInit(cipher_context, cipher_algorithm, key, iv, DECRYPT)) {
        handle_errors();
    }

    /* cipher update (reading from file) */
    int n_read;
    char buffer[MAX_BUFFER];
    unsigned char *plaintext = (unsigned char *) malloc(100*MAX_BUFFER * sizeof(unsigned char));
    if (plaintext == NULL) {
        fprintf(stderr, "[ERROR] malloc failed execution\n");
        exit(EXIT_FAILURE);
    }
    unsigned int plaintext_length = 0;
    unsigned int update_length;
    unsigned int final_length;
    while ((n_read = fread(buffer, 1, MAX_BUFFER, fp_in)) > 0) {
        if (!EVP_CipherUpdate(cipher_context, plaintext, &update_length, buffer, n_read)) {
            handle_errors();
        }

        if (fwrite(plaintext, 1, update_length, fp_out) < update_length) {
            fprintf(stderr, "[ERROR] fwrite failed execution\n");
            exit(EXIT_FAILURE);
        }
    }

    /* cipher finalization */
    if (!EVP_CipherFinal(cipher_context, plaintext, &final_length)) {
        handle_errors();
    }
    if (fwrite(plaintext, 1, final_length, fp_out) < final_length) {
        fprintf(stderr, "[ERROR] fwrite failed execution\n");
        exit(EXIT_FAILURE);
    }
    
    /* some cleaning up */
    fclose(fp_in);
    fclose(fp_out);
    EVP_CIPHER_CTX_free(cipher_context);
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    exit(EXIT_SUCCESS);
}