//
// Created by 赵睿智 on 2022/6/2.
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
    wchar_t m[LEN];
    get_wchar(stdin, m);
    byte_arr byte_m, ans;
    WStr2ByteStr_utf8(&byte_m, m);
    SM3(byte_m.arr, byte_m.len, ans.arr);
    ans.len = HASH_DIGEST_SIZE;
    printHex(ans);
    return 0;
}