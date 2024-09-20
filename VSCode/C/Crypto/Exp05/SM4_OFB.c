//
// Created by 赵睿智 on 2022/4/10.
//

#include <stdio.h>
#include <stdlib.h>
#include "../../.lib/SM4/SM4_enc_dec.h"

#define LEN 100000

int main(){
    int digit;
    scanf("%d\n", &digit);
    char key[35], iv_str[35];
    scanf("%s\n", key);
    scanf("%s\n", iv_str);
    int op;
    scanf("%d\n", &op);
    ui MK[4], Key_list[36], group[33][4], iv_temp[4];
    SM4_str2key(&MK, key);
    extend_key(&Key_list, MK);
    ui list[LEN];
    int cnt = 0;
    while(scanf("%x_str", &list[cnt]) != EOF)
        cnt++;
    long long int row = (cnt%digit==0?cnt/digit:cnt/digit+1), cnt1, cnt2;
    ui temp[16];
    str2group(&iv_temp, iv_str);
    for(cnt1 = 0; cnt1 < row; cnt1++){
        for(cnt2 = 0; cnt2 < 4; cnt2++){
            group[0][cnt2] = iv_temp[cnt2];
        }
        SM4(&group, Key_list, 'e');
        for(cnt2 = 0; cnt2 < 4; cnt2++){
            temp[4*cnt2] = group[0][cnt2] >> 24;
            temp[4*cnt2+1] = (group[0][cnt2]&0x00ff0000) >> 16;
            temp[4*cnt2+2] = (group[0][cnt2]&0x0000ff00) >> 8;
            temp[4*cnt2+3] = (group[0][cnt2]&0x000000ff);
        }
        if(cnt1 != row-1)
            for(cnt2 = 0; cnt2 < digit; cnt2++)
                printf("0x%02x ", temp[cnt2]^list[digit*cnt1+cnt2]);
        else
            for(cnt2 = 0; cnt2 < cnt-digit*cnt1; cnt2++)
                printf("0x%02x ", temp[cnt2]^list[digit*cnt1+cnt2]);
        printf("\n");
//        if(cnt1 == 2){
//            temp[3] ^= 0x89;
//        }
        OFB_lshift_fill(&iv_temp, digit, temp);
    }
    return 0;
}