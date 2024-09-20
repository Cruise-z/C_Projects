//
// Created by 赵睿智 on 2022/4/27.
//

#include<stdio.h>
#include<ctype.h>
int main()
{
    int i, j;
    int num = 0, temp, tempS, t;
    int K[256] = {0}, S[256] = {0}, T[256] = {0};
    int c1, c2;
    char ch1, ch2;
    char tempK[600] = {'\0'};
    scanf("%s", tempK);
    for (i = 2; isalnum(tempK[i]); i+=2) {
        c1 = isdigit(tempK[i]) ? tempK[i] - '0' : tempK[i] - 'a' + 10;
        c2 = isdigit(tempK[i + 1]) ? tempK[i + 1] - '0' : tempK[i + 1] - 'a' + 10;
        K[num++] = (c1 << 4) + c2;
    }
    for (i = 0; i < 256; i++) {
        S[i] = i;
        T[i] = K[i % num];
    }
    j = 0;
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + T[i]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
    ch1 = getchar();
    while(ch1 != '0')
        ch1 = getchar();
    ch2 = getchar();
    i = j = 0;
    printf("0x");
    while (1) {
        ch1 = getchar();
        if (!isalnum(ch1))
            break;
        ch2 = getchar();
        c1 = isdigit(ch1) ? ch1 - '0' : ch1 - 'a' + 10;
        c2 = isdigit(ch2) ? ch2 - '0' : ch2 - 'a' + 10;
        tempS = (c1 << 4) + c2;
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        t = (S[i] + S[j]) % 256;
        printf("%02x", S[t] ^ tempS);
    }
    return 0;
}