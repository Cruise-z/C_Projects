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
#include "../../.lib/My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../../.lib/My_Matrix_cal/My_Matrix_cal.h"
#include "../../.lib/Classical_Cipher/Classical_Cipher.h"

#define len 2048

int main(){
    char key[len], str[len];
    int op;
    scanf("%s\n", key);
    scanf("%s\n", str);
    scanf("%d", &op);
    if(op == 1){
        char *re = Vernam(key, str, 'e');
        puts(re);
    }else if(op == 0){
        char *re = Vernam(key, str, 'd');
        puts(re);
    }
    return 0;
}
