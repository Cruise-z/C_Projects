//
// Created by 赵睿智 on 2022/3/16.
//

#include "library.h"
#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define LEN 2048
#define maxlen 1000
#define maxpfx 10
#define maxsub 10


int gcd(int a, int b)
{
    if (a%b == 0)
        return b;
    else
        return gcd(b, a%b);
}

char xor(char c1, char c2)
{
    return (char)(((c1-'0')^(c2-'0'))+'0');
}

char *standard(char *binary_s)
{
    mpz_t s;
    mpz_init_set_str(s, binary_s, 2);
    char *ans = NULL;
    ans = mpz_get_str(ans, 2, s);
    return ans;
}

char *GF_2_xor(char *str1,char *str2)
{
    //对输入的二进制字符串标准化操作
    str1 = standard(str1);
    str2 = standard(str2);
    int len1 = (int)strlen(str1), len2 = (int)strlen(str2), cnt1,cnt2;
    int cnt = (len1>len2?len1:len2);
    char *re = malloc(sizeof(char)*(cnt+1));
    re[cnt] = '\0';
    cnt--;
    for(cnt1 = len1-1,cnt2 = len2-1;cnt1>=0&&cnt2>=0;cnt1--,cnt2--)
        re[cnt--] = xor(str1[cnt1],str2[cnt2]);
    if(cnt1 == -1)
        while(cnt2>=0)
            re[cnt--] = str2[cnt2--];
    else
        while(cnt1>=0)
            re[cnt--] = str1[cnt1--];
    //对输出标准化
    re = standard(re);
    return re;
}

//计算完成后返回商字符串的首地址，被除字符串变为余数字符串
mpz_div_re GF_2_poly_division(char *binary_dividend, char *binary_divisor)
{
    //对输入的二进制字符串标准化操作
    binary_dividend = standard(binary_dividend);
    binary_divisor = standard(binary_divisor);
    //设置结构体承载输出变量
    mpz_div_re result;
    int degree1 = (int)strlen(binary_dividend)-1, degree2 = (int)strlen(binary_divisor)-1;
    //使用binary_dividend_re操作，防止改变binary_dividend
    char *binary_dividend_re  = malloc(sizeof(char)*LEN);
    strcpy(binary_dividend_re, binary_dividend);
    if(degree1 < degree2){
        char *none = malloc(sizeof(char)*LEN);
        none[0] = '0';  none[1] = '\0';
        result.ans = none;
        result.remain = binary_dividend_re;
        return result;
    }else{
        char *ans_str = malloc(sizeof(char) * (degree1 - degree2 + 2));
        memset(ans_str, '0', degree1 - degree2 + 2);
        ans_str[degree1 - degree2 + 1] = '\0';
        int anscnt = 0, cnt1 = 0, cnt2 = 0;
        for (; cnt1 <= degree1 - degree2; cnt1++) {
            ans_str[anscnt] = (char) ((binary_dividend_re[cnt1] - '0') / (binary_divisor[0] - '0') + '0');
            if (ans_str[anscnt] == '1')
                for (cnt2 = 0; cnt2 < degree2 + 1; cnt2++)
                    binary_dividend_re[cnt2 + cnt1] = xor(binary_dividend_re[cnt2 + cnt1], binary_divisor[cnt2]);
            anscnt++;
        }
        //对输出标准化
        ans_str = standard(ans_str);
        binary_dividend_re = standard(binary_dividend_re);
        result.ans= ans_str;
        result.remain = binary_dividend_re;
        return result;
    }
}

