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

int main(int argc, char **argv) {
    mpz_t a;
    char message[1024];
    gets(message);
    mpz_init_set_str(a, message, 10);
    if(rabin_wit(a)==false)
        printf("NO");
    else
        printf("YES");
    return 0;
}


