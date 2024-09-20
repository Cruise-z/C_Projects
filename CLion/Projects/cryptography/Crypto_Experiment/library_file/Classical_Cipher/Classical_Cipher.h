//
// Created by 赵睿智 on 2022/3/17.
//

#ifndef CRYPTO_EXPERIMENT_CLASSICAL_CIPHER_H
#define CRYPTO_EXPERIMENT_CLASSICAL_CIPHER_H

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../My_Matrix_cal/My_Matrix_cal.h"
#include "../My_BigNum_GF_cal/My_BigNum_GF_cal.h"

typedef struct Mono_sub_key_inf{
    char m;
    char c;
}mono_sub_key, *mono_sub_ptr;

typedef struct St_Ring{
    char c;
    struct St_Ring *next;
}ring, *ring_ptr;

typedef struct Matrix_cipher_key_inf{
    int order;
    int column;
}mc_key_inf, *mc_key_inf_ptr;

typedef struct Letter_frequency_inf{
    char letter;
    long long int freq;
}letter_freq, *letter_freq_ptr;

char *Hill(const char *s, long long int **key, int order, char mode);
long long int **Attack_Hill(const char *M, const char *C, int order);
char *mpz_Hill(const char *s, mpz_t **key, int order, char mode);

bool Affine_check_key(long long int k, int word_range);
char *Affine(const char *m, long long int k, long long int b, int word_range, char mode);
bool mpz_Affine_check_key(mpz_t k, int word_range);
char *mpz_Affine(const char *m, mpz_t k, mpz_t b, int word_range, char mode);

int qsort_encode_cmp(const void *a, const void *b);
int qsort_decode_cmp(const void *a, const void *b);
char *Mono_substitution(mono_sub_ptr key_ptr, char *str, int word_range, char mode);

ring_ptr struct_ring(char *str);
char *Vigenere(char *key, char *str, int word_range, char mode);

char *Vernam(char *key, char *str, char mode);

char *Fence(char *str, int fence_len, char mode);

mc_key_inf_ptr matrix_cipher_key_init(char *key_str, int num);
int matrix_cipher_key_cmp(const void *a, const void *b);
mc_key_inf_ptr matrix_cipher_key_sort(mc_key_inf_ptr key, int num);
char *Matrix_cipher(char *s, mc_key_inf_ptr key_ptr, int key_len, char mode);

void init_letter_freq_form(letter_freq_ptr *form_ptr);
int qsort_letter_freq_cmp(const void *a, const void *b);
letter_freq_ptr Letter_freq_match(char *s);

#endif //CRYPTO_EXPERIMENT_CLASSICAL_CIPHER_H