char *GF_2_poly_multiplication(char *binary_s1, char *binary_s2) {
    //标准化输入
    binary_s1 = standard(binary_s1);
    binary_s2 = standard(binary_s2);
    int digit1 = (int)strlen(binary_s1), digit2 = (int)strlen(binary_s2), cnt1, cnt2, time = 1;
    char *ans = malloc(sizeof(char) * (digit1 + digit2));
    memset(ans, '0', digit1 + digit2);
    ans[digit1 + digit2 - 1] = '\0';
    for (cnt1 = digit2 - 1; cnt1 >= 0; cnt1--, time++)
        if (binary_s2[cnt1] == '1')
            for (cnt2 = digit1 - 1; cnt2 >= 0; cnt2--)
                ans[cnt2 + digit2 - time] = xor(ans[cnt2 + digit2 - time], binary_s1[cnt2]);
    //标准化输出
    ans = standard(ans);
    return ans;
}

char *GF_2_add_sub(char *binary_s1, char *binary_s2, char *binary_modstr)
{
    mpz_div_re result;
    //binary_modstr是域上的不可约多项式系数的二进制表示
    char *flag_str = NULL;
    flag_str = GF_2_xor(binary_s1, binary_s2);
    result = GF_2_poly_division(flag_str, binary_modstr);
    return result.remain;
}

char *GF_2_mul(char *binary_s1, char *binary_s2, char *binary_modstr)
{
    mpz_div_re result;
    //binary_modstr是域上的不可约多项式系数的二进制表示
    char *flag_str;
    flag_str = GF_2_poly_multiplication(binary_s1, binary_s2);
    result = GF_2_poly_division(flag_str,binary_modstr);
    return result.remain;
}

mpz_div_re GF_2_hex_cal(char op, char *hex_s1, char *hex_s2, char *hex_modstr)
{
    mpz_t s1,s2,mod,ans;
    mpz_init_set_str(s1, hex_s1, 16);
    mpz_init_set_str(s2, hex_s2, 16);
    mpz_init_set_str(mod, hex_modstr, 16);
    char *bin_s1 = NULL, *bin_s2 = NULL, *bin_modstr = NULL;// *hex_ans = NULL;
    mpz_div_re result;
    result.ans = NULL;    result.remain = NULL;
    bin_s1 = mpz_get_str(bin_s1, 2, s1);
    bin_s2 = mpz_get_str(bin_s2, 2, s2);
    bin_modstr = mpz_get_str(bin_modstr, 2, mod);
    if(op == '+' || op == '-')
        result.ans = GF_2_add_sub(bin_s1, bin_s2, bin_modstr);
    else if(op == '*')
        result.ans = GF_2_mul(bin_s1, bin_s2, bin_modstr);
    else if(op == '/')
    {
        result = GF_2_poly_division(bin_s1, bin_s2);
        mpz_init_set_str(mod,result.remain,2);
        result.remain = mpz_get_str(result.remain, 16, mod);
    }
    mpz_init_set_str(ans, result.ans, 2);
    result.ans = mpz_get_str(result.ans, 16, ans);
    return result;
}

char *GF_2_Fp(char *binary_b, char *binary_p, char *binary_modstr)
{
    //标准化输入
    binary_b = standard(binary_b);
    binary_p = standard(binary_p);
    binary_modstr = standard(binary_modstr);
    char *ans = malloc(sizeof(char)*LEN);
    char *temp = malloc(sizeof(char)*LEN);
    int p_digit = (int)strlen(binary_p), cnt;
    strcpy(temp, binary_b);     //先给temp赋binary_b的值
    binary_p[p_digit-1] == '1'? strcpy(ans, binary_b):strcpy(ans,"1");
    //该步是根据binary_p的最低位是否为1对ans赋相应的值，这第一步放在循环之外是为了让循环更条理
    for(cnt = p_digit-2; cnt>=0; cnt--)
    {
        temp = GF_2_mul(temp, temp, binary_modstr);
        if(binary_p[cnt] == '1')
            ans = GF_2_mul(ans, temp, binary_modstr);
    }
    ans = standard(ans);
    return ans;
}

