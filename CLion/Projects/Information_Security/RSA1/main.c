#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>

#define Len 5

void Fp(mpz_t * B, mpz_t *P, mpz_t *M,mpz_t *ans);
void CRT_RSA(mpz_t * c,mpz_t * p,mpz_t * q,mpz_t * d,mpz_t * ans);
void get_strong_prime(mpz_t *prime, int size);
bool rabin_wit(mpz_t key);
void randrange(mpz_t * randnum, mpz_t start, mpz_t end);
void mpz_pow(mpz_t * B,mpz_t * P,mpz_t * ans);

void init_set(mpz_t in[],int number);
void clear_set(mpz_t in[], int number);
void get_MI(mpz_t in[], mpz_t out[],int number);
void get_prime1(mpz_t in[], int number);
int inrange();
void get_edn(mpz_t in[], mpz_t *N, mpz_t *faiN, mpz_t *d, mpz_t *e,int number);
void get_si(mpz_t in[], mpz_t *d,mpz_t out[], int number);
void get_bi(mpz_t in[], mpz_t *M, mpz_t N[], mpz_t out[], int number);
void get_C(mpz_t MI[], mpz_t B[], mpz_t *C,mpz_t *N,int number);
void inversion(mpz_t N, mpz_t M, mpz_t *rea, mpz_t *reb);

