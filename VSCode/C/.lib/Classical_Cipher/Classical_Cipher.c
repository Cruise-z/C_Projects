//
// Created by 赵睿智 on 2022/3/17.
//

#include "Classical_Cipher.h"
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

char *Hill(const char *s, long long int **key, int order, char mode){
    inf m_matrix = str2matrix(s, order);
    long long int **re_matrix = NULL;
    if(mode == 'e')
        re_matrix = matrix_mul_mod(m_matrix.matrix, m_matrix.row, m_matrix.column, key, order, order, 26);
    else if(mode == 'd'){
        long long int **inversion_matrix = NULL;
        inversion_matrix = inv_matrix(key, order, 26);
        re_matrix = matrix_mul_mod(m_matrix.matrix, m_matrix.row, m_matrix.column, inversion_matrix, order, order, 26);
    }else exit(-1);
    char *re = (char *)malloc(sizeof(char)*(m_matrix.row*(order+1)));
    int i, j;
    for(i = 0; i < m_matrix.row; i++){
        for(j = 0; j < order; j++){
            re_matrix[i][j] += 'a';
            re[j+i*order] = (char)re_matrix[i][j];
        }
    }
    re[j+i*order] = '\0';
    return re;
}

long long int **Attack_Hill(const char *M, const char *C, int order){
    long long int cnt, **inv_m_matrix = NULL, **re_temp = NULL;
    inf m_matrix, c_matrix;
    for(cnt = 0; cnt <= strlen(M)-order*order; cnt++){
        m_matrix = str2matrix(&M[cnt], order);      m_matrix.row = order;
        c_matrix = str2matrix(&C[cnt], order);      c_matrix.row = order;
        if((inv_m_matrix = inv_matrix(m_matrix.matrix, order, 26)) != NULL){
            re_temp = matrix_mul_mod(inv_m_matrix, order, order, c_matrix.matrix, order, order, 26);
            if(inv_matrix(re_temp, order, 26) != NULL){
                //检验可能会出现两次逆均通过但不是key的情况
                char *C_temp = Hill(M, re_temp, order, 'e');
                if(strcmp(C_temp, C) == 0)
                    return re_temp;
            }
        }
    }
    return NULL;
}

char* mpz_Hill(const char *s, mpz_t **key, int order, char mode){ //m:plaintext
    mpz_inf m_matrix = mpz_str2matrix(s, order);
    mpz_t **re_matrix = NULL;
    if(mode == 'e')
        mpz_matrix_mul_mod(m_matrix.matrix, m_matrix.row, m_matrix.column, key, order, order, 26, &re_matrix);
    else if(mode == 'd'){
        mpz_t **inversion_matrix = NULL;
        mpz_inv_matrix(key, order, 26, &inversion_matrix);
        mpz_matrix_mul_mod(m_matrix.matrix, m_matrix.row, m_matrix.column, inversion_matrix, order, order, 26, &re_matrix);
    }else exit(-1);
    char *re = (char *)malloc(sizeof(char)*(m_matrix.row*(order+1)));
    int i, j;
    for(i = 0; i < m_matrix.row; i++){
        for(j = 0; j < order; j++){
            mpz_add_ui(re_matrix[i][j], re_matrix[i][j], 'a');
            re[j+i*order] = (char)mpz_get_si(re_matrix[i][j]);
        }
    }
    re[j+i*order] = '\0';
    return re;
}

bool Affine_check_key(long long int k, int word_range){
    gcd_re re;
    re = ex_gcd(k , word_range);
    return (re.gcd == 1);
}

char *Affine(const char *m, long long int k, long long int b, int word_range, char mode){
    if(Affine_check_key(k, word_range) == false)
        exit(-1);
    int i = 0;
    char *re = (char *)malloc(sizeof(char)*(strlen(m)+1));
    div_re div;
    if(mode == 'e'){
        for(i = 0; i < strlen(m); i++){
            div = Div(k*(m[i]-'a')+b, 26);
            re[i] = (char)(div.remain + 'a');
        }
    }else if(mode == 'd'){
        gcd_re gcd = ex_gcd(k, 26);
        for(i = 0; i < strlen(m); i++){
            div = Div(gcd.x*((m[i]-'a')-b), 26);
            re[i] = (char)(div.remain + 'a');
        }
    }
    re[i] = '\0';
    return re;
}

