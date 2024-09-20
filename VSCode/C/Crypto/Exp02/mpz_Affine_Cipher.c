//
// Created by 赵睿智 on 2022/3/18.
//

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../../.lib/My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../../.lib/My_Matrix_cal/My_Matrix_cal.h"
#include "../../.lib/Classical_Cipher/Classical_Cipher.h"

#define len 2048

int main(){
    mpz_t k, b;
    char s[len];
    int op;
    mpz_init(k);
    mpz_init(b);
    gmp_scanf("%Zd%Zd",k,b);    rewind(stdin);
    gets(s);    rewind(stdin);
    scanf("%d",&op);    rewind(stdin);
    if(mpz_Affine_check_key(k, 26) == false){
        printf("invalid key");
        exit(-1);
    }
    if(op == 1){
        char *re = mpz_Affine(s, k, b, 26, 'e');
        puts(re);
    }else if(op == 0){
        char *re = mpz_Affine(s, k, b, 26, 'd');
        puts(re);
    }
    return 0;
}