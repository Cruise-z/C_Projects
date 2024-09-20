//
// Created by 赵睿智 on 2022/3/17.
//

#include "My_Matrix_cal.h"
#include "../My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define LEN 2048

void init_matrix(long long int ***matrix_ptr, int r, int c){
    int i, j;
    *matrix_ptr = (long long int **)malloc(sizeof(long long int *)*r);
    for(i = 0; i < r; i++)
        (*matrix_ptr)[i] = malloc(sizeof(long long int)*c);
    for(i = 0; i < r; i++)
        for(j = 0; j < c; j++)
            (*matrix_ptr)[i][j] = 0;
}

long long int laplace_expansion(long long int **matrix,int r,int c,int order)
{
    long long int result = 0,original_i,original_j,i,j;
    long long int **cofactor = NULL;
    init_matrix(&cofactor, order-1, order-1);
    for(i = 0;i < order;i ++)
        for(j = 0;j < order;j ++){
            original_i = i;
            original_j = j;
            if(i == r || j == c);
            else{
                if(i > r)
                    i --;
                if(j > c)
                    j --;
                cofactor[i][j] = matrix[original_i][original_j];
                i = original_i;
                j = original_j;
            }
        }
    if(order >= 2)
        result = determinant(cofactor,order - 1);
    free(cofactor);
    return result;
}

long long int determinant(long long int **matrix, int order)
{
    long long int result = 0, cofactor;
    int i, sign = 1;
    if(order == 1)
        result = matrix[0][0];
    else
        for(i = 0;i < order;i ++){
            cofactor = laplace_expansion(matrix,i,0,order);
            result += sign * matrix[i][0] * cofactor;
            sign *= -1;
        }
    return result;
}

long long int determinant_mod(long long int **matrix, int order, int mod){
    long long int re = determinant(matrix, order);
    re = Div(re, mod).remain;
    return re;
}

long long int **inv_matrix(long long int **matrix, int order, int mod){
    long long int **inv_matrix = NULL, matrix_det, inv_matrix_det, ans;
    init_matrix(&inv_matrix, order, order);
    matrix_det = determinant_mod(matrix, order, mod);
    gcd_re result = ex_gcd(matrix_det, mod);
    if(result.gcd == 1){
        inv_matrix_det = result.x;
        int i, j, flag; //flag = (-1)^(i+j)
        for(i = 0; i < order; i++){
            for(j = 0; j < order; j++){
                (i+j)%2 == 0? (flag = 1):(flag = -1);
                ans = laplace_expansion(matrix, j, i, order);
                ans *= flag * inv_matrix_det;
                inv_matrix[i][j] = Div(ans, mod).remain;
            }
        }
        return inv_matrix;
    }else{
        return NULL;
    }
}

long long int **matrix_mul(long long int **matrix1, int r1, int c1, long long int **matrix2, int r2, int c2){
    if(c1 != r2)
        exit(-1);
    else{
        int i, j, cnt;
        long long int **re = NULL;
        init_matrix(&re, r1, c2);
        for(i = 0; i < r1; i++)
            for(j = 0; j < c2; j++)
                for(cnt = 0; cnt < c1; cnt++)
                    re[i][j] += matrix1[i][cnt] * matrix2[cnt][j];
        return re;
    }
}

long long int **matrix_mul_mod(long long int **matrix1, int r1, int c1, long long int **matrix2, int r2, int c2, int mod){
    if(c1 != r2)
        exit(-1);
    else{
        int i, j, cnt;
        long long int **re = NULL;
        init_matrix(&re, r1, c2);
        for(i = 0; i < r1; i++)
            for(j = 0; j < c2; j++){
                for(cnt = 0; cnt < c1; cnt++)
                    re[i][j] += matrix1[i][cnt] * matrix2[cnt][j];
                re[i][j] = Div(re[i][j], mod).remain;
            }
        return re;
    }
}

