//
// Created by 赵睿智 on 2022/6/7.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include "../../.lib/.extern_lib/Wrap/wrap.h"
#include "../../.lib/HashLib/Hash.h"
#include "../../.lib/conversion/lib_Convert.h"
#include "../../.lib/RSA_Cipher/RSA_enc_dec.h"
#include "../../.lib/ECC/ECC_enc_dec.h"
#include "../../.lib/Digital_Signature/Digital_Signature.h"

int main(){
    char mode[LEN];
    EC ec;
    init_EC(&ec);
    gmp_scanf("%Zd\n%Zd\n%Zd\n", &ec.p, &ec.a, &ec.b);
    point G, Pa;
    init_point(&G);
    init_point(&Pa);
    gmp_scanf("%Zd%Zd\n%Zd\n", &G.x, &G.y, &G.order);
    wchar_t ida[LEN], m[LEN];
    byte_arr bm;
    get_wchar(stdin, ida);
    gmp_scanf("%Zd%Zd\n", &Pa.x, &Pa.y);
    get_wchar(stdin, m);
    WStr2ByteStr_utf8(&bm, m);
    scanf("%s\n", mode);
    if(strcmp(mode, "Sign") == 0){
        mpz_t da, K;
        mpz_init(da);
        mpz_init(K);
        gmp_scanf("%Zd", &da);
        gmp_scanf("%Zd", &K);
        sig ans;
        SM2_Sign(&ans, bm, ida, da, Pa, K, G, ec, SM3);
        gmp_printf("%Zd\n%Zd", ans.S1, ans.S2);
    }else if(strcmp(mode, "Vrfy") == 0){
        sig sigp;
        init_Sig(&sigp);
        gmp_scanf("%Zd", &sigp.S1);
        gmp_scanf("%Zd", &sigp.S2);
        bool j = SM2_vrfy(bm, sigp, ida, Pa, G, ec, SM3);
        if(j == true)
            printf("True");
        else
            printf("False");
    }
    return 0;
}