char *GF_2_hex_Fp(char *hex_b, char *dec_p, char *hex_modstr)
{
    mpz_t b, p, mod, ans;
    mpz_init_set_str(b, hex_b, 16);
    mpz_init_set_str(p, dec_p, 10);
    mpz_init_set_str(mod, hex_modstr, 16);
    char *bin_b = NULL, *bin_p = NULL, *bin_modstr = NULL;
    bin_b = mpz_get_str(bin_b, 2, b);
    bin_p = mpz_get_str(bin_p, 2, p);
    bin_modstr = mpz_get_str(bin_modstr, 2, mod);
    char *ans_str = GF_2_Fp(bin_b, bin_p, bin_modstr);
    mpz_init_set_str(ans, ans_str, 2);
    ans_str = mpz_get_str(ans_str, 16, ans);
    return ans_str;
}

mpz_gcd_re GF_2_exEuclid(char *binary_s1, char *binary_s2, char *modstr)
{
    binary_s1 = standard(binary_s1);
    binary_s2 = standard(binary_s2);
    modstr = standard(modstr);
    mpz_gcd_re result, temp;
    if(binary_s2[0] == '0'){
        result.gcd = binary_s1;
        result.x = malloc(sizeof(char)*LEN);
        strcpy(result.x, "1");
        result.y = binary_s2;
        return result;
    }else{
        temp = GF_2_exEuclid(binary_s2, GF_2_poly_division(binary_s1, binary_s2).remain, modstr);
        result.gcd = temp.gcd;
        result.x = temp.y;
        result.y = GF_2_xor(temp.x, GF_2_mul(temp.y, GF_2_poly_division(binary_s1, binary_s2).ans, modstr));
        return result;
    }
}

mpz_gcd_re GF_2_hex_exEuclid(char *hex_s1, char *hex_s2, char *hex_modstr)
{
    mpz_t s1, s2, modstr, invu_s1, invu_s2, invu_gcd;
    mpz_init_set_str(s1, hex_s1, 16);
    mpz_init_set_str(s2, hex_s2, 16);
    mpz_init_set_str(modstr, hex_modstr, 16);
    char *bin_s1 = NULL, *bin_s2 = NULL, *bin_modstr = NULL;
    bin_s1 = mpz_get_str(bin_s1, 2, s1);
    bin_s2 = mpz_get_str(bin_s2, 2, s2);
    bin_modstr = mpz_get_str(bin_modstr, 2, modstr);
    mpz_gcd_re result = GF_2_exEuclid(bin_s1, bin_s2, bin_modstr);
    mpz_init_set_str(invu_gcd, result.gcd, 2);
    result.gcd = mpz_get_str(result.gcd, 16, invu_gcd);
    mpz_init_set_str(invu_s1, result.x, 2);
    result.x = mpz_get_str(result.x, 16, invu_s1);
    mpz_init_set_str(invu_s2, result.y, 2);
    result.y = mpz_get_str(result.y, 16, invu_s2);
    return result;
}

div_re Div(long long int pm, long long int mod){
    div_re re;
    if(pm < 0){
        re.ans = pm / mod - 1;
        re.remain = (pm % mod + mod);
    }else {
        re.ans = pm / mod;
        re.remain = pm % mod;
    }
    return re;
}

gcd_re ex_gcd(long long int a, long long int b){
    gcd_re re;
    if(b == 0){
        re.gcd = a;
        re.x = 1;
        re.y = 0;
        return re;
    }else{
        re = ex_gcd(b, Div(a, b).remain);
        long long int x = re.y;
        long long int y = re.x - Div(a, b).ans*re.y;
        re.x = x;
        re.y = y;
        return re;
    }
}

