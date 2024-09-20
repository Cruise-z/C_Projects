//
// Created by 赵睿智 on 2022/3/17.
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
    long long int k, b;
    char s[len];
    int op;
    scanf("%lld%lld",&k,&b);    rewind(stdin);
    scanf("%s", s);    rewind(stdin);
    scanf("%d",&op);    rewind(stdin);
    if(Affine_check_key(k, 26) == false){
        printf("invalid key");
        exit(-1);
    }
    if(op == 1){
        char *re = Affine(s, k, b, 26, 'e');
        printf("%s", re);
    }else if(op == 0){
        char *re = Affine(s, k, b, 26, 'd');
        printf("%s", re);
    }
    return 0;
}