//
// Created by 赵睿智 on 2022/4/17.
//

#include<stdio.h>

int main(){
    FILE *fp = fopen("./pic_original.bmp", "r");
    int c;
    while((c = fgetc(fp)) != EOF){
        printf("%02x ", c);
    }
    return 0;
}