//拓展欧几里得
mpz_gcd_re str_gcdext(char *s1, char *s2)
{
    mpz_t a1,a2,inv_a1,inv_a2,ans,none;
    mpz_init_set_str(a1,s1,10);
    mpz_init_set_str(a2,s2,10);
    mpz_init(inv_a1);
    mpz_init(inv_a2);
    mpz_init(ans);
    mpz_t inv_ans1; mpz_init(inv_ans1);
    mpz_t inv_ans2; mpz_init(inv_ans2);
    mpz_init_set_si(none,0);
    mpz_gcdext(ans,inv_a1,inv_a2,a1,a2);
    mpz_div(a1, a1, ans);
    mpz_div(a2, a2, ans);
    mpz_mod(inv_ans1, inv_a1, a2);
    mpz_t flag; mpz_init(flag);
    mpz_sub(flag, inv_a1, inv_ans1);
    mpz_div(flag, flag, a2);
    mpz_mul(flag, flag, a1);
    mpz_add(inv_ans2, inv_a2, flag);
    char *inv_ans1_str = NULL, *inv_ans2_str = NULL, *ans_str = NULL;
    inv_ans1_str = mpz_get_str(inv_ans1_str, 10, inv_ans1);
    inv_ans2_str = mpz_get_str(inv_ans2_str, 10, inv_ans2);
    ans_str = mpz_get_str(ans_str, 10, ans);
    mpz_gcd_re re;
    re.x = inv_ans1_str;
    re.y = inv_ans2_str;
    re.gcd = ans_str;
    return re;
}

//厄尔拉多筛
void Eeatosthese(int num)
{
    int sqrt_num;
    //简化运算，只取小于平方根范围内的素数即可
    sqrt_num = (int)sqrt(num);
    int *prime = malloc(sizeof(int)*sqrt_num);
    prime[0] = 2;
    int cnt0 = 1;
    //初始化操作
    for (cnt0 = 1; cnt0 < sqrt_num; cnt0++)
        prime[cnt0] = 0;
    int cnt = 1;
    //找小于平方根内的素数的核心代码
    for (int x = 3; x <= sqrt_num; x++)
    {
        for (int i = 0; i < cnt&&prime[i] <= (int)sqrt(x); i++){
            if (x%prime[i] == 0)
                continue;
        }
        prime[cnt++] = x;
    }

    //建立素数表
    bool *isprime = malloc(sizeof(bool)*num);
    isprime[0] = false;
    for (int cnt1 = 1; cnt1 < num; cnt1++)
    {
        isprime[cnt1] = true;
    }
    for (int cnt2 = 0; prime[cnt2] != 0; cnt2++)
    {
        for (int k = 2; prime[cnt2] * k <= num; k++)
        {
            isprime[prime[cnt2] * k - 1] = false;
        }
    }

    //输出素数表
    int primecnt = 0;
    for (int cnt3 = 0; cnt3 < num; cnt3++)
    {
        if (isprime[cnt3] == true)
        {
            printf("%d ", cnt3 + 1);
            primecnt++;
        }
    }
    free(prime);
    free(isprime);
}

//快速幂算法
void Fp(mpz_t * B, mpz_t *P, mpz_t *M, mpz_t *ans)
{
    mpz_t b, p, m, result, none, t, ma, unit, temp;
    mpz_init(b);              mpz_init(p);           mpz_init(m);
    mpz_init(result);         mpz_init(temp);        mpz_init(ma);
    mpz_set(b, *B);           mpz_set(p, *P);        mpz_set(m, *M);
    mpz_init_set_si(none,0);  mpz_init_set_ui(t,2);  mpz_init_set_ui(unit,1);
    if(mpz_cmp(p,none)<0)
    {
        mpz_invert(b,b,m);
        mpz_neg(p,p);
    }
    mpz_mod(temp,b,m);        mpz_mod(ma,p,t);       mpz_div(p,p,t);
    mpz_cmp(ma,unit)==0? mpz_set(result, temp):mpz_set_ui(result,1);
    while(mpz_cmp(p,none)!=0)
    {
        mpz_mul(temp,temp,temp);
        mpz_mod(temp,temp,m);
        mpz_mod(ma,p,t);
        if(mpz_cmp(ma,unit)==0)
        {
            mpz_mul(result,result,temp);
            mpz_mod(result,result,m);
        }
        mpz_div(p,p,t);
    }
    mpz_set(*ans, result);
}

