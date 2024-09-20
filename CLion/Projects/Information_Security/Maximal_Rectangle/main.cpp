#include <iostream>
#include <vector>

typedef struct{
    int lr_len;
    int ud_len;
}st;

int main() {

}

int Maxmal_Rec(std::vector<std::vector<char>>& matrix){
    if(matrix.empty()||matrix[0].empty())
        return 0;
    int m = matrix.size(), n = matrix[0].size(), len1, len2;
    int dp[m+1][n+1][2] = {0};

}