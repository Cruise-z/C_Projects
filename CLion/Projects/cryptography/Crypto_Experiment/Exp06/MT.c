//
// Created by 赵睿智 on 2022/4/21.
//

#include <stdio.h>
#include <stdlib.h>
#include "../library_file/Stream_Cipher/Stream_Cipher.h"

int main(){
    int cnt;
    uint32_t seed;
    uint32_t *ans;
    scanf("%u", &seed);
    ans = Mersenne_Twister(seed, 20);
    for(cnt =0 ; cnt < 20; cnt++){
        printf("%u\n", ans[cnt]);
    }
}