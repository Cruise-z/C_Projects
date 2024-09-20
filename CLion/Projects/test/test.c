//
// Created by 19839 on 2024/3/4.
//
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#define PackageLEN 15
uint8_t buffer[PackageLEN];
uint8_t map[128]={
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x70, 0x5a, 0x88, 0x5c, 0x5d, 0x5e, 0x68, 0x80,
        0x6a, 0x6b, 0x69, 0x7a, 0x82, 0x71, 0x83, 0x84,
        0x63, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x60,
        0x61, 0x62, 0x7f, 0x77, 0x8a, 0x72, 0x8b, 0x8c,
        0x5b, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
        0x2f, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
        0x3f, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,
        0x4f, 0x58, 0x59, 0x73, 0x75, 0x74, 0x5f, 0x79,
        0x81, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
        0x27, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
        0x37, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x50, 0x51, 0x7b, 0x7d, 0x7c, 0x89, 0xb0,
};

void Get_Descriptor(uint8_t buffer[PackageLEN], uint8_t ascii){
    memset(buffer, 0x00, sizeof(uint8_t)*PackageLEN);
    buffer[(uint8_t)(ascii>>4)] |= 1<<((uint8_t)(ascii&0x07)-1);
    if((ascii&0x08) == 8)
        buffer[0] |= 0x02;
}

int pow10(int j){
    int ans = 1;
    for(int cnt = 0; cnt < j; cnt++){
        ans *= 10;
    }
    return ans;
}

int len(int i){
    int cnt;
    while(i != 0){
        i = i/10;
        cnt++;
    }
    return cnt;
}

bool is(int x, int length){
    int rlen = len(x);
    if(rlen != length)
        return 0;
    else{
        if(rlen == 1)
            return 1;
        else if(rlen == 2)
            return (x%10) == (x/10);
        else
            return ((x/pow10(rlen-1))==(x%10)) && is((x%pow10(rlen-1))/10, length-2);
    }
}

bool isPalindrome(int x) {
    if(x < 0){
        return 0;
    }else if(x == 0){
        return 1;
    }else{
        int cnt = len(x);
        if(cnt == 1)
            return 1;
        else if(cnt == 2)
            return (x%10) == (x/10);
        else{
            int a = (x/pow10(cnt-1))==(x%10);
            int b = is((x%pow10(cnt-1))/10, cnt-2);
            return a&&b;
        }
    }
}

char* longestCommonPrefix(char** strs, int strsSize) {
    char ans[200];
    int strcnt, cnt, flag = 0;
    for(cnt = 0; strs[0][cnt] != '\0'; cnt++){
        flag = 1;
        for(strcnt = 1; strcnt < strsSize; strcnt++){
            if(strs[strcnt][cnt] != strs[0][cnt])
                flag = 0;
        }
        if(flag == 1)
            ans[cnt] = strs[0][cnt];
        else{
            ans[cnt] = '\0';
            return ans;
        }
    }
    ans[cnt] = '\0';
    return ans;
}

int removeElement(int* nums, int numsSize, int val) {
    int pos = 0, cnt, real = 0;
    while(pos < numsSize){
        cnt = pos;
        while(nums[cnt] == val && cnt < numsSize)
            cnt++;
        while(nums[cnt] != val && cnt < numsSize){
            nums[real++] = nums[cnt++];
        }
        pos = cnt;
    }
    return real;
}

void getnextval(char* nextval, char* needle){
    int nlen = strlen(needle);
    nextval[0] = -1;
    int i = 0, j = -1;
    while(i < nlen -1){
        if(j == -1 || needle[i] == needle[j]){
            i++; j++;
            if(needle[i] != needle[j])
                nextval[i] = j;
            else
                nextval[i] = nextval[j];
        }else{
            j = nextval[j];
        }
    }
}

int strStr(char* haystack, char* needle) {
    int hlen = strlen(haystack), nlen = strlen(needle);
    int *next = (int*)malloc(sizeof(int)*nlen);
    getnextval(next, needle);
    int i = 0, j = 0;
    while(i < hlen && j < nlen){
        if(j == -1 || haystack[i] == needle[j]){
            i++; j++;
        }else{
            j = next[j];
        }
    }
    free(next);
    if(j == nlen)
        return i-j;
    else
        return -1;
}

int main(int argc, char *argv[])
{
    char str[200], needle[200];
    strcpy(str, "leetcode");
    strcpy(needle, "leeto");
    int ans = strStr(str, needle);
    printf("%d ", ans);
    return 0;
}

