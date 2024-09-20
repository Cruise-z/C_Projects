//
// Created by 赵睿智 on 2022/4/7.
//

#include <stdio.h>
#include <stdlib.h>
#include "../library_file/SM4/SM4_enc_dec.h"

int main(){
    int T, op, cnt;
    char m[60], key[60];
    scanf("%d\n", &T);
    scanf("%s\n", m);
    scanf("%s\n", key);
    scanf("%d", &op);
    ui Mk[4], group[33][4], key_list[36];
    SM4_str2key(&Mk, key);
    str2group_block(&group, m);
    extend_key(&key_list, Mk);
    if(op == 1){
        for(cnt = 0; cnt < T; cnt++)
            SM4(&group, key_list, 'e');
        output_group(group);
    }else if(op == 0){
        for(cnt = 0; cnt < T; cnt++)
            SM4(&group, key_list, 'd');
        output_group(group);
    }
    return 0;
}