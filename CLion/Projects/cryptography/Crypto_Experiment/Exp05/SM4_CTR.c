//
// Created by 赵睿智 on 2022/4/9.
//

#include <stdio.h>
#include <stdlib.h>
#include "../library_file/SM4/SM4_enc_dec.h"

#define LEN 2048

int main(){
    char key[35], iv_str[35];
    scanf("%s\n", key);
    scanf("%s\n", iv_str);
    int op;
    scanf("%d\n", &op);
    ui MK[4], Key_list[36], group[33][4], counter[4];
    SM4_str2key(&MK, key);
    extend_key(&Key_list, MK);
    ui list[LEN];
    int cnt = 0;
    while(scanf("%x_str", &list[cnt]) != EOF)
        cnt++;
    int row = (cnt%16==0?cnt/16:cnt/16+1), cnt1, cnt2;
    ui temp[16];
    str2group(&counter, iv_str);
    for(cnt1 = 0; cnt1 < row; cnt1++){
        for(cnt2 = 0; cnt2 < 4; cnt2++)
            group[0][cnt2] = counter[cnt2];
        SM4(&group, Key_list, 'e');
        for(cnt2 = 0; cnt2 < 4; cnt2++){
            temp[4*cnt2] = group[0][cnt2] >> 24;
            temp[4*cnt2+1] = (group[0][cnt2]&0x00ff0000) >> 16;
            temp[4*cnt2+2] = (group[0][cnt2]&0x0000ff00) >> 8;
            temp[4*cnt2+3] = (group[0][cnt2]&0x000000ff);
        }
        if(cnt1 != row-1)
            for(cnt2 = 0; cnt2 < 16; cnt2++)
                printf("0x%02x ", temp[cnt2]^list[16*cnt1+cnt2]);
        else
            for(cnt2 = 0; cnt2 < cnt-16*cnt1; cnt2++)
                printf("0x%02x ", temp[cnt2]^list[16*cnt1+cnt2]);
        printf("\n");
        CTR_counter_add(&counter, 1);
    }
    return 0;
}