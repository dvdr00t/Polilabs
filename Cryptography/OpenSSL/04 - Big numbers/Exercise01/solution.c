#include <stdio.h>
#include <unistd.h>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#define NBYTES 32

/**
 *                                             Exercise01
 * ---------------------------------------------------------------------------------------------------
 * Write a program that, using OpenSSL, generates three random strings of 32 bytes each, convert them 
 * into Big Numbers bn1,bn2,bn3, then computes:
 *      - sum (bn1+bn2)
 *      - difference (bn1-bn3)
 *      - multiplication (bn1*bn2*bn3)
 *      - integer division (bn3/bn1)
 *      - modulus (bn1 mod bn2)
 *      - modulus-exponentiation (bn1^bn3 mod bn2)
*/

/**
 * @brief Handle errors generated with the openssl functions
 * 
 */
void handle_errors() {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
}


int main (int argc, char *argv[]) {

    ERR_load_crypto_strings();

    unsigned char stringA[NBYTES];
    unsigned char stringB[NBYTES];
    unsigned char stringC[NBYTES];

    /* initializing seed */
    if (RAND_load_file("/dev/random", 64) != 64) {
        handle_errors();
    }

    /* generating random strings of 32 bytes (256 bits) */
    if (RAND_bytes(stringA, NBYTES) != 1 || RAND_bytes(stringB, NBYTES) != 1 || RAND_bytes(stringC, NBYTES) != 1) {
        handle_errors();
    }

    /* converting string into bignumbers */
    BIGNUM *bn1 = BN_new();
    BIGNUM *bn2 = BN_new();
    BIGNUM *bn3 = BN_new();
    BIGNUM *res = BN_new();
    if (!BN_bin2bn(stringA, NBYTES, bn1) || !BN_bin2bn(stringB, NBYTES, bn2) || !BN_bin2bn(stringC, NBYTES, bn3)) {
        handle_errors();
    }

    printf(">>> bn1: ");
    BN_print_fp(stdout, bn1);
    printf("\n");
    printf(">>> bn2: ");
    BN_print_fp(stdout, bn2);
    printf("\n");
    printf(">>> bn3: ");
    BN_print_fp(stdout, bn3);
    printf("\n");

    /* computing operations */

    /* res = bn1 + bn2 */
    if (!BN_add(res, bn1, bn2)) {
        handle_errors();
    }
    printf(">>> bn1 + bn2: ");
    BN_print_fp(stdout, res);
    printf("\n");

    /* res = bn1 - bn3 */
    if (!BN_sub(res, bn1, bn3)) {
        handle_errors();
    }
    printf(">>> bn1 - bn3: ");
    BN_print_fp(stdout, res);
    printf("\n");

    /* res = bn1 * bn2 * bn3 */
    BN_CTX *ctx = BN_CTX_new();
    if (!BN_mul(res, bn1, bn2, ctx)) {
        handle_errors();
    }
    if (!BN_mul(res, res, bn3, ctx)) {
        handle_errors();
    }
    printf(">>> bn1 * bn2 * bn3: ");
    BN_print_fp(stdout, res);
    printf("\n");
    
    /* res = bn3/bn1 */
    BIGNUM *rem = BN_new();
    if (!BN_div(res, rem, bn3, bn1, ctx)) {
        handle_errors();
    }
    printf(">>> bn3/bn1: ");
    BN_print_fp(stdout, res);
    printf("\n");

    /* res = bn1 mod bn2 */
    if (!BN_mod(res, bn1, bn2, ctx)) {
        handle_errors();
    }
    printf(">>> bn1 mod bn2: ");
    BN_print_fp(stdout, res);
    printf("\n");

    /* res = bn1 ^ bn3 (mod bn2) */
    if (!BN_mod_exp(res, bn1, bn3, bn2, ctx)) {
        handle_errors();
    }
    printf(">>> bn1 ^ bn3 (mod bn2): ");
    BN_print_fp(stdout, res);
    printf("\n");


    /* cleaning up stuff */
    BN_free(bn1);
    BN_free(bn2);
    BN_free(bn3);
    BN_free(res);
    BN_CTX_free(ctx);

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    exit(EXIT_SUCCESS);
}