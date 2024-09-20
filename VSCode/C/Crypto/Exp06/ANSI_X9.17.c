//
// Created by 赵睿智 on 2022/4/27.
//

#include<stdio.h>

int PC_1[56]=
        {
                57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
                10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
                63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
                14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
        };
int PC_2[48]=
        {
                14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
                23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
                41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
                44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
        };
int IP[64]=
        {
                58, 50, 42, 34, 26, 18, 10,  2, 60, 52, 44, 36, 28, 20, 12,  4,
                62, 54, 46, 38, 30, 22, 14,  6, 64, 56, 48, 40, 32, 24, 16,  8,
                57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11,  3,
                61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15,  7
        };
int C_IP[64]=
        {
                40,  8, 48, 16, 56, 24, 64, 32, 39,  7, 47, 15, 55, 23, 63, 31,
                38,  6, 46, 14, 54, 22, 62, 30, 37,  5, 45, 13, 53, 21, 61, 29,
                36,  4, 44, 12, 52, 20, 60, 28, 35,  3, 43, 11, 51, 19, 59, 27,
                34,  2, 42, 10, 50, 18, 58, 26, 33,  1, 41,  9, 49, 17, 57, 25
        };
int E[48]=
        {
                32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
                8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
                16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
                24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
        };
int P[32]=
        {
                16,  7, 20, 21, 29, 12, 28, 17,  1, 15, 23, 26,  5, 18, 31, 10,
                2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6, 22, 11,  4, 25
        };
int S_Box[8][4][16]=
        {
                {
                        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
                        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
                        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
                        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
                },
                {
                        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
                        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
                        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
                        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
                },
                {
                        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
                        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
                        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
                        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
                },
                {
                        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
                        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
                        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
                        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
                },
                {
                        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
                        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
                        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
                        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
                },
                {
                        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
                        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
                        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
                        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
                },
                {
                        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
                        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
                        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
                        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
                },
                {
                        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
                        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
                        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
                        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
                }
        };
