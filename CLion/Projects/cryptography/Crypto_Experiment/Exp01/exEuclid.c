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
#include "../library_file/My_BigNum_GF_cal/My_BigNum_GF_cal.h"

int main(int argc, char **argv) {
    char s1[LEN], s2[LEN];
    scanf("%s%s",s1,s2);
    mpz_gcd_re re = str_gcdext(s1, s2);
    printf("%s %s %s", re.x_str, re.y_str, re.gcd_str);
    return 0;
}

