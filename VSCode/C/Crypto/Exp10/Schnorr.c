//
// Created by 赵睿智 on 2022/6/5.
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
    scP parm;
    init_Schnorr_Parm(&parm);
    wchar_t m[LEN];
    char mode[LEN];
    gmp_scanf("%Zd\n", &parm.p);
    gmp_scanf("%Zd\n", &parm.q);
    gmp_scanf("%Zd\n", &parm.a);
    get_wchar(stdin, m);
    scanf("%s", mode);
    byte_arr byte_m;
    init_byte_arr(&byte_m);
    WStr2ByteStr_utf8(&byte_m, m);
    if(strcmp(mode, "Sign") == 0){
        mpz_t s, r;
        mpz_init(s);
        mpz_init(r);
        gmp_scanf("%Zd", &s);
        gmp_scanf("%Zd", &r);
        sig ans;
        Schnorr_Sign(&ans, byte_m, s, r, parm, SHA1);
        gmp_printf("%Zd %Zd", ans.S1, ans.S2);
    }else if(strcmp(mode, "Vrfy") == 0){
        mpz_t v;
        mpz_init(v);
        sig sigp;
        init_Sig(&sigp);
        gmp_scanf("%Zd", &v);
        gmp_scanf("%Zd", &sigp.S1);
        gmp_scanf("%Zd", &sigp.S2);
        bool j = Schnorr_Vrfy(byte_m, v, sigp, parm, SHA1);
        if(j == true)
            printf("True");
        else
            printf("False");
    }
    return 0;
}