//中国剩余定理算法
void CRT_RSA(mpz_t * c,mpz_t * p,mpz_t * q,mpz_t * d,mpz_t * ans)
{
    mpz_t C,P,Q,D,C1,C2,R1,R2,M1,M2,P1,Q1,unit,PQ,result;
    //初始化操作
    mpz_init(C);    mpz_init(P);    mpz_init(Q);    mpz_init(D);
    mpz_set(C,*c);  mpz_set(P,*p);  mpz_set(Q,*q);  mpz_set(D,*d);
    mpz_init(C1);   mpz_init(C2);   mpz_init(R1);   mpz_init(R2);
    mpz_init(M1);   mpz_init(M2);   mpz_init(P1);   mpz_init(Q1);
    mpz_init(result);               mpz_init_set_ui(unit,1);
    mpz_init(PQ);                   mpz_mul(PQ,P,Q);        //PQ=P*Q
    mpz_mod(C1,C,P);                mpz_mod(C2,C,Q);        //C1=C%P;C2=C%Q
    mpz_sub(R1,P,unit);             mpz_sub(R2,Q,unit);     //R1=P-1;R2=Q-1
    mpz_mod(R1,D,R1);               mpz_mod(R2,D,R2);       //R1=D%(P-1);R2=D%(Q-1)
    Fp(&C1,&R1,&P,&M1);             Fp(&C2,&R2,&Q,&M2);     //M1=C1^R1%P;M2=C2^R2%Q
    mpz_invert(P1,P,Q);             mpz_invert(Q1,Q,P);     //求逆
    mpz_mul(M2,M2,P);               mpz_mul(M2,M2,P1);      //M2*P*P1赋值给M2
    mpz_mul(M1,M1,Q);               mpz_mul(M1,M1,Q1);      //M1*Q*Q1赋值给M1
    mpz_add(result,M1,M2);                                  //result=M1+M2
    mpz_mod(result,result,PQ);                              //result=(M1+M2)%PQ
    mpz_set(*ans,result);
}

//幂函数
void mpz_pow(mpz_t * B,mpz_t * P,mpz_t * ans)
{
    mpz_t b,p,result,cnt,unit;
    mpz_init_set(b,*B);
    mpz_init_set(p,*P);
    mpz_init_set_si(cnt,0);
    mpz_init_set_si(unit,1);
    mpz_init_set(result,unit);
    while(mpz_cmp(cnt,p)<0)
    {
        mpz_mul(result,result,b);
        mpz_add(cnt,cnt,unit);
    }
    mpz_set(*ans,result);
    mpz_clear(b);
    mpz_clear(p);
    mpz_clear(result);
    mpz_clear(cnt);
    mpz_clear(unit);
}

//生成start-end的随机数
void randrange(mpz_t * randnum, mpz_t start, mpz_t end)
{
    mpz_t END;
    mpz_init_set(END,end);
    while(1)
    {
        clock_t time = clock();
        gmp_randstate_t grt;
        gmp_randinit_default(grt);
        gmp_randseed_ui(grt, time);
        mpz_t A;    mpz_init(A);
        mpz_urandomm(A, grt, END);
        if (mpz_cmp(A, start) >= 0)
        {
            mpz_set(*randnum,A);
            break;
        }
    }
}

