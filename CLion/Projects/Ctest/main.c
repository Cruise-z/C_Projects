//
// Created by 19839 on 2024/3/4.
//
#include <stdio.h>
#include <time.h>
#include <string.h>

int lengthOfLongestSubstring(char* s) {
    int len = strlen(s), cnt=0, maxcnt = 0;
    int jilu[256] = {0};
    for(int j=0; j<len; j++){
        for(int i=j; i < len; i++){
            if(jilu[s[i]] == 0){
                cnt++;
                jilu[s[i]] = 1;
            }
            else{
                if(cnt > maxcnt)
                    maxcnt = cnt;
                cnt = 0;
                memset(jilu, 0, sizeof(int)*256);
                break;
            }
        }
    }
    return maxcnt>cnt?maxcnt:cnt;
}


int main(int argc, char *argv[])
{
    char s[20];
    scanf("%s", &s);
    int len = lengthOfLongestSubstring(&s);
    printf("%d", len);
    return 0;
}

