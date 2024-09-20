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
    char *mod_str = "11b";
    char b[1024], p[1024];
    scanf("%s%s",&b, &p);
    char *ans = GF_2_hex_Fp(b, p, mod_str);
    puts(ans);
    return 0;
}