//拉宾素数检测法
bool rabin_wit(mpz_t key)
{
    mpz_t K,K_1,isK_e,unit,none,two,s,t,judge;
    mpz_t cnt,cnt1,range,g,a,x,count,step1,step2,P;
    mpz_init_set_ui(none,0);
    mpz_init_set_ui(unit,1);
    mpz_init_set_ui(two,2);
    mpz_init_set(K,key);
    mpz_init(K_1);      mpz_sub(K_1,K,unit);
    mpz_init(isK_e);    mpz_mod(isK_e,K,two);
    mpz_init(s);        mpz_sub(s,K,unit);
    mpz_init_set(t,none);
    mpz_init_set(cnt,none);
    mpz_init_set(cnt1,none);
    mpz_init_set_ui(range,20);
    mpz_init(judge);    mpz_init(g);
    mpz_init(a);        mpz_init(x);
    mpz_init(count);    mpz_init(step1);
    mpz_init(step2);    mpz_init(P);
    if(mpz_cmp(K,two)==0)
        return true;
    if(mpz_cmp(isK_e,none)==0)
        return false;
    while(1)
    {
        mpz_mod(judge,s,two);
        if(mpz_cmp(judge,none)==0)
        {
            mpz_div(s,s,two);
            mpz_add(t,t,unit);
        }
        else
            break;
    }
    while(mpz_cmp(cnt,range)<0)
    {
        mpz_set_si(g,0);
        while(mpz_cmp(g,unit)!=0)
        {
            randrange(&a,two,K_1);
            mpz_gcd(g,a,K);
        }
        Fp(&a,&s,&K,&x);
        mpz_set(count,none);
        mpz_add(step1,x,K);
        mpz_mod(step1,step1,K);
        if(mpz_cmp(step1,unit)!=0)
            mpz_add(count,count,unit);
        while(mpz_cmp(cnt1,t)<0)
        {
            mpz_pow(&two,&cnt1,&P);
            mpz_mul(P,P,s);
            Fp(&a,&P,&K,&x);
            mpz_mod(x,x,K);
            if(mpz_cmp(x,K_1)!=0)
                mpz_add(count,count,unit);
            mpz_add(step2,t,unit);
            if(mpz_cmp(count,step2)==0)
                return false;
            mpz_add(cnt1,cnt1,unit);
        }
        mpz_add(cnt,cnt,unit);
    }
    return true;
}

//得到强素数算法
void get_prime(mpz_t *prime, int size)
{
    mpz_t key,start,end,Size,unit,two,Size_1;
    mpz_init(key);
    mpz_init(start);
    mpz_init(end);
    mpz_init(Size_1);
    mpz_init_set_ui(Size,size);
    mpz_init_set_ui(unit,1);
    mpz_sub(Size_1,Size,unit);
    mpz_init_set_ui(two,2);
    mpz_pow(&two,&Size_1,&start);
    mpz_pow(&two,&Size,&end);
    do{
        randrange(&key,start,end);
    }while(rabin_wit(key)==false);
    mpz_set(*prime,key);
}

//强素数生成算法
bool get_strong_prime(mpz_t *key,int bit)
{
    mpz_t b256,b257,b_511,b_512,unit,two,p1,q1,pq_1,q2,result;
    mpz_init(b256);     mpz_init(b257);
    mpz_init(b_511);    mpz_init(b_512);
    mpz_init(p1);       mpz_init(q1);
    mpz_init(pq_1);     mpz_init(q2);
    mpz_init(result);
    mpz_init_set_ui(unit,1);    mpz_init_set_ui(two,2);
    get_prime(&p1,256);
    //随机生成256位的素数p1
    mpz_pow_ui(b256,two,256);           mpz_pow_ui(b257,two,257);
    mpz_pow_ui(b_512,two,bit-512);      mpz_pow_ui(b_511,two,bit-511);
    do{
        randrange(&q1,b256,b257);
        //随机生成256位的数q1
        mpz_mul(pq_1,p1,q1);
        mpz_add(pq_1,pq_1,unit);
        //p1与q1相乘再加1得pq_1并判断其是否为素数
    }while(rabin_wit(pq_1)==false);
    do{
        randrange(&q2,b_512,b_511);
        //随机生成bit-512位的数q2
        mpz_mul(result,q2,pq_1);
        mpz_add(result,result,unit);
        //q2与pq_1相乘再加1得ans并判断其是否为素数
    }while(rabin_wit(result)==false);
    //这样得到得素数有一定概率满足ans+1有大的素数因子
    mpz_set(*key,result);
    return true;
}

