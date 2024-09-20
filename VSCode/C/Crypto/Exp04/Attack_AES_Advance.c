//
// Created by 赵睿智 on 2022/5/11.
//

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../../.lib/AES/AES_enc_dec.h"

int main(){
    int key_board[4][44], correct_cipher_block[4][4], r, c, cnt;
    char message[35], cipher[35];
    wc_inf wrong_cipher[4][4];
    scanf("%s", message);
    scanf("%s", cipher);
    str2block(&correct_cipher_block, cipher);
    for(r = 0; r < 4; r++){
        for(c = 0; c < 4; c++){
            for(cnt = 0; cnt < 10; cnt++)
                scanf("%s", wrong_cipher[r][c].cipher[cnt]);
            wrong_cipher[r][c].num = cnt;
        }
    }
    get_key_128(&key_board, correct_cipher_block, wrong_cipher, 'D');
    printf("0x");
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            printf("%02x", key_board[j][i]);
    return 0;
}