int left_shift_table[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
char D2H[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
int L[32],R[32],SK[16][48], SK2[16][48];
char tempM[20],tempK[20], tempK2[20];
int M[64], Key[64], Key2[64], temp[32], out[64], IV[64], R_out[64];
char cipher[20];
char DT[250][20];

void myF(int );
void generate_key_k1();
void generate_key_k2();
void encryption(int ,int [][48]);
void decryption(int ,int [][48]);
void TDES(){
    int i,j,ans;
    encryption(1, SK);
    for (i = 2; i < 18; i++) {
        if ('0' <= cipher[i] && cipher[i] <= '9')
            ans = cipher[i] - '0';
        else
            ans = cipher[i] - 'a' + 10;
        for (j = 3; j >= 0; j--)
            M[(i - 2) * 4 + 3 - j]= ((ans >> j) & 1);
    }
    decryption(1, SK2);
    for (i = 2; i < 18; i++) {
        if ('0' <= cipher[i] && cipher[i] <= '9')
            ans = cipher[i] - '0';
        else
            ans = cipher[i] - 'a' + 10;
        for (j = 3; j >= 0; j--)
            M[(i - 2) * 4 + 3 - j]= ((ans >> j) & 1);
    }
    encryption(1, SK);
}


int main()
{
    int i, j, n;
    int ans;
    char iv[20];
    scanf("%s\n", iv);
    scanf("%s\n", tempK);
    scanf("%s\n", tempK2);
    scanf("%d\n", &n);
    for(i=0;i<n;i++){
        scanf("%s\n", DT[i]);
    }
    for (i = 2; i < 18; i++) {
        if ('0' <= iv[i] && iv[i] <= '9')
            ans = iv[i] - '0';
        else
            ans = iv[i] - 'a' + 10;
        for (j = 3; j >= 0; j--)
            IV[(i - 2) * 4 + 3 - j]= ((ans >> j) & 1);
    }
    for (i = 2; i < 18; i++) {
        if ('0' <= tempK[i] && tempK[i] <= '9')
            ans = tempK[i] - '0';
        else
            ans = tempK[i] - 'a' + 10;
        for (j = 3; j >= 0; j--)
            Key[(i - 2) * 4 + 3 - j] = ((ans >> j) & 1);
    }
    for (i = 2; i < 18; i++) {
        if ('0' <= tempK2[i] && tempK2[i] <= '9')
            ans = tempK2[i] - '0';
        else
            ans = tempK2[i] - 'a' + 10;
        for (j = 3; j >= 0; j--)
            Key2[(i - 2) * 4 + 3 - j] = ((ans >> j) & 1);
    }
    generate_key_k1();
    generate_key_k2();
    for(int t=0;t<n;t++){
        for (i = 2; i < 18; i++) {
            if ('0' <= DT[t][i] && DT[t][i] <= '9')
                ans = DT[t][i] - '0';
            else
                ans = DT[t][i] - 'a' + 10;
            for (j = 3; j >= 0; j--)
                M[(i - 2) * 4 + 3 - j]= ((ans >> j) & 1);
        }
        TDES();
        for (i = 2; i < 18; i++) {
            if ('0' <= cipher[i] && cipher[i] <= '9')
                ans = cipher[i] - '0';
            else
                ans = cipher[i] - 'a' + 10;
            for (j = 3; j >= 0; j--)
                M[(i - 2) * 4 + 3 - j]= ((ans >> j) & 1);
        }
        for(i=0;i<64;i++){
            M[i] = M[i] ^ IV[i];
        }
        TDES();
        printf("%s\n", cipher);
        for (i = 2; i < 18; i++) {
            if ('0' <= cipher[i] && cipher[i] <= '9')
                ans = cipher[i] - '0';
            else
                ans = cipher[i] - 'a' + 10;
            for (j = 3; j >= 0; j--)
                R_out[(i - 2) * 4 + 3 - j]= ((ans >> j) & 1);
        }
        for (i = 2; i < 18; i++) {
            if ('0' <= DT[t][i] && DT[t][i] <= '9')
                ans = DT[t][i] - '0';
            else
                ans = DT[t][i] - 'a' + 10;
            for (j = 3; j >= 0; j--)
                M[(i - 2) * 4 + 3 - j]= ((ans >> j) & 1);
        }
        TDES();
        for (i = 2; i < 18; i++) {
            if ('0' <= cipher[i] && cipher[i] <= '9')
                ans = cipher[i] - '0';
            else
                ans = cipher[i] - 'a' + 10;
            for (j = 3; j >= 0; j--)
                M[(i - 2) * 4 + 3 - j]= ((ans >> j) & 1);
        }
        for(i=0;i<64;i++){
            M[i] = M[i] ^ R_out[i];
        }
        TDES();
        for (i = 2; i < 18; i++) {
            if ('0' <= cipher[i] && cipher[i] <= '9')
                ans = cipher[i] - '0';
            else
                ans = cipher[i] - 'a' + 10;
            for (j = 3; j >= 0; j--)
                IV[(i - 2) * 4 + 3 - j]= ((ans >> j) & 1);
        }
    }
    return 0;
}

void generate_key_k1(){
    int i, j;
    //瀵嗛挜鐢熸垚閮ㄥ垎
    int key_left[28], key_right[28], temp_l[28], temp_r[28];
    //鍒嗕负宸﹀彸涓ら儴鍒?
    for (i = 0; i < 28; i++)
        key_left[i] = Key[PC_1[i] - 1];
    for (i = 0; i < 28; i++)
        key_right[i] = Key[PC_1[i + 28] - 1];
    for(i=0; i<16; i++) {
        for (j = left_shift_table[i]; j < 28; j++)
            temp_l[j - left_shift_table[i]] = key_left[j];
        for (j = 28 - left_shift_table[i]; j < 28; j++)
            temp_l[j] = key_left[j + left_shift_table[i]- 28];
        for (j = left_shift_table[i]; j < 28; j++)
            temp_r[j - left_shift_table[i]] = key_right[j];
        for (j = 28 - left_shift_table[i]; j < 28; j++)
            temp_r[j] = key_right[j +  left_shift_table[i]- 28];
        for (j = 0; j < 28; j++) {
            key_left[j] = temp_l[j];
            key_right[j] = temp_r[j];
        }
        for (j = 0; j < 48; j++) {
            if (PC_2[j] <= 28)
                SK[i][j] = key_left[PC_2[j]-1];
            else
                SK[i][j] = key_right[PC_2[j]-29];
        }
    }
}


void generate_key_k2(){
    int i, j;
    int key_left[28], key_right[28], temp_l[28], temp_r[28];
    for (i = 0; i < 28; i++)
        key_left[i] = Key2[PC_1[i] - 1];
    for (i = 0; i < 28; i++)
        key_right[i] = Key2[PC_1[i + 28] - 1];
    for(i=0; i<16; i++) {
        for (j = left_shift_table[i]; j < 28; j++)
            temp_l[j - left_shift_table[i]] = key_left[j];
        for (j = 28 - left_shift_table[i]; j < 28; j++)
            temp_l[j] = key_left[j + left_shift_table[i]- 28];
        for (j = left_shift_table[i]; j < 28; j++)
            temp_r[j - left_shift_table[i]] = key_right[j];
        for (j = 28 - left_shift_table[i]; j < 28; j++)
            temp_r[j] = key_right[j +  left_shift_table[i]- 28];
        for (j = 0; j < 28; j++) {
            key_left[j] = temp_l[j];
            key_right[j] = temp_r[j];
        }
        for (j = 0; j < 48; j++) {
            if (PC_2[j] <= 28)
                SK2[i][j] = key_left[PC_2[j]-1];
            else
                SK2[i][j] = key_right[PC_2[j]-29];
        }
    }
}
void encryption(int n, int K[][48]){
    int cnt1, cnt2,i,j, k, ans;
    int row, col;
    int out1[48], out2[48], out3[32], out4[32];
    for (i = 0; i < n; i++) {
        for (j = 0; j < 32; j++)
            L[j] = M[IP[j] - 1];
        for (j = 0; j < 32; j++)
            R[j] = M[IP[j + 32] - 1];
        for(j = 0; j < 16; j++) {
            for (k = 0; k < 32; k++)
                temp[k] = R[k];
            for(cnt1 = 0; cnt1 < 48; cnt1++)
                out1[cnt1] = R[E[cnt1] - 1];
            for(cnt1 = 0; cnt1 < 48; cnt1++)
                out2[cnt1] = out1[cnt1] ^ K[j][cnt1];
            for(cnt1 = 0; cnt1 < 8; cnt1++) {
                row = (out2[cnt1*6] << 1) + out2[cnt1* 6+5];
                col = (out2[cnt1*6+1] <<3 ) + (out2[cnt1*6+2] << 2) + (out2[cnt1*6+3] << 1) + out2[cnt1*6+4];
                for(cnt2=3; cnt2>=0; cnt2--)
                    out3[cnt1*4+3-cnt2] = (S_Box[cnt1][row][col]>>cnt2)&1;
            }
            for(cnt1 = 0; cnt1 < 32; cnt1++)
                out4[cnt1] = out3[P[cnt1] - 1];
            for(cnt1 = 0; cnt1 < 32; cnt1++)
                R[cnt1] = L[cnt1] ^ out4[cnt1];

            for (k = 0; k < 32; k++)
                L[k] = temp[k];
        }
        for (j = 0; j < 32; j++)
            out[j] = R[j];
        for (j = 0; j < 32; j++)
            out[j + 32] = L[j];
        for (j = 0; j < 64; j++)
            M[j] = out[C_IP[j] - 1];
    }
    cipher[0] = '0';
    cipher[1] = 'x';
    k = 2;
    for (i = 0,ans=0,j=3; i < 64; i++, j--) {
        ans += M[i] << j;
        if (j == 0) {
            j = 4;
            cipher[k++] = D2H[ans];
            ans = 0;
        }
    }
    cipher[k] = '\0';
}

void decryption(int n, int K[][48]){
    int cnt1, cnt2,i,j, k, ans;
    int row, col;
    int out1[48], out2[48], out3[32], out4[32];
    for (i = 0; i < n; i++) {
        for (j = 0; j < 32; j++)
            L[j] = M[IP[j] - 1];
        for (j = 0; j < 32; j++)
            R[j] = M[IP[j + 32] - 1];
        for(j = 0; j < 16; j++) {
            for (k = 0; k < 32; k++)
                temp[k] = R[k];
            for(cnt1 = 0; cnt1 < 48; cnt1++)
                out1[cnt1] = R[E[cnt1] - 1];
            for(cnt1 = 0; cnt1 < 48; cnt1++)
                out2[cnt1] = out1[cnt1] ^ K[15-j][cnt1];
            for(cnt1 = 0; cnt1 < 8; cnt1++) {
                row = (out2[cnt1*6] << 1) + out2[cnt1* 6+5];
                col = (out2[cnt1*6+1] <<3 ) + (out2[cnt1*6+2] << 2) + (out2[cnt1*6+3] << 1) + out2[cnt1*6+4];
                for(cnt2=3; cnt2>=0; cnt2--)
                    out3[cnt1*4+3-cnt2] = (S_Box[cnt1][row][col]>>cnt2)&1;
            }
            for(cnt1 = 0; cnt1 < 32; cnt1++)
                out4[cnt1] = out3[P[cnt1] - 1];
            for(cnt1 = 0; cnt1 < 32; cnt1++)
                R[cnt1] = L[cnt1] ^ out4[cnt1];

            for (k = 0; k < 32; k++)
                L[k] = temp[k];
        }
        for (j = 0; j < 32; j++)
            out[j] = R[j];
        for (j = 0; j < 32; j++)
            out[j + 32] = L[j];
        for (j = 0; j < 64; j++)
            M[j] = out[C_IP[j] - 1];
    }
    cipher[0] = '0';
    cipher[1] = 'x';
    k = 2;
    for (i = 0,ans=0,j=3; i < 64; i++, j--) {
        ans += M[i] << j;
        if (j == 0) {
            j = 4;
            cipher[k++] = D2H[ans];
            ans = 0;
        }
    }
    cipher[k] = '\0';
}