//求逆函数(虽然比mpz_invert慢了亿点点)
//rea是N模M的逆，reb是M模N的逆
void inversion(mpz_t N, mpz_t M, mpz_t *rea, mpz_t *reb)
{
    mpz_t unit;     mpz_init_set_si(unit,1);
    mpz_t ANS;      mpz_init_set_si(ANS,1);
    mpz_t none;     mpz_init_set_si(none,0);
    mpz_t step1,step2,step3;
    mpz_init(step1);    mpz_init(step2);    mpz_init(step3);
    while(1)
    {
        mpz_mul(step1,N,ANS);
        mpz_sub(step2,unit,step1);
        mpz_mod(step3,step2,M);
        if(mpz_cmp(step3,none)==0)
            break;
        mpz_add(ANS,ANS,unit);
    }
    mpz_set(*rea,ANS);
    mpz_mul(step1,N,*rea);
    mpz_sub(step2,unit,step1);
    mpz_div(step3,step2,M);
    mpz_set(*reb,step3);
}

unsigned char *base64_encode(unsigned char *str)
{
    long len;
    long str_len;
    unsigned char *res;
    int i,j;
    //定义base64编码表
    unsigned char *base64_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //计算经过base64编码后的字符串长度
    str_len = (int)strlen(str);
    if(str_len % 3 == 0)
        len=str_len/3*4;
    else
        len=(str_len/3+1)*4;
    res=malloc(sizeof(unsigned char)*len+1);
    res[len]='\0';
    //以3个8位字符为一组进行编码
    for(i=0,j=0;i<len-2;j+=3,i+=4)
    {
        res[i]=base64_table[str[j]>>2];
        //取出第一个字符的前6位并找出对应的结果字符
        res[i+1]=base64_table[(str[j]&0x3)<<4|(str[j+1]>>4)];
        //将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符
        res[i+2]=base64_table[(str[j+1]&0xf)<<2|(str[j+2]>>6)];
        //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
        res[i+3]=base64_table[str[j+2]&0x3f];
        //取出第三个字符的后6位并找出结果字符
    }
    switch(str_len % 3)
    {
        case 1:
            res[i-2]='=';
            res[i-1]='=';
            break;
        case 2:
            res[i-1]='=';
            break;
    }
    return res;
}

unsigned char *base64_decode(unsigned char *code)
{
//根据base64表，以字符找到对应的十进制数据
    int table[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0,
                   63, 52, 53, 54, 55, 56, 57, 58,
                   59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0,
                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                   13, 14, 15, 16, 17, 18, 19, 20, 21,
                   22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26,
                   27, 28, 29, 30, 31, 32, 33, 34, 35,
                   36, 37, 38, 39, 40, 41, 42, 43, 44,
                   45, 46, 47, 48, 49, 50, 51
    };
    long len;
    long str_len;
    unsigned char *res;
    int i, j;
    //计算解码后的字符串长度
    len = (int)strlen(code);
    //判断编码后的字符串后是否有=
    if (strstr(code, "=="))
        str_len = len / 4 * 3 - 2;
    else if (strstr(code, "="))
        str_len = len / 4 * 3 - 1;
    else
        str_len = len / 4 * 3;
    res = malloc(sizeof(unsigned char) * str_len + 1);
    res[str_len] = '\0';
    //以4个字符为一位进行解码
    for (i = 0, j = 0; i < len - 2; j += 3, i += 4)
    {
        res[j]=((unsigned char) table[code[i]])<<2|(((unsigned char)table[code[i+1]])>>4);
        //取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的后2位进行组合
        res[j+1]=(((unsigned char)table[code[i+1]])<<4)|(((unsigned char)table[code[i+2]])>>2);
        //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合
        res[j+2]=(((unsigned char)table[code[i+2]])<<6)|((unsigned char)table[code[i+3]]);
        //取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合
    }

    return res;
}

//查了好多资料还是不会用c将hex转utf-8,只能用这个凑数了...
void to_utf8(char *tar)
{
    int len = (int)strlen(tar),cnt;
    for(cnt=len-1;cnt>=0;cnt--)
        tar[cnt+2]=tar[cnt];
    tar[0]='b';
    tar[1]='\'';
    tar[len+2]='\'';
    tar[len+3]='\0';
}
