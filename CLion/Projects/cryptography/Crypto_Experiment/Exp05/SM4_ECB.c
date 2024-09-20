//
// Created by 赵睿智 on 2022/4/8.
//

#include <stdio.h>
#include <stdlib.h>
#include "../library_file/SM4/SM4_enc_dec.h"

#define LEN 2048

int main(){
    char key[35];
    scanf("%s\n", key);
    int op;
    scanf("%d\n", &op);
    ui MK[4], Key_list[36], group[33][4];
    SM4_str2key(&MK, key);
    extend_key(&Key_list, MK);
    ui list[LEN];
    int cnt = 0;
    while(scanf("%x_str", &list[cnt]) != EOF)
        cnt++;
    if(op == 1){
        int row = cnt/16+1, cnt1, cnt2;
        int fill = 16*row - cnt;
        ui temp[16];
        for(cnt1 = cnt; cnt1 < 16*row; cnt1++){
            list[cnt1] = fill;
        }
        for(cnt1 = 0; cnt1 < row; cnt1++){
            group[0][0] = list[16*cnt1]*0x1000000 + list[16*cnt1+1]*0x10000 + list[16*cnt1+2]*0x100 + list[16*cnt1+3];
            group[0][1] = list[16*cnt1+4]*0x1000000 + list[16*cnt1+5]*0x10000 + list[16*cnt1+6]*0x100 + list[16*cnt1+7];
            group[0][2] = list[16*cnt1+8]*0x1000000 + list[16*cnt1+9]*0x10000 + list[16*cnt1+10]*0x100 + list[16*cnt1+11];
            group[0][3] = list[16*cnt1+12]*0x1000000 + list[16*cnt1+13]*0x10000 + list[16*cnt1+14]*0x100 + list[16*cnt1+15];
            SM4(&group, Key_list, 'e');
            for(cnt2 = 0; cnt2 < 4; cnt2++){
                temp[4*cnt2] = group[0][cnt2] >> 24;
                temp[4*cnt2+1] = (group[0][cnt2]&0x00ff0000) >> 16;
                temp[4*cnt2+2] = (group[0][cnt2]&0x0000ff00) >> 8;
                temp[4*cnt2+3] = (group[0][cnt2]&0x000000ff);
            }
            for(cnt2 = 0; cnt2 < 16; cnt2++){
                printf("0x%02x ", temp[cnt2]);
            }
            printf("\n");
        }
//        printf("%d\n", cnt);
//        for(cnt1 = 0; cnt1 < row; cnt1++){
//            for(cnt2 = 0; cnt2 < 16; cnt2++){
//                printf("%02x ", list[16*cnt1+cnt2]);
//            }
//            printf("\n");
//        }
    }else if(op == 0){
        int row = cnt/16, cnt1, cnt2;
        int fill = 16*row - cnt;
        ui temp[16];
        for(cnt1 = cnt; cnt1 < 16*row; cnt1++){
            list[cnt1] = fill;
        }
        for(cnt1 = 0; cnt1 < row; cnt1++){
            group[0][0] = list[16*cnt1]*0x1000000 + list[16*cnt1+1]*0x10000 + list[16*cnt1+2]*0x100 + list[16*cnt1+3];
            group[0][1] = list[16*cnt1+4]*0x1000000 + list[16*cnt1+5]*0x10000 + list[16*cnt1+6]*0x100 + list[16*cnt1+7];
            group[0][2] = list[16*cnt1+8]*0x1000000 + list[16*cnt1+9]*0x10000 + list[16*cnt1+10]*0x100 + list[16*cnt1+11];
            group[0][3] = list[16*cnt1+12]*0x1000000 + list[16*cnt1+13]*0x10000 + list[16*cnt1+14]*0x100 + list[16*cnt1+15];
            SM4(&group, Key_list, 'd');
            for(cnt2 = 0; cnt2 < 4; cnt2++){
                temp[4*cnt2] = group[0][cnt2] >> 24;
                temp[4*cnt2+1] = (group[0][cnt2]&0x00ff0000) >> 16;
                temp[4*cnt2+2] = (group[0][cnt2]&0x0000ff00) >> 8;
                temp[4*cnt2+3] = (group[0][cnt2]&0x000000ff);
            }
            if(cnt1 != row-1){
                for(cnt2 = 0; cnt2 < 16; cnt2++){
                    printf("0x%02x ", temp[cnt2]);
                }
                printf("\n");
            }else{
                for(cnt2 = 0; cnt2 < 16-temp[15]; cnt2++){
                    printf("0x%02x ", temp[cnt2]);
                }
            }
        }
    }
    return 0;
}