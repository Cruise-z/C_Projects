//
// Created by 赵睿智 on 2022/5/19.
//

#include <stdio.h>
#include <string.h>
#include "../../.lib/.extern_lib/Wrap/wrap.h"
#include "../../.lib/HashLib/Hash.h"
#include "../../.lib/conversion/lib_Convert.h"


int log_hexdump(const char *title, const unsigned char *data, int len)
{
    char str[160], octet[10];
    int ofs, i, k, d;
    const unsigned char *buf = (const unsigned char *)data;
    const char dimm[] = "+------------------------------------------------------------------------------+";

    printf("%s (%d bytes):\r\n", title, len);
    printf("%s\r\n", dimm);
    printf("| Offset  : 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   0123456789ABCDEF |\r\n");
    printf("%s\r\n", dimm);

    for (ofs = 0; ofs < (int)len; ofs += 16) {
        d = snprintf( str, sizeof(str), "| %08X: ", ofs );

        for (i = 0; i < 16; i++) {
            if ((i + ofs) < (int)len) {
                snprintf( octet, sizeof(octet), "%02X ", buf[ofs + i] );
            } else {
                snprintf( octet, sizeof(octet), "   " );
            }

            d += snprintf( &str[d], sizeof(str) - d, "%s", octet );
        }
        d += snprintf( &str[d], sizeof(str) - d, "  " );
        k = d;

        for (i = 0; i < 16; i++) {
            if ((i + ofs) < (int)len) {
                str[k++] = (0x20 <= (buf[ofs + i]) &&  (buf[ofs + i]) <= 0x7E) ? buf[ofs + i] : '.';
            } else {
                str[k++] = ' ';
            }
        }

        str[k] = '\0';
        printf("%s |\r\n", str);
    }

    printf("%s\r\n", dimm);

    return 0;
}

int main(int argc, const char *argv[])
{
    char data[LEN];
    fgets(data, 1024, stdin);

    int cnt;
    for(cnt = 0; cnt < strlen(data); cnt++){
        printf("%c", data[cnt]);
    }
    printf("\n");

    char digest_exp_str[LEN];
    //用标准库函数检验
    byte_arr ans_byte;
    SHA1((unsigned char *)data, strlen(data), ans_byte.arr);
    ans_byte.len = 20;
    utils_bytes_2_hex_string(ans_byte.arr, ans_byte.len, digest_exp_str);
    //检验结束
    uint8_t digest_calc[SHA1_DIGEST_LEN];
    uint8_t digest_exp_hex[SHA1_DIGEST_LEN];
    sha1_ctx_t ctx;
    const char *p_calc = data;
    uint16_t len_bytes;

    if (argc > 1) {
        p_calc = argv[1];
    }

    crypto_sha1_init(&ctx);
    crypto_sha1_update(&ctx, (uint8_t *)p_calc, strlen(p_calc));
    crypto_sha1_final(&ctx, digest_calc);

    utils_hex_string_2_bytes(digest_exp_str, digest_exp_hex, &len_bytes);
    if (len_bytes == sizeof(digest_calc) && !memcmp(digest_calc, digest_exp_hex, sizeof(digest_calc))) {
        printf("SHA1 digest test OK\n");
        log_hexdump("digest_calc", digest_calc, sizeof(digest_calc));
    } else {
        log_hexdump("digest_calc", digest_calc, sizeof(digest_calc));
        log_hexdump("digest_exp", digest_exp_hex, sizeof(digest_exp_hex));
        printf("SHA1 digest test FAIL\n");
    }

    for(cnt = 0; cnt < 20; cnt++){
        printf("%02x", digest_calc[cnt]);
    }
    return 0;
}