bool mpz_Affine_check_key(mpz_t k, int word_range){
    mpz_t gcd, one;
    mpz_init_set_ui(one, 1);
    mpz_init(gcd);
    mpz_gcd_ui(gcd, k, word_range);
    return (mpz_cmp(gcd, one) == 0);
}

char *mpz_Affine(const char *m, mpz_t k, mpz_t b, int word_range, char mode){
    if(mpz_Affine_check_key(k, word_range) == false)
        exit(-1);
    int i = 0;
    char *re = (char *)malloc(sizeof(char)*(strlen(m)+1));
    mpz_t p, flag;
    mpz_init(flag);
    if(mode == 'e'){
        for(i = 0; i < strlen(m); i++){
            mpz_init_set_si(p, m[i]-'a');
            mpz_mul(flag, k, p);
            mpz_add(flag, flag, b);
            mpz_mod_ui(flag, flag, word_range);
            re[i] = mpz_get_si(flag) + 'a';
        }
    }else if(mode == 'd'){
        mpz_t inv_k, word_range_mpz;
        mpz_init(inv_k);
        mpz_init_set_ui(word_range_mpz, word_range);
        mpz_invert(inv_k, k, word_range_mpz);
        for(i = 0; i < strlen(m); i++){
            mpz_init_set_si(p, m[i]-'a');
            mpz_sub(flag, p, b);
            mpz_mul(flag, flag, inv_k);
            mpz_mod_ui(flag, flag, word_range);
            re[i] = mpz_get_si(flag) + 'a';
        }
    }
    re[i] = '\0';
    return re;
}

//加密比较函数
int qsort_encode_cmp(const void *a, const void *b){
    int a_val = (int)((mono_sub_ptr)a)->m;
    int b_val = (int)((mono_sub_ptr)b)->m;
    return a_val < b_val ? -1:1;
}

//解密比较函数
int qsort_decode_cmp(const void *a, const void *b){
    int a_val = (int)((mono_sub_ptr)a)->c;
    int b_val = (int)((mono_sub_ptr)b)->c;
    return a_val < b_val ? -1:1;
}

char *Mono_substitution(mono_sub_ptr key_ptr, char *str, int word_range, const char mode){
    char *re = (char *)malloc(sizeof(char)*(strlen(str)+1));
    if(mode == 'e')
        qsort(key_ptr, word_range, sizeof(key_ptr[0]), qsort_encode_cmp);
    else if(mode == 'd')
        qsort(key_ptr, word_range, sizeof(key_ptr[0]), qsort_decode_cmp);
    int i = 0;
    if(mode == 'e')
        for(i = 0; i < strlen(str); i++)
            re[i] = key_ptr[str[i]-'a'].c;
    else if(mode == 'd')
        for(i = 0; i < strlen(str); i++)
            re[i] = key_ptr[str[i]-'a'].m;
    re[i] = '\0';
    return re;
}

ring_ptr struct_ring(char *str){
     ring_ptr ptr = (ring_ptr)malloc(sizeof(ring)*strlen(str));
     int i;
     for(i = 0; i < strlen(str)-1; i++){
         ptr[i].c = str[i];
         ptr[i].next = &ptr[i+1];
     }
     ptr[i].c = str[i];
     ptr[i].next = &ptr[0];
     return ptr;
}

char *Vigenere(char *key, char *str, int word_range, char mode){
    ring_ptr key_ring = struct_ring(key), ptr = key_ring;
    char *re = (char *)malloc(sizeof(char)*(strlen(str)+1));
    int i = 0;
    if(mode == 'e'){
        for(i = 0; i < strlen(str); i++){
            re[i] = (char)((str[i]-'a'+ptr->c-'a')%word_range + 'a');
            ptr = ptr->next;
        }
    }else if(mode == 'd'){
        for(i = 0; i < strlen(str); i++){
            int val = ((str[i]-'a') - (ptr->c-'a'))%word_range;
            int pos = val<0?(val+word_range):val;
            re[i] = (char)(pos + 'a');
            ptr = ptr->next;
        }
    }
    re[i] = '\0';
    return re;
}

char *Vernam(char *key, char *str, char mode){
    ring_ptr key_ring = struct_ring(key), ptr = key_ring;
    char *re = (char *)malloc(sizeof(char)*(strlen(str)+1));
    int i = 0;
    if(mode == 'e' || mode == 'd'){
        for(i = 0; i < strlen(str); i++){
            re[i] = (char)((ptr->c)^(str[i]));
            ptr = ptr->next;
        }
    }
    re[i] = '\0';
    return re;
}

