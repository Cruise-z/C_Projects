//
// Created by 赵睿智 on 2022/5/7.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "../../.lib/RSA_Cipher/RSA_enc_dec.h"
#include "../../.lib/conversion/lib_Convert.h"

static const char hex_chars[] = "0123456789abcdef";

int  main(int argc, char **argv)
{
    char sText[100];
    gets(sText);
    byte_arr sSHA;
    unsigned char *ret = SHA1((unsigned char *)sText, strlen(sText), sSHA.arr);
    sSHA.len = 20;
    printf("ret %p \n", ret);
    printHex(sSHA);
}