//
// Created by 赵睿智 on 2022/3/16.
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
    int order, op;
    char s[len];
    long long int **matrix = NULL;
    scanf("%d\n", &order);
    get_matrix(&matrix, order, order);
    scanf("%s\n", s);
    scanf("%d", &op);
    if(op == 1){
        char *c = Hill(s, matrix, order, 'e');
        puts(c);
    }else if(op == 0){
        char *c = Hill(s, matrix, order, 'd');
        puts(c);
    }
    return 0;
}