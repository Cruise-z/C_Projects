//
// Created by 赵睿智 on 2022/5/24.
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

int main(){
    char key_str[LEN];
    wchar_t m[LEN];
    scanf("%s\n", key_str);
    get_wchar(stdin, m);
    byte_arr byte_key, byte_m, ans;
    str2byteString(&byte_key, key_str, 0);
    WStr2ByteStr_utf8(&byte_m, m);
    crypto_HMAC_SHA1(&ans, byte_key, byte_m);
    printHex(ans);
    return 0;
}