//
// Created by 赵睿智 on 2022/3/21.
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

#define len 1000000

int main(){
    char s[len];
    scanf("%s", s);
    letter_freq_ptr freq_form = Letter_freq_match(s);
    int offset = freq_form[0].letter - 'e';
    printf("%d", offset);
    return 0;
}