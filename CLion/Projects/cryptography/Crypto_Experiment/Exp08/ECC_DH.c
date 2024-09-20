//
// Created by 赵睿智 on 2022/5/16.
//

#include <stdio.h>
#include "../library_file/ECC/ECC_enc_dec.h"

int main(){
    EC C;
    init_EC(&C);
    point G, Pb;
    init_point(&G);
    init_point(&Pb);
    gmp_scanf("%Zd%Zd%Zd", &C.p, &C.a, &C.b);
    gmp_scanf("%Zd%Zd", &G.x, &G.y);
    mpz_t Xa;
    mpz_init(Xa);
    gmp_scanf("%Zd", &Xa);
    gmp_scanf("%Zd%Zd", &Pb.x, &Pb.y);
    point ans;
    sMul_point(&ans, Pb, Xa, C);
    gmp_printf("%Zd %Zd", ans.x, ans.y);
}