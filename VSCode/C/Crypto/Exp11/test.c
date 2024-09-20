//
// Created by 赵睿智 on 2022/6/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../.lib/miracl/hexdump.h"
#include "../../.lib/miracl/sm2.h"
#include "../../.lib/ECC/ECC_enc_dec.h"

int main(){
    clock_t start, end;
    double tt;
    int cnt;
    point p, ans;
    EC C;
    mpz_t k;
    mpz_init(k);
    init_EC(&C);
    init_point(&p);
    gmp_scanf("%Zd%Zd%Zd", &C.p, &C.a, &C.b);
    gmp_scanf("%Zd%Zd", &p.x, &p.y);
    gmp_scanf("%Zd", &k);
    start = clock();
    for(cnt = 0; cnt < 200; cnt++)
        slid_window_smul1(&ans, p, k, C);
    end = clock();
    tt = (double)(end-start)/CLOCKS_PER_SEC;
    printf("slid_window_smul speed:%lf/s\n", (double)1/tt);
    start = clock();
    for(cnt = 0; cnt < 200; cnt++)
        sMul_point(&ans, p, k, C);
    end = clock();
    tt = (double)(end-start)/CLOCKS_PER_SEC;
    printf("standard_smul speed:%lf/s\n", (double)1/tt);
    gmp_printf("%Zd %Zd\n", ans.x, ans.y);
    return 0;
}