int main(int argc, char **argv)
{
    mpz_t p,q,p_1,q_1,N,L,gcd,unit,c,e;
    //声明变量
    mpz_init(p);    mpz_init(q);    mpz_init(p_1);  mpz_init(q_1);
    mpz_init(N);    mpz_init(L);    mpz_init(gcd);  mpz_init(c);
    mpz_init_set_ui(unit,1);        mpz_init_set_ui(e,65537);
    //变量初始化
    get_strong_prime(&p,1024);
    get_strong_prime(&q,1024);
    mpz_mul(N, p, q);                   //N=p*q
    mpz_sub(p_1,p,unit);                //p_1=p-1
    mpz_sub(q_1,q,unit);                //q_1=q-1
    mpz_gcd(gcd,p_1,q_1);               //gcd为(p-1),(q-1)的最大公约数
    mpz_mul(L,p_1,q_1);
    mpz_div(L,L,gcd);                   //求(p-1),(q-1)的最小公倍数
    mpz_t d;    mpz_init(d);
    mpz_invert(d,e,L);                  //求e关于模L的逆元
    char *send_d=NULL,*send_N=NULL;
    send_d=mpz_get_str(send_d,10,d);
    send_N=mpz_get_str(send_N,10,N);    //得到d,N的字符串形式
    FILE *recipient=fopen("Recip_inf.txt","w+");
    fprintf(recipient,"d=\n%s\nN=\n%s\n请谨慎保管\n",send_d,send_N);
    fclose(recipient);                  //以上操作将密钥发送给接收者用于验证身份，解密
    printf("是否开启校验模式?(1:是/2:否)\n");
    //不开启校验模式，无法验证算法的正确性。建议使用校验模式进行算法检验
    int op1,op2,op3;
    scanf("%d",&op1);
    getchar();
    if(op1==1)
        goto flag1;
    else if(op1==2)
        printf("即将进入加密/解密模块(不校验)\n");
    else
    {
        printf("What?-_-......");
        return 0;
    }
    printf("模式: (1:加密/2:解密)\n");
    scanf("%d",&op2);
    getchar();
    if(op2==1)
    {
        flag1:
        printf("请输入明文的numtype:\n");
        char message[1024];
        gets(message);
        mpz_t m;
        mpz_init_set_str(m, message, 10);
        //将需要加密的numtype由字符串转化为mpz_t形式
        printf("公钥为:\nE=65537\nN=");
        gmp_printf("%Zd\n",N);      //输出N
        printf("现已公布\n");
        Fp(&m, &e, &N, &c);
        printf("密文为:\n");
        gmp_printf("%Zd\n",c);      //输出密文的numtype
        char *sendc=NULL;
        sendc=mpz_get_str(sendc,10,c);
        FILE *send=fopen("C_inf.txt","w+");
        fprintf(send,"您已收到密文:\n%s",sendc);
        fclose(send);               //将密文写入C_inf.txt
        printf("请妥善保管\n");
        if(op1==1)
            goto flag2;
    }
    else if(op2==2)
    {
        flag2:
        if(op1==1)
            printf("您选择校验模式，将直接进入解密模式:\n");
        printf("请输入密文的numtype:\n");
        char C[1024];
        gets(C);
        mpz_t c;
        mpz_init_set_str(c,C,10);
        //将密由字符串转化为mpz_t形式
        printf("\n请输入您的密钥用于身份验证：\nN=\n");
        char getN[1024],getd[1024];
        scanf("%s",getN);
        printf("d=\n");
        scanf("%s",getd);
        mpz_t GN,GD;
        mpz_init_set_str(GN,getN,10);
        mpz_init_set_str(GD,getd,10);
        if(mpz_cmp(N,GN)==0&&mpz_cmp(d,GD)==0)
        {
            printf("校验正确！\n");
            printf("是否采用中国剩余定理加速?(1:是/2:否)\n");
            scanf("%d",&op3);
            getchar();
            mpz_t ans;
            mpz_init(ans);
            if(op3==1)                  //将使用中国剩余定理还原密文
                CRT_RSA(&c,&p,&q,&d,&ans);
            else if(op3==2)
                Fp(&c,&d,&N,&ans);      //将用密钥还原密文
            printf("解密成功！\n");
            gmp_printf("%Zd",ans);
        }
        else
        {
            printf("密钥校验失败！程序终止！\n已向发送者反馈信息。");
            FILE *sender=fopen("Sender_inf.txt","w+");
            fprintf(sender,"有用户身份验证失败，请重新加密！\n");
            fprintf(sender,"密文为：%s",C);
            fclose(sender);
        }
    }
    else
    {
        printf("What?-_-......");
    }
    return 0;
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
        mpz_mod(ma,p,t);
        mpz_div(p,p,t);
        mpz_mul(temp,temp,temp);
        mpz_mod(temp,temp,m);
        if(mpz_cmp(ma,unit)==0)
        {
            mpz_mul(result,result,temp);
            mpz_mod(result,result,m);
        }
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

//得到强素数算法
void get_strong_prime(mpz_t *prime, int size)
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

//幂函数
void mpz_pow(mpz_t * B,mpz_t * P,mpz_t * ans)
{
    mpz_t b,p,result,cnt,unit;
    mpz_init_set(b,*B);         mpz_init_set(p,*P);
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

void init_set(mpz_t in[],int number)
{
    for (int i = 0; i < number; i++)
        mpz_init(in[i]);
}

void clear_set(mpz_t in[], int number)
{
    for (int i = 0; i < number; i++)
        mpz_clear(in[i]);
}

void get_MI(mpz_t in[], mpz_t out[],int number)
{
    mpz_t temp1, temp2; mpz_init(temp1); mpz_init(temp2);
    for (int i = 0; i < number; i++)
    {
        mpz_t sum; mpz_init(sum); mpz_set_ui(sum, 1);
        for (int j = 0; j < number; j++)
            if (j == i)continue;
            else mpz_mul(sum, sum, in[j]);
        mpz_gcdext(temp1, out[i], temp2, sum, in[i]);
        if (mpz_cmp_ui(out[i], 0) < 0)
            mpz_add(out[i], out[i], in[i]);
        mpz_mul(out[i], out[i], sum); //gmp_printf("%Zd,%Zd\n", out[i],sum);
        mpz_clear(sum);
        //gmp_printf("%Zd\n", out[i]);
    }
    mpz_clear(temp1);
    mpz_clear(temp2);
}



void get_prime1(mpz_t in[], int number)
{
    mpz_t min;      mpz_init(min);
    int minbitlen = 1024;
    mpz_set_ui(min, 2);
    mpz_pow_ui(min, min, minbitlen);
    srand(time(NULL));
    for (int i = 0; i < number; i++)
    {
        int len = rand()%20+1;
        while (len--)
            mpz_nextprime(min, min);
        mpz_set(in[i], min);
    }
    mpz_clear(min);
}

int inrange()
{
    int num;
    while((num=rand())>=50);
    return num;
}

void get_edn(mpz_t in[], mpz_t *N, mpz_t *phi, mpz_t *d, mpz_t *e,int number)
{
    mpz_set_ui(*N, 1);
    mpz_set_ui(*phi, 1);
    mpz_t temp[2];
    init_set(temp, 2);
    for (int i = 0; i < number; i++)
    {
        mpz_mul(*N, *N, in[i]);
        mpz_sub_ui(temp[0], in[i], 1);
        mpz_mul(*phi, *phi, temp[0]);
    }
    mpz_set_ui(*e, 3);
    for (int i = 0; i < 100; i++)
    {
        mpz_gcdext(temp[0], *d, temp[1], *e, *phi);
        if (mpz_cmp_ui(temp[0], 1) == 0)
        {
            if (mpz_cmp_ui(*d, 0) < 0)
                mpz_add(*d, *d, *phi);
            break;
        }
        mpz_nextprime(*e, *e);
    }
    clear_set(temp,2);
}

void get_si(mpz_t in[], mpz_t *d,mpz_t out[], int number)
{
    mpz_t temp; mpz_init(temp);
    for (int i = 0; i < number; i++)
    {
        mpz_sub_ui(temp, in[i], 1);
        mpz_mod(out[i], *d, temp);
        if (mpz_cmp_ui(out[i], 0) < 0)
            mpz_add(out[i],out[i], temp);
    }
}

void get_bi(mpz_t in[], mpz_t *M, mpz_t N[], mpz_t out[], int number)
{
    for (int i = 0; i < number; i++)
        Fp(M, &(in[i]), &(N[i]), &(out[i]));
}

void get_C(mpz_t MI[], mpz_t B[], mpz_t *C,mpz_t *N,int number)
{
    mpz_set_ui(*C, 0);
    mpz_t temp;
    mpz_init(temp);
    for (int i = 0; i < number; i++)
    {
        mpz_mul(temp, MI[i], B[i]);
        mpz_add(*C, *C, temp);
        mpz_mod(*C, *C, *N);
    }
}


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
    mpz_mul(step1,N,rea);
    mpz_sub(step2,unit,step1);
    mpz_div(step3,step2,M);
    mpz_set(*reb,step3);
}