inf str2matrix(const char *str, int column){
    long long int **matrix = NULL;
    int i, j, str_len, row;
    str_len = (int)strlen(str);
    char str_temp[LEN];
    strcpy(str_temp, str);
    str_len % column == 0? (row = str_len/column):(row = str_len/column+1);
    //对长度不足部分补齐
    for(i = row*column-1; i >= str_len; i--)
        str_temp[i] = 'A';
    str_temp[row*column] = '\0';
    init_matrix(&matrix, row, column);
    for(i = 0; i < row; i++)
        for(j = 0; j < column; j++)
            matrix[i][j] = str_temp[j+i*column]-'a';
    inf re;
    re.matrix = matrix;
    re.row = row;
    re.column = column;
    return re;
}

void get_matrix(long long int ***matrix_ptr, int r, int c){
    init_matrix(matrix_ptr, r, c);
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            scanf("%lld", &(*matrix_ptr)[i][j]);
        }
    }
    rewind(stdin);
}

void output_matrix(long long int **matrix, int r, int c){
    int i, j;
    for(i = 0; i < r; i++){
        for(j = 0; j < c; j++){
            printf("%lld ", matrix[i][j]);
        }
        //if(i != r-1)
            printf("\n");
    }
}


void mpz_init_matrix(mpz_t ***matrix_ptr, int r, int c){
    int i, j;
    *matrix_ptr = (mpz_t **)malloc(sizeof(mpz_t *)*r);
    for(i = 0; i < r; i++)
        (*matrix_ptr)[i] = malloc(sizeof(mpz_t)*c);
    for(i = 0; i < r; i++)
        for(j = 0; j < c; j++)
            mpz_init_set_si((*matrix_ptr)[i][j], 0);
}

void mpz_laplace_expansion(mpz_t **matrix, int r, int c, int order, mpz_t *cofactor){
    int i, j, ori_i, ori_j;
    mpz_t **cofactor_matrix = NULL, ans;
    mpz_init_set_si(ans, 0);
    mpz_init_matrix(&cofactor_matrix, order-1, order-1);
    for(i = 0; i < order; i++)
        for(j = 0; j < order; j++){
            ori_i = i;
            ori_j = j;
            if(i == r || j == c);
            else{
                if(i > r)   i--;
                if(j > c)   j--;
                mpz_set(cofactor_matrix[i][j], matrix[ori_i][ori_j]);
                i = ori_i;
                j = ori_j;
            }
        }
    if(order >= 2)
        mpz_determinant(cofactor_matrix, order-1, &ans);
    mpz_set(*cofactor, ans);
}

void mpz_determinant(mpz_t **matrix, int order, mpz_t *result){
    mpz_t sign;     mpz_init_set_si(sign, 1);
    mpz_t flag;     mpz_init(flag);
    mpz_t cofactor; mpz_init(cofactor);
    mpz_t ans;      mpz_init_set_si(ans, 0);
    int i;
    if(order == 1)
        mpz_set(ans, matrix[0][0]);
    else
        for(i=0; i<order; i++){
            mpz_laplace_expansion(matrix, i, 0, order, &cofactor);
            mpz_set(flag, cofactor);
            mpz_mul(flag, flag, matrix[i][0]);
            mpz_mul(flag, flag, sign);
            mpz_add(ans, ans, flag);
            mpz_mul_si(sign, sign, -1);
        }
    mpz_set(*result, ans);
}

void mpz_determinant_mod(mpz_t **matrix, int order, int mod, mpz_t *result){
    mpz_t ans;
    mpz_init(ans);
    mpz_determinant(matrix, order, &ans);
    mpz_mod_ui(ans, ans, mod);
    mpz_set(*result, ans);
}

