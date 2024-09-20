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
    char *mod_str = "11b";
    char s[1024], X[1024], Y, Z[1024];
    gets(s);
    int cnt = 0, cntX = 0, cntZ = 0;
    while (s[cnt] == ' ')
        cnt++;
    while (s[cnt] != ' ')
        X[cntX++] = s[cnt++];
    X[cntX] = '\0';    //cntX = strlen(X);
    while (s[cnt] == ' ')
        cnt++;
    Y = s[cnt];
    cnt++;
    while (s[cnt] == ' ')
        cnt++;
    while (s[cnt] != ' ')
        Z[cntZ++] = s[cnt++];
    Z[cntZ] = '\0';    //cntZ = strlen(Z);

    mpz_div_re result;

    if (Y == '+' || Y == '-') {
        result = GF_2_hex_cal(Y,X,Z,mod_str);
        puts(result.ans);
    }else if(Y == '*')
    {
        result = GF_2_hex_cal(Y,X,Z,mod_str);
        puts(result.ans);
    }else if (Y == '/')
    {
        result = GF_2_hex_cal(Y,X,Z,mod_str);
        puts(result.ans);
        puts(result.remain);
    }
    return 0;
}


