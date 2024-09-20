//
// Created by 赵睿智 on 2022/3/17.
//

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../library_file/My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../library_file/My_Matrix_cal/My_Matrix_cal.h"
#include "../library_file/Classical_Cipher/Classical_Cipher.h"

#define len 2048

int main(){
    char s[len];
    mono_sub_key key[26];
    for(int i = 0; i < 26; i++)
        scanf("%c", &key[i].m);
    rewind(stdin);
    for(int i = 0; i < 26; i++)
        scanf("%c", &key[i].c);
    rewind(stdin);
    scanf("%s", s);    rewind(stdin);
    int op;
    scanf("%d", &op);   rewind(stdin);
    if(op == 1){
        char *re = Mono_substitution(key, s, 26, 'e');
        printf("%s", re);
    }else if(op == 0){
        char *re = Mono_substitution(key, s, 26, 'd');
        printf("%s", re);
    }
    return 0;
}