#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define LEN 2048
#define maxlen 1000
#define maxpfx 10
#define maxsub 10

const char hex_table[] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

//RSA加密解密的相关函数
void Fp(mpz_t * B, mpz_t *P, mpz_t *M,mpz_t *ans);
void CRT_RSA(mpz_t * c,mpz_t * p,mpz_t * q,mpz_t * d,mpz_t * ans);
void get_prime(mpz_t *prime, int size);
bool get_strong_prime(mpz_t *key,int bit);
bool rabin_wit(mpz_t key);
void randrange(mpz_t * randnum, mpz_t start, mpz_t end);
void mpz_pow(mpz_t * B,mpz_t * P,mpz_t * ans);
void inversion(mpz_t N, mpz_t M, mpz_t *rea, mpz_t *reb);
//pem编解码用到的函数
unsigned char *base64_encode(unsigned char *str);
unsigned char *base64_decode(unsigned char *code);
void to_utf8(char *tar);

int main(int argc, char **argv)
{
    //声明变量
    mpz_t p,q_root_50,q,p_1,q_1,N,L,gcd,unit,two,b_1024,b_1025,c,e,p_q,p_q4;
    int op,op1,op2,op3;
    //变量初始化
    mpz_init(p);    mpz_init(q);    mpz_init(p_1);    mpz_init(q_1);
    mpz_init(N);    mpz_init(L);    mpz_init(gcd);    mpz_init(c);
    mpz_init(p_q);  mpz_init(p_q4); mpz_init(b_1024); mpz_init(b_1025);
    mpz_init(e);    mpz_init(q_root_50);
    mpz_init_set_ui(unit,1);        mpz_init_set_ui(two,2);
    mpz_pow_ui(b_1024,two,1024);    mpz_pow_ui(b_1025,two,1025);
    printf("是否利用强素数生成p,q?\n1.p,q相差较大\n2.gcd(p-1,q-1)很小\n1:是/2:否\n");
    scanf("%d",&op);
    getchar();
    if(op==1)
    {
        printf("ET:30secs\n");
        get_strong_prime(&p,1240);
        mpz_sub(p_1,p,unit);                    //p_1=p-1
        do{
            get_strong_prime(&q,1024);
            mpz_sub(q_1,q,unit);                //q_1=q-1
            mpz_gcd(gcd,p_1,q_1);               //gcd为(p-1),(q-1)的最大公约数
            mpz_root(q_root_50,q,6);
        }while(mpz_cmp(gcd,q_root_50)>=0);
        goto flag;
    }
    else if(op==2)
    {
        printf("ET:2min\n");
        get_prime(&p,1024);
        get_prime(&q,1240);
    }
    else
    {
        printf("What?-_-......");
        return 0;
    }
    mpz_sub(p_1,p,unit);                //p_1=p-1
    mpz_sub(q_1,q,unit);                //q_1=q-1
    mpz_gcd(gcd,p_1,q_1);               //gcd为(p-1),(q-1)的最大公约数
    flag:
    mpz_mul(N, p, q);                   //N=p*q
    mpz_mul(L,p_1,q_1);
    mpz_div(L,L,gcd);                   //求(p-1),(q-1)的最小公倍数
    randrange(&e,b_1024,b_1025);
    mpz_nextprime(e,e);                 //随机生成1024位的素数e
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
        printf("公钥为:\nE=");
        gmp_printf("%Zd\n",e);
        printf("N=");
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
        mpz_t getc;
        mpz_init_set_str(getc,C,10);
        //将密文由字符串转化为mpz_t形式
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
            if(op3==1)                     //将使用中国剩余定理还原密文
                CRT_RSA(&getc,&p,&q,&d,&ans);
            else if(op3==2)
                Fp(&getc,&d,&N,&ans);      //将用密钥还原密文
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
    printf("\n是否进行pem编码?(1:是/2:否)\n");
    scanf("%d",&op);
    getchar();
    if(op==1)
    {
        char Nhex[LEN],ehex[LEN],buf[LEN];
        unsigned char *buff=NULL;
        mpz_get_str(Nhex,16,N);     //将N转化为16进制
        mpz_get_str(ehex,16,e);     //将e转化为16进制
        FILE *public=fopen("public_key.pem","w+");
        fputs("-----BEGIN PUBLIC KEY-----\n",public);
        strcpy(buf,"this is N:");
        to_utf8(buf);
        buff= base64_encode(buf);
        fputs(buff,public);             //
        to_utf8(Nhex);
        buff= base64_encode(Nhex);
        fputs(buff,public);             //N转换输出完成
        strcpy(buf,"this is e:");
        to_utf8(buf);
        buff= base64_encode(buf);
        fputs(buff,public);             //
        to_utf8(ehex);
        buff= base64_encode(ehex);
        fputs(buff,public);             //e转换输出完成
        fputs("\n-----END PUBLIC KEY-----",public);
        fclose(public);
        char phex[LEN],qhex[LEN],dhex[LEN];
        mpz_get_str(phex,16,p);     //将p转化为16进制
        mpz_get_str(qhex,16,q);     //将q转化为16进制
        mpz_get_str(dhex,16,d);     //将d转化为16进制
        FILE *private=fopen("private_key.pem","w+");
        fputs("-----BEGIN RSA PRIVATE KEY-----\n",private);
        strcpy(buf,"this is p:");
        to_utf8(buf);
        buff= base64_encode(buf);
        fputs(buff,private);            //
        to_utf8(phex);
        buff= base64_encode(phex);
        fputs(buff,private);            //p转换输出完成
        strcpy(buf,"this is q:");
        to_utf8(buf);
        buff= base64_encode(buf);
        fputs(buff,private);            //
        to_utf8(qhex);
        buff= base64_encode(qhex);
        fputs(buff,private);            //q输出转换完成
        strcpy(buf,"this is d:");
        to_utf8(buf);
        buff= base64_encode(buf);
        fputs(buff,private);            //
        to_utf8(dhex);
        buff= base64_encode(dhex);
        fputs(buff,private);            //d输出转化完成
        fputs("\n-----END RSA PRIVATE KEY-----",private);
        fclose(private);
        printf("\n编码成功!");
    }
    else
        printf("-_-");
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
    str_len=strlen(str);
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
    len = strlen(code);
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
    int len=strlen(tar),cnt;
    for(cnt=len-1;cnt>=0;cnt--)
        tar[cnt+2]=tar[cnt];
    tar[0]='b';
    tar[1]='\'';
    tar[len+2]='\'';
    tar[len+3]='\0';
}