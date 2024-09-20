//
// Created by 赵睿智 on 2022/5/13.
//

#include <stdio.h>
#include "../library_file/ECC/ECC_enc_dec.h"

int main(){
    point p, q, ans;
    EC C;
    mpz_t k;
    mpz_init(k);
    init_EC(&C);
    init_point(&p);
    init_point(&q);
    gmp_scanf("%Zd%Zd%Zd", &C.p, &C.a, &C.b);
    gmp_scanf("%Zd%Zd", &p.x, &p.y);
    gmp_scanf("%Zd%Zd", &q.x, &q.y);
    gmp_scanf("%Zd", &k);
    Add_point(&ans, p, q, C);
    gmp_printf("%Zd %Zd\n", ans.x, ans.y);
    Sub_point(&ans, p, q, C);
    gmp_printf("%Zd %Zd\n", ans.x, ans.y);
    sMul_point(&ans, p, k, C);
    gmp_printf("%Zd %Zd\n", ans.x, ans.y);
    sDiv_point(&ans, q, k, C);
    gmp_printf("%Zd %Zd", ans.x, ans.y);
    return 0;
}