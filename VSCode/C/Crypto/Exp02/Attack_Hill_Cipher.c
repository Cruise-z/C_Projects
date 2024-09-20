//
// Created by 赵睿智 on 2022/3/20.
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
    int order;
    char m[len], c[len];
    scanf("%d\n", &order);
    scanf("%s\n", m);
    scanf("%s", c);
    long long int **re = NULL;
    re = Attack_Hill(m, c, order);
    output_matrix(re, order, order);
    return 0;
}