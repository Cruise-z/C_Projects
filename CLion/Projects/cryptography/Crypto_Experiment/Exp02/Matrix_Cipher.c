//
// Created by 赵睿智 on 2022/3/19.
//

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../library_file/My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../library_file/My_Matrix_cal/My_Matrix_cal.h"
#include "../library_file/Classical_Cipher/Classical_Cipher.h"

#define len 2048

int main(){
    int n, i, op;
    mc_key_inf_ptr key = NULL;
    char s[len], key_str[len];
    scanf("%d\n", &n);
    scanf("%s\n", key_str);
    key = matrix_cipher_key_init(key_str, n);
    scanf("%s\n", s);
    scanf("%d", &op);
    if(op == 1){
        //key = matrix_cipher_key_sort(key, n);
        char *re = Matrix_cipher(s, key, n, 'e');
        printf("%s", re);
    }else if(op == 0){
        char *re = Matrix_cipher(s, key, n, 'd');
        printf("%s", re);
    }
    return 0;
}