char *Fence(char *str, int fence_len, char mode){
    long long int str_len = strlen(str), recnt = 0, pos;
    char *re = (char *)malloc(sizeof(char)*(str_len+1));
    if(mode == 'e'){
        long long int group_cnt;
        int fence_len_mod;
        for(fence_len_mod = 0; fence_len_mod < fence_len; fence_len_mod++)
            for(group_cnt = 0; (pos = group_cnt*fence_len+fence_len_mod) < str_len; group_cnt++)
                re[recnt++] = str[pos];
        re[str_len] = '\0';
    }else if(mode == 'd'){
        div_re result = Div(str_len, fence_len);
        long long int true_group, group_cnt, cnt;
        result.remain != 0?(true_group = result.ans+1):(true_group = result.ans);
        for(group_cnt = 0; group_cnt < true_group; group_cnt++)
            for(pos = group_cnt, cnt = 0; pos < str_len; cnt<result.remain?(pos+=true_group):(pos+=result.ans), cnt++)
                re[recnt++] = str[pos];
        re[str_len] = '\0';
    }
    return re;
}

mc_key_inf_ptr matrix_cipher_key_init(char *key_str, int num){
    int cnt, str_len = (int)strlen(key_str);
    mc_key_inf_ptr key = NULL;
    if(str_len != num){
        printf("key_length is not matched");
        exit(-1);
    }
    key = (mc_key_inf_ptr)malloc(sizeof(mc_key_inf)*num);
    for(cnt = 0; cnt < num; cnt++){
        key[cnt].column = cnt;
        key[cnt].order = key_str[cnt] - '0';
    }
    return key;
}

int matrix_cipher_key_cmp(const void *a, const void *b){
    int a_val = (int)((mc_key_inf_ptr)a)->order;
    int b_val = (int)((mc_key_inf_ptr)b)->order;
    return a_val < b_val ? -1:1;
}

mc_key_inf_ptr matrix_cipher_key_sort(mc_key_inf_ptr key, int num){
    qsort(key, num, sizeof(key[0]), matrix_cipher_key_cmp);
    return key;
}

char *Matrix_cipher(char *s, mc_key_inf_ptr key_ptr, int key_len, char mode){
    long long int str_len = strlen(s), recnt = 0;
    char *re = (char *)malloc(sizeof(char)*(str_len+1));
    if(mode == 'e'){
        long long int row, row_cnt, key_cnt;
        str_len%key_len == 0?(row = str_len/key_len):(row = str_len/key_len+1);
        inf matrix_inf = str2matrix(s, key_len);
        matrix_cipher_key_sort(key_ptr, key_len);
        for(key_cnt = 0; key_cnt < key_len; key_cnt++)
            for(row_cnt = 0; row_cnt < row; row_cnt++)
                re[recnt++] = (char)(matrix_inf.matrix[row_cnt][key_ptr[key_cnt].column] + 'a');
        re[recnt] = '\0';
    }else if(mode == 'd'){
        long long int column, column_cnt, key_cnt;
        str_len%key_len == 0?(column = str_len/key_len):(column = str_len/key_len+1);
        inf matrix_inf = str2matrix(s, (int)column);
        for(column_cnt = 0; column_cnt < column; column_cnt++)
            for(key_cnt = 0; key_cnt < key_len; key_cnt++)
                re[recnt++] = (char)(matrix_inf.matrix[key_ptr[key_cnt].order-1][column_cnt] + 'a');
        re[recnt] = '\0';
    }
    return re;
}

void init_letter_freq_form(letter_freq_ptr *form_ptr){
    int i;
    *form_ptr = (letter_freq_ptr)malloc(sizeof(letter_freq)*255);//asc码的取值范围0-255
    for(i = 0; i < 255; i++){
        (*form_ptr)[i].freq = 0;
        (*form_ptr)[i].letter = (char)i;
    }
}

int qsort_letter_freq_cmp(const void *a, const void *b){
    int a_val = (int)((letter_freq_ptr)a)->freq;
    int b_val = (int)((letter_freq_ptr)b)->freq;
    return a_val < b_val ? 1:-1;
}

letter_freq_ptr Letter_freq_match(char *s){
    letter_freq_ptr form_ptr = NULL;
    init_letter_freq_form(&form_ptr);
    long long int cnt;
    for(cnt = 0; cnt < strlen(s); cnt++)
        form_ptr[s[cnt]].freq++;
    qsort(form_ptr, 255, sizeof(form_ptr[0]), qsort_letter_freq_cmp);
    return form_ptr;
}