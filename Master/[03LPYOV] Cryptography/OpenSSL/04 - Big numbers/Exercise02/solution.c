#include <stdio.h>
#include <unistd.h>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#define NBYTES 32

/**
 *                                             Exercise01
 * ---------------------------------------------------------------------------------------------------
 * Using OpenSSL, write a C program that implements all the operations that are performed by both the 
 * parties of a DH key exchange (no need to exchange data, just perform the mathematical operations).
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

    /* intializing stuff */
    ERR_load_crypto_strings();
    if (RAND_load_file("/dev/random", 64) != 64) {
        handle_errors();
    }

    /* SHARED INFORMATIONS */
    BIGNUM *p = BN_new();       // large prime number
    BIGNUM *g = BN_new();       // generator (p > g > 1)

    /* generating p */
    BN_CTX *ctx = BN_CTX_new();
    if (!BN_generate_prime_ex(p, 1024, 0, NULL, NULL, NULL)) {
        handle_errors();
    } else if (!BN_is_prime_ex(p, 16, ctx, NULL)) {
        fprintf(stderr, "[ERROR] whaaat? not a prime number!\n");
        exit(EXIT_FAILURE);
    }
    
    /* generating g */
    if (!BN_rand_range(g, p)) {
        handle_errors();
    }

    /* ALICE */
    BIGNUM *a = BN_new();       // exponent A = g^a mod p
    BIGNUM *A = BN_new();       // half key value A

    /* generating a and computing A */
    if (!BN_rand(a, 512, 0, 1)) {
        handle_errors();
    } else if (!BN_mod_exp(A, g, a, p, ctx)) {
        handle_errors();
    }

    /* BOB */
    BIGNUM *b = BN_new();       // exponent B = g^b mod p
    BIGNUM *B = BN_new();       // half key value B

    /* generating b and computing B */
    if (!BN_rand(b, 512, 0, 1)) {
        handle_errors();
    } else if (!BN_mod_exp(B, g, b, p, ctx)) {
        handle_errors();
    }


    /* KEY GENERATION */
    BIGNUM *K1 = BN_new();
    BIGNUM *K2 = BN_new();
    if (!BN_mod_exp(K1, A, b, p, ctx) || !BN_mod_exp(K2, B, a, p, ctx)) {
        handle_errors();
    } 

    /* checking */
    if (!BN_cmp(K1, K2)) {
        fprintf(stdout, ">>> Key is: ");
        BN_print_fp(stdout, K1);
        fprintf(stdout, "\n");
    } else {
        fprintf(stderr, "[ERROR] key generation failed!");
        exit(EXIT_FAILURE);
    }


    /* cleaning up stuff */
    BN_free(p);
    BN_free(g); 
    BN_free(a);
    BN_free(b);
    BN_free(A);
    BN_free(B);
    BN_free(K1);
    BN_free(K2);
    BN_CTX_free(ctx);
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    exit(EXIT_SUCCESS);
}