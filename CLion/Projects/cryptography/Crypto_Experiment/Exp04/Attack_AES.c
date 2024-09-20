//
// Created by 赵睿智 on 2022/4/5.
//

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../library_file/AES/AES_enc_dec.h"

int main(){
    int key_board[4][44], byte_temp[4][4][256] = {0}, correct_cipher_block[4][4], cnt;
    char message[35], cipher[35], wc[LEN][35];
    wc_inf wrong_cipher[4][4];
    scanf("%s", message);
    scanf("%s", cipher);
    str2block(&correct_cipher_block, cipher);
    for(cnt = 0; cnt < 160; cnt++){
        scanf("%s", wc[cnt]);
    }
    int r, c, cnt1;
    for(r = 0; r < 4; r++){
        for(c = 0; c < 4; c++){
            for(cnt1 = 0; cnt1 < 10; cnt1++)
                get_prob_byte(&byte_temp, correct_cipher_block, r, c, wc[(4 * r + c) * 10 + cnt1]);
        }
    }
    for(r = 0; r < 4; r++){
        for(c = 0; c < 4; c++){
            for(cnt = 0; cnt < 256; cnt++){
                if(byte_temp[r][c][cnt] == 40)
                    key_board[r][c+40] = cnt;
            }
        }
    }
    inv_Extend_key_128(&key_board);
    printf("0x");
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            printf("%02x", key_board[j][i]);
    return 0;
}

