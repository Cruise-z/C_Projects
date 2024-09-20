//
// Created by 赵睿智 on 2022/3/17.
//

#ifndef CRYPTO_EXPERIMENT_MY_MATRIX_CAL_H
#define CRYPTO_EXPERIMENT_MY_MATRIX_CAL_H

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct matrix_inf{
    long long int **matrix;
    int row;
    int column;
}inf, *inf_ptr;

typedef struct mpz_matrix_inf{
    mpz_t **matrix;
    int row;
    int column;
}mpz_inf, *mpz_inf_ptr;

void init_matrix(long long int ***matrix_ptr, int r, int c);
long long int laplace_expansion(long long int **matrix, int r, int c, int order);
long long int determinant(long long int **matrix, int order);
long long int determinant_mod(long long int **matrix, int order, int mod);
long long int **inv_matrix(long long int **matrix, int order, int mod);
long long int **matrix_mul(long long int **matrix1, int r1, int c1, long long int **matrix2, int r2, int c2);
long long int **matrix_mul_mod(long long int **matrix1, int r1, int c1, long long int **matrix2, int r2, int c2, int mod);
inf str2matrix(const char *str, int column);
void get_matrix(long long int ***matrix_ptr, int r, int c);
void output_matrix(long long int **matrix, int r, int c);

void mpz_init_matrix(mpz_t ***matrix_ptr, int r, int c);
void mpz_laplace_expansion(mpz_t **matrix, int r, int c, int order, mpz_t *cofactor); //r:row行数,c:column列数
void mpz_determinant(mpz_t **matrix, int order, mpz_t *result);
void mpz_determinant_mod(mpz_t **matrix, int order, int mod, mpz_t *result);
bool mpz_inv_matrix(mpz_t **matrix, int order, int mod, mpz_t ***inv_matrix_ptr);
bool mpz_matrix_mul(mpz_t **matrix1, int r1, int c1, mpz_t **matrix2, int r2, int c2, mpz_t ***re_matrix_ptr);
bool mpz_matrix_mul_mod(mpz_t **matrix1, int r1, int c1, mpz_t **matrix2, int r2, int c2, int mod, mpz_t ***re_matrix_ptr);
mpz_inf mpz_str2matrix(const char *str, int column);
void mpz_get_matrix(mpz_t ***matrix_ptr, int r, int c);
void mpz_output_matrix(mpz_t **matrix, int r, int c);

#endif //CRYPTO_EXPERIMENT_MY_MATRIX_CAL_H
