//
// Created by 赵睿智 on 2022/5/6.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include "../library_file/My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../library_file/RSA_Cipher/RSA_enc_dec.h"

int main(){
    int op, k;
    scanf("%d\n", &op);
    //mpz_set_str对于16进制数无法读取前面的0x
    mpz_t re;
    if(op == 1){
        char str_exp[LEN], str_N[LEN], str_M[LEN], str_L[LEN], str_Seed[LEN];
        scanf("%d\n", &k);
        scanf("%s\n", str_exp);
        scanf("%s\n", str_N);
        scanf("%s\n", str_M);
        scanf("%s\n", str_L);
        scanf("%s", str_Seed);
        int judge = OAEP_SHA1(&re, k, str_M, str_L, str_Seed, 'e', str_exp, str_N);
        char format[LEN];
        strcpy(format, "0x%\0");
        sprintf(format+3, "%d", k);
        strcat(format, "Zx\0");
        if(judge == 0)
            gmp_printf(format, re);
//        else
//            printf("\nreturn %d", judge);
    }else if(op == 0){
        char str_exp[LEN], str_N[LEN], str_M[LEN], str_L[LEN];
        scanf("%d\n", &k);
        scanf("%s\n", str_exp);
        scanf("%s\n", str_N);
        scanf("%s\n", str_M);
        scanf("%s", str_L);
        int judge = OAEP_SHA1(&re, k, str_M, str_L, "\0", 'd', str_exp, str_N);
        if(judge == 0)
            gmp_printf("0x%Zx", re);
//        else
//            printf("\nreturn %d", judge);
    }
    return 0;
}