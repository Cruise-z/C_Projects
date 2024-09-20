//
// Created by 赵睿智 on 2022/5/18.
//

#include <stdio.h>
#include "../library_file/ECC/ECC_enc_dec.h"

int main(){
    int op;
    EC C;
    init_EC(&C);
    point G;
    init_point(&G);
    gmp_scanf("%Zd%Zd%Zd", &C.p, &C.a, &C.b);
    gmp_scanf("%Zd%Zd", &G.x, &G.y);
    uint64_t par;
    scanf("%lld\n", &par);
    scanf("%d\n", &op);
    char M[LEN];
    scanf("%s\n", M);
    byte_arr byte_M;
    str2byteString(&byte_M, M+2, 0);
    if(op == 1){
        point pb;
        init_point(&pb);
        gmp_scanf("%Zd%Zd", &pb.x, &pb.y);
        mpz_t k, ans;
        mpz_init(k);
        gmp_scanf("%Zd", &k);
        ECC_encode(&ans, byte_M, k, par, G, pb, C);
        char format[LEN];
        strcpy(format, "0x0%\0");
        strcat(format, "Zx\0");
        gmp_printf(format, ans);
    }else if(op == 0){
        mpz_t Db;
        byte_arr ans;
        mpz_init(Db);
        gmp_scanf("%Zd", &Db);
        ECC_decode(&ans, byte_M, par, G, Db, C);
        printf("0x");
        printHex(ans);
    }
    return 0;
}