bool mpz_inv_matrix(mpz_t **matrix, int order, int mod, mpz_t ***ret_matrix_ptr){
    mpz_t matrix_det, inv_matrix_det, mod_tran, ans;  //ans用来存放逆矩阵中每个位置的值
    mpz_init(matrix_det);               mpz_init(inv_matrix_det);
    mpz_init_set_ui(mod_tran, mod);     mpz_init(ans);
    mpz_determinant_mod(matrix, order, mod, &matrix_det);
    mpz_t inv_mod, gcd;
    mpz_init(inv_mod);                  mpz_init(gcd);
    mpz_gcdext(gcd, inv_matrix_det, inv_mod, matrix_det, mod_tran);
    if(mpz_cmp_si(gcd, 1) == 0){
        mpz_t **inv_matrix;
        mpz_init_matrix(&inv_matrix, order, order);
        int i, j, flag; //flag = (-1)^(i+j)
        for(i = 0; i < order; i++){
            for(j = 0; j < order; j++){
                (i+j)%2 == 0? (flag = 1):(flag = -1);
                mpz_laplace_expansion(matrix, j, i, order, &ans);
                mpz_mul_si(ans, ans, flag);
                mpz_mul(ans, ans, inv_matrix_det);
                mpz_mod_ui(ans, ans, mod);
                mpz_set(inv_matrix[i][j], ans);
            }
        }
        *ret_matrix_ptr = inv_matrix;
        return true;
    }else{
        *ret_matrix_ptr = NULL;
        return false;
    }
}

bool mpz_matrix_mul(mpz_t **matrix1, int r1, int c1, mpz_t **matrix2, int r2, int c2, mpz_t ***re_matrix_ptr){
    if(c1 != r2)
        return false;
    else{
        mpz_t **re_matrix = NULL, temp;
        mpz_init(temp);
        mpz_init_matrix(&re_matrix, r1, c2);
        int i, j, cnt;
        for(i = 0; i < r1; i++)
            for(j = 0; j < c2; j++)
                for(cnt = 0; cnt < c1; cnt++){
                    mpz_mul(temp, matrix1[i][cnt], matrix2[cnt][j]);
                    mpz_add(re_matrix[i][j], re_matrix[i][j], temp);
                }
        *re_matrix_ptr = re_matrix;
        return true;
    }
}

bool mpz_matrix_mul_mod(mpz_t **matrix1, int r1, int c1, mpz_t **matrix2, int r2, int c2, int mod, mpz_t ***re_matrix_ptr){
    if(c1 != r2)
        return false;
    else{
        mpz_t **re_matrix = NULL, temp;
        mpz_init(temp);
        mpz_init_matrix(&re_matrix, r1, c2);
        int i, j, cnt;
        for(i = 0; i < r1; i++)
            for(j = 0; j < c2; j++) {
                for (cnt = 0; cnt < c1; cnt++) {
                    mpz_mul(temp, matrix1[i][cnt], matrix2[cnt][j]);
                    mpz_add(re_matrix[i][j], re_matrix[i][j], temp);
                }
                mpz_mod_ui(re_matrix[i][j], re_matrix[i][j], mod);
            }
        *re_matrix_ptr = re_matrix;
        return true;
    }
}

mpz_inf mpz_str2matrix(const char *str, int column){
    mpz_t **matrix = NULL;
    int i, j, str_len, row;
    str_len = (int)strlen(str);
    char str_temp[LEN];
    strcpy(str_temp, str);
    str_len % column == 0? (row = str_len/column):(row = str_len/column+1);
    //对长度不足部分补齐
    for(i = row*column-1; i >= str_len; i--)
        str_temp[i] = 'A';
    str_temp[row*column] = '\0';
    mpz_init_matrix(&matrix, row, column);
    for(i = 0; i < row; i++)
        for(j = 0; j < column; j++)
            mpz_init_set_ui(matrix[i][j], str_temp[j+i*column]-'a');
    mpz_inf re;
    re.matrix = matrix;
    re.row = row;
    re.column = column;
    return re;
}

void mpz_get_matrix(mpz_t ***matrix_ptr, int r, int c){
    mpz_init_matrix(matrix_ptr, r, c);
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            gmp_scanf("%Zd", (*matrix_ptr)[i][j]);
        }
    }
    rewind(stdin);
}

void mpz_output_matrix(mpz_t **matrix, int r, int c){
    int i, j;
    for(i = 0; i < r; i++){
        for(j = 0; j < c; j++){
            gmp_printf("%Zd ", matrix[i][j]);
        }
        printf("\n");
    }
}