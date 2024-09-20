//
// Created by 赵睿智 on 2022/6/4.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include "../extern_lib/Wrap/wrap.h"
#include "../library_file/HashLib/Hash.h"
#include "../library_file/conversion/lib_Convert.h"
#include "../library_file/RSA_Cipher/RSA_enc_dec.h"
#include "../library_file/ECC/ECC_enc_dec.h"
#include "../library_file/Digital_Signature/Digital_Signature.h"


int main(){
    elP parm;
    init_ElGamal_Parm(&parm);
    wchar_t s[LEN];
    char mode[LEN];
    gmp_scanf("%Zd\n", &parm.p);
    gmp_scanf("%Zd\n", &parm.g);
    get_wchar(stdin, s);
    scanf("%s", mode);
    byte_arr byte_m;
    init_byte_arr(&byte_m);
    WStr2ByteStr_utf8(&byte_m, s);
    if(strcmp(mode, "Sign") == 0){
        mpz_t Xa, K;
        mpz_init(Xa);
        mpz_init(K);
        gmp_scanf("%Zd", &Xa);
        gmp_scanf("%Zd", &K);
        sig ans;
        ElGamal_Sign(&ans, byte_m, Xa, K, parm, SHA256);
        gmp_printf("%Zd %Zd", ans.S1, ans.S2);
    }else if(strcmp(mode, "Vrfy") == 0){
        mpz_t Ya;
        mpz_init(Ya);
        sig sigp;
        init_Sig(&sigp);
        gmp_scanf("%Zd", &Ya);
        gmp_scanf("%Zd", &sigp.S1);
        gmp_scanf("%Zd", &sigp.S2);
        bool j = ElGamal_Vrfy(byte_m, Ya, sigp, parm, SHA256);
        if(j == true)
            printf("True");
        else
            printf("False");
    }
    return 0;
}