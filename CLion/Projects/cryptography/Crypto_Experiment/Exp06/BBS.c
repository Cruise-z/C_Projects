//
// Created by 赵睿智 on 2022/4/21.
//

#include <stdio.h>
#include <stdlib.h>
#include "../library_file/Stream_Cipher/Stream_Cipher.h"

int main(){
    int len;
    mpz_t p, q, s, ans;
    mpz_init(p);
    mpz_init(q);
    mpz_init(s);
    mpz_init(ans);
    scanf("%d", &len);
    gmp_scanf("%Zd", &p);
    gmp_scanf("%Zd", &q);
    gmp_scanf("%Zd", &s);
    BBS_random(&ans, len, p, q, s);
    gmp_printf("%Zd", ans);
    return 0;
}
