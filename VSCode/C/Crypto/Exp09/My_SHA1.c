//
// Created by 赵睿智 on 2022/5/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <openssl/sha.h>
#include "../../.lib/.extern_lib/Wrap/wrap.h"
#include "../../.lib/HashLib/Hash.h"
#include "../../.lib/conversion/lib_Convert.h"
#include "../../.lib/RSA_Cipher/RSA_enc_dec.h"
#include "../../.lib/ECC/ECC_enc_dec.h"

int main(){
    byte_arr byte_str, ans;
    byte_str.len = 0;
    wchar_t a[LEN];
    get_wchar(stdin, a);
    //fwprintf(stdout, a, 2048);
    WStr2ByteStr_utf8(&byte_str, a);
    crypto_SHA1(&ans, byte_str);
    printHex(ans);
    return 0;
}