//
// Created by 赵睿智 on 2022/4/21.
//

#include <stdio.h>
#include <stdlib.h>
#include "../library_file/Stream_Cipher/Stream_Cipher.h"

int main(){
    char key_str[35];
    char IV_str[35];
    int round;
    scanf("%s\n", key_str);
    scanf("%s\n", IV_str);
    scanf("%d", &round);
    ZUC(key_str, IV_str, round);
    return 0;
}