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
    int fence_len, op;
    char s[len], *re = NULL;
    scanf("%d\n", &fence_len);
    scanf("%s\n", s);
    scanf("%d", &op);
    if(op == 1){
        re = Fence(s, fence_len, 'e');
        printf("%s", re);
    }else if(op == 0){
        re = Fence(s, fence_len, 'd');
        printf("%s", re);
    }
    return 0;
}