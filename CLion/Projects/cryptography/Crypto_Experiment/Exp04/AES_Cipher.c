//
// Created by 赵睿智 on 2022/3/31.
//

#include <stdio.h>
#include "../library_file/AES/AES_enc_dec.h"

int main(){
    int type, T, op;
    char s[100], k[100];
    scanf("%d\n", &type);
    //type = 128;
    scanf("%d\n", &T);
    scanf("%s\n%s\n", s, k);
    scanf("%d", &op);
    int re_block[4][4], key_table[4][60], cnt;
    str2block(&re_block, s);
    str2key(&key_table, k);
    Extend_key(&key_table, type);
    if(op == 1){
        for(cnt = 0; cnt < T; cnt++)
            round_AES(&re_block, key_table, type, 'e');
    }else if(op == 0){
        for(cnt = 0; cnt < T; cnt++)
            round_AES(&re_block, key_table, type, 'd');
    }
    output_block(re_block);
    return 0;
}
