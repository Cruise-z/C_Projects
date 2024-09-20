//
// Created by 赵睿智 on 2022/4/29.
//

#include "RSA_enc_dec.h"

///head of 维纳攻击
void con_frac(mpz_t buf[LEN], int *len, mpz_t N, mpz_t D){
    int i, cnt = 0;
    (*len) = 0;
    mpz_t temp, n, d;
    mpz_init_set(n, N);
    mpz_init_set(d, D);
    mpz_init(temp);
    for(i = 0; i < LEN; i++)
        mpz_init(buf[i]);
    while(mpz_cmp_si(d, 0) != 0) {
        mpz_div(buf[cnt++], n, d);
        mpz_mod(temp, n, d);
        mpz_set(n, d);
        mpz_set(d, temp);
        (*len) ++;
    }
    (*len)--;
}

//计算逼近分数列
void get_frac(mpz_t (*frac)[LEN][2], mpz_t buf[LEN], int len){
    int cnt1, cnt2;
    mpz_t n, d, temp;
    mpz_init(n);
    mpz_init(d);
    mpz_init(temp);
    for(cnt1 = 0; cnt1 < LEN; cnt1++){
        mpz_init((*frac)[cnt1][0]);
        mpz_init((*frac)[cnt1][1]);
    }//frac[cnt1][0]存逼近分数的分子，frac[cnt1][1]存逼近分数的分母
    for(cnt1 = 0; cnt1 < len; cnt1++){
        mpz_set_ui(d, 0);
        mpz_set_ui(n, 1);
        for(cnt2 = cnt1; cnt2 >= 0; cnt2--){
            mpz_set(temp, d);
            mpz_set(d, n);
            mpz_mul(n, n, buf[cnt2]);
            mpz_add(n, n, temp);
        }
        mpz_set((*frac)[cnt1][0], d);
        mpz_set((*frac)[cnt1][1], n);
    }
}

//解首项为1的一元二次方程的整数解，存在返回1，不存在返回0
int solve_x2_bx_c_integer(mpz_t *x1, mpz_t *x2, mpz_t b, mpz_t c){
    mpz_init(*x1);
    mpz_init(*x2);
    mpz_t delta, temp1, temp2;
    mpz_init(delta);
    mpz_init(temp1);
    mpz_init(temp2);
    //计算delta
    mpz_mul(temp1, b, b);
    mpz_mul_ui(temp2, c, 4);
    mpz_sub(delta, temp1, temp2);
    mpz_root(delta, delta, 2);
    mpz_neg(temp1, b);
    //计算x1
    mpz_add(temp2, temp1, delta);
    mpz_div_ui(*x1, temp2, 2);
    //计算x2
    mpz_sub(temp2, temp1, delta);
    mpz_div_ui(*x2, temp2, 2);
    //检验
    mpz_add(temp1, *x1, *x2);
    mpz_add(temp1, temp1, b);
    mpz_mul(temp2, *x1, *x2);
    if(mpz_cmp_ui(temp1, 0) == 0 && mpz_cmp(temp2, c) == 0)
        return 1;
    else
        return 0;
}

void Wiener_Attack(mpz_t *p, mpz_t *q, mpz_t *d, mpz_t e, mpz_t N){
    mpz_init(*p);
    mpz_init(*q);
    mpz_init(*d);
    mpz_t buf[LEN], frac[LEN][2];
    mpz_t phi, temp, x1, x2;
    mpz_init(phi);
    mpz_init(temp);
    int len, cnt;
    con_frac(buf, &len, e, N);
    get_frac(&frac, buf, len);
    for(cnt = 0; cnt < len; cnt++){
        if(mpz_cmp_ui(frac[cnt][1], 0) == 0)
            continue;
        mpz_mul(phi, e, frac[cnt][0]);
        mpz_sub_ui(phi, phi, 1);
        mpz_mod(temp, phi, frac[cnt][1]);
        if(mpz_cmp_ui(temp, 0) != 0)
            continue;
        mpz_div(phi, phi, frac[cnt][1]);
        mpz_sub(temp, N, phi);
        mpz_add_ui(temp, temp, 1);
        if(solve_x2_bx_c_integer(&x1, &x2, temp, N)){
            mpz_abs(*p, x1);
            mpz_abs(*q, x2);
            mpz_invert(*d, e, phi);
            break;
        }
    }
}
///end of 维纳攻击

///head of 已知公私钥分解合数N攻击
void PkSk2pq_Attack(mpz_t *p, mpz_t *q, mpz_t e, mpz_t d, mpz_t N){
    mpz_init(*p);        mpz_init(*q);
    mpz_t kphi_pq, k, temp;
    mpz_init(kphi_pq);
    mpz_init(k);
    mpz_init(temp);
    mpz_mul(kphi_pq, e, d);
    mpz_sub_ui(kphi_pq, kphi_pq, 1);
    mpz_div(k, kphi_pq, N);
    while(1){
        mpz_mod(temp, kphi_pq, k);
        if(mpz_cmp_si(temp, 0) == 0)
            break;
        else
            mpz_add_ui(k, k, 1);
    }
    mpz_div(temp, kphi_pq, k);
    mpz_t p_add_q, p_sub_q, temp1;
    mpz_init(p_add_q);  mpz_init(p_sub_q);
    mpz_init(temp1);
    mpz_sub(p_add_q, N, temp);
    mpz_add_ui(p_add_q, p_add_q, 1);
    mpz_mul(temp, p_add_q, p_add_q);
    mpz_mul_ui(temp1, N, 4);
    mpz_sub(temp, temp, temp1);
    mpz_root(p_sub_q, temp, 2);
    mpz_add(*p, p_add_q, p_sub_q);
    mpz_div_ui(*p, *p, 2);
    mpz_sub(*q, p_add_q, p_sub_q);
    mpz_div_ui(*q, *q, 2);
}
///end of 已知公私钥分解合数N攻击

///head of 共模攻击
void CM_Attack(mpz_t *message, mpz_t e1, mpz_t e2, mpz_t c1, mpz_t c2, mpz_t N){
    mpz_t temp;
    mpz_init(temp);
    mpz_init(*message);
    mpz_gcd_re re = gcd_ext(e1, e2);
    Fp(message, c1, re.x, N);
    Fp(&temp, c2, re.y, N);
    mpz_mul(*message, *message, temp);
    mpz_mod(*message, *message, N);
}
///end of 共模攻击

///head of 小指数广播攻击
void LEIB_Attack(mpz_t *message, mpz_t src_r[LEN], mpz_t src_p[LEN], int group_num){
    mpz_t r, p;
    mpz_init_set(r, src_r[0]);
    mpz_init_set(p, src_p[0]);
    int cnt;
    for(cnt = 1; cnt < group_num-1; cnt++)
        CRT(&r, &p, src_r[cnt], src_p[cnt]);
    mpz_root(*message, r, group_num);
}
///end of 小指数广播攻击

///head of RSA_OAEP
int MGF1_SHA1(byte_ptr ans_ptr, byte_arr mgfSeed, int64_t maskLen){
    int64_t h_len = 20;
    if(maskLen > (1ull<<32) * h_len)
        return 1;
    byte_arr temp, T, C, sha_re;
    int32_t counter, cnt, counter_temp;
    T.len = 0;
    for(counter = 0; counter < maskLen/h_len+1; counter++){
        counter_temp = counter;
        for(cnt = 3; cnt >=0; cnt--){
            C.arr[cnt] = counter_temp % 256;
            counter_temp /= 256;
        }
        C.len = 4;
        byte_cpy(&temp, mgfSeed, mgfSeed.len);
        byte_cat(&temp, C);
        SHA1(temp.arr, byte_len(temp), sha_re.arr);
        sha_re.len = h_len;
        byte_cat(&T, sha_re);
    }
    byte_cpy(ans_ptr, T, maskLen);
    return 0;
}

/*
 *下面是报错信息归纳:
 * 1: return值为 1->Mask is too long!
 * 2: return值为-1->Tag is too long!
 * 3: return值为-2->plaintext/Ciphertext is too long!
 * 4: return值为-3->k < 2hLen + 2!
 * 5: return值为-4->c >= N!
 * 6: return值为-5->Y isn't 0x00!
 * 7: return值为-6->hash wrong!
 * 8: return值为-7->can't find plaintext!
 */
int OAEP_SHA1(mpz_t *re, int64_t k, char *str_M, char *str_L, char *str_Seed, char mode, char *str_eORd, char *str_N){
    //返回值加密
    srand(time(NULL));
    str_M += 2;
    str_L += 2;
    str_Seed += 2;
    str_eORd += 2;
    str_N += 2;
    mpz_t M, L, e, N;
    int64_t cnt;
    mpz_init_set_str(M, str_M, 16);
    mpz_init_set_str(L, str_L, 16);
    mpz_init_set_str(e, str_eORd, 16);
    mpz_init_set_str(N, str_N, 16);
    int64_t L_len = mpz_sizeinbase(L, 16)/2;
    int64_t m_len = mpz_sizeinbase(M, 16)/2;
    int64_t h_len = 20;
    if(mode == 'e'){
        if(L_len > ((1ull<<61)-1)){
            printf("Err");
            return -1+9*rand();
        }else if(m_len > k-2*h_len-2){
            printf("Err");
            return -2+9*rand();
        }
        byte_arr byteL, lHASH, PS, DB, con, byteM, Seed, dbMask, maskedDB, seedMask, maskedSeed, EM;
        str2byteString(&byteL, str_L, 0);
        SHA1(byteL.arr, byteL.len, lHASH.arr);
        lHASH.len = 20;
        PS.len = k - m_len - 2 * h_len - 2;
        for(cnt = 0; cnt < PS.len; cnt++)
            PS.arr[cnt] = 0x00;
        byte_cpy(&DB, lHASH, lHASH.len);
        byte_cat(&DB, PS);
        con.arr[0] = 0x01;
        con.len = 1;
        byte_cat(&DB, con);
        str2byteString(&byteM, str_M, 0);
        byte_cat(&DB, byteM);
        str2byteString(&Seed, str_Seed, 0);
        MGF1_SHA1(&dbMask, Seed, k-h_len-1);
        for(cnt = 0; cnt < k-h_len-1; cnt++)
            maskedDB.arr[cnt] = dbMask.arr[cnt] ^ DB.arr[cnt];
        maskedDB.len = k-h_len-1;
        MGF1_SHA1(&seedMask, maskedDB, h_len);
        for(cnt = 0; cnt < h_len; cnt++)
            maskedSeed.arr[cnt] = seedMask.arr[cnt] ^ Seed.arr[cnt];
        maskedSeed.len = h_len;
        EM.arr[0] = 0x00;   EM.len = 1;
        byte_cat(&EM, maskedSeed);
        byte_cat(&EM, maskedDB);
        byteStr2num(re, EM);
        Fp(re, *re, e, N);
        mpz_clear(M);
        mpz_clear(L);
        mpz_clear(e);
        mpz_clear(N);
        return 0;
    }else if(mode == 'd'){
        if(L_len > ((1ull<<61)-1)){
            printf("Ree");
            return -1+9*rand();
        }else if(m_len > k){
            printf("Ree");
            return -2+9*rand();
        }else if(k < 2*h_len+2){
            printf("Ree");
            return -3+9*rand();
        }
        byte_arr byteL, byteC, EM, lHASH, Y, maskedDB, maskedSeed, seedMask, seed, dbMask, DB, plaintext;
        mpz_t c, m;
        char *str_m = NULL;
        str2byteString(&byteC, str_M, 0);
        byteStr2num(&c, byteC);
        Fp(&m, c, e, N);
        if(mpz_cmp(c, N) >= 0){
            printf("Ree");
            return -4+9*rand();
        }
        str_m = mpz_get_str(str_m, 16, m);
        str2byteString(&EM, str_m, k);
        str2byteString(&byteL, str_L, 0);
        SHA1(byteL.arr, byteL.len, lHASH.arr);
        lHASH.len = 20;
        Y.len = 0;
        maskedDB.len = 0;
        maskedSeed.len = 0;
        for(cnt = 0; cnt < k; cnt++){
            if(cnt == 0)
                Y.arr[Y.len++] = EM.arr[cnt];
            else if(cnt >= 1 && cnt <= h_len)
                maskedSeed.arr[maskedSeed.len++] = EM.arr[cnt];
            else if(cnt >= h_len+1 && cnt < k)
                maskedDB.arr[maskedDB.len++] = EM.arr[cnt];
        }
        if(Y.arr[0] != 0x00){
            printf("Ree");
            return -5+9*rand();
        }
        MGF1_SHA1(&seedMask, maskedDB, h_len);
        for(cnt = 0; cnt < h_len; cnt++)
            seed.arr[cnt] = maskedSeed.arr[cnt] ^ seedMask.arr[cnt];
        seed.len = h_len;
        MGF1_SHA1(&dbMask, seed, k-h_len-1);
        for(cnt = 0; cnt < k-h_len-1; cnt++)
            DB.arr[cnt] = maskedDB.arr[cnt] ^ dbMask.arr[cnt];
        DB.len = k-h_len-1;
        for(cnt = 0; cnt < h_len; cnt++){
            if(DB.arr[cnt] != lHASH.arr[cnt]){
                printf("Ree");
                return -6+9*rand();
            }
        }//步过lHASH'
        for(; cnt<DB.len && DB.arr[cnt]!=0x01; cnt++);
        if(cnt == DB.len){
            printf("Ree");
            return -7+9*rand();
        }else{
            plaintext.len = 0;
            for(cnt++; cnt < DB.len; cnt++)
                plaintext.arr[plaintext.len++] = DB.arr[cnt];
            byteStr2num(re, plaintext);
            mpz_clear(M);
            mpz_clear(L);
            mpz_clear(e);
            mpz_clear(N);
            return 0;
        }
    }else{
        mpz_clear(M);
        mpz_clear(L);
        mpz_clear(e);
        mpz_clear(N);
        printf("mode unrecognized!");
        return rand();
    }
}
///end of RSA_OAEP

void RSA_Workmode(){
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
    printf("Whether to use strong prime number to generate p,q?\n"
           "1.|p-q| is big\n2.gcd_str(p-1,q-1) is small\n1:Yes/2:No\n");
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
    FILE *recipient=fopen("../Exp07/RSA_work_inf/Recip_inf.txt","w+");
    fprintf(recipient,"d=\n%s\nN=\n%s\nPlease keep it carefully\n",send_d,send_N);
    fclose(recipient);                  //以上操作将密钥发送给接收者用于验证身份，解密
    printf("Whether to enable verification mode?(1:Yes/2:No)\n");
    //不开启校验模式，无法验证算法的正确性。建议使用校验模式进行算法检验
    scanf("%d",&op1);
    getchar();
    if(op1==1)
        goto flag1;
    else if(op1==2)
        printf("Entering the encryption / decryption module (no verification)\n");
    else
    {
        printf("What?-_-......");
    }
    printf("Mode: (1: encryption / 2: decryption)\n");
    scanf("%d",&op2);
    getchar();
    if(op2==1)
    {
        flag1:
        printf("Please enter num_type of plaintext:\n");
        char message[1024];
        gets(message);
        mpz_t m;
        mpz_init_set_str(m, message, 10);
        //将需要加密的numtype由字符串转化为mpz_t形式
        printf("Pk is:\nE=");
        gmp_printf("%Zd\n",e);
        printf("N=");
        gmp_printf("%Zd\n",N);      //输出N
        printf("Now published\n");
        Fp(&c, m, e, N);
        printf("Ciphertext is:\n");
        gmp_printf("%Zd\n",c);      //输出密文的numtype
        char *sendc=NULL;
        sendc=mpz_get_str(sendc,10,c);
        FILE *send=fopen("../Exp07/RSA_work_inf/C_inf.txt","w+");
        fprintf(send,"You have received the ciphertext:\n%s",sendc);
        fclose(send);               //将密文写入C_inf.txt
        printf("Please keep it properly\n");
        if(op1==1)
            goto flag2;
    }
    else if(op2==2)
    {
        flag2:
        if(op1==1)
            printf("This is the verification mode, you will directly enter the decryption mode:\n");
        printf("Please enter num_type of ciphertext:\n");
        char C[1024];
        gets(C);
        mpz_t getc;
        mpz_init_set_str(getc,C,10);
        //将密文由字符串转化为mpz_t形式
        printf("\nPlease enter your key for authentication:\nN=\n");
        char getN[1024],getd[1024];
        scanf("%s",getN);
        printf("d=\n");
        scanf("%s",getd);
        mpz_t GN,GD;
        mpz_init_set_str(GN,getN,10);
        mpz_init_set_str(GD,getd,10);
        if(mpz_cmp(N,GN)==0&&mpz_cmp(d,GD)==0)
        {
            printf("Correct verification!\n");
            printf("Is the Chinese remainder theorem adopted?(1:Yes/2:No)\n");
            scanf("%d",&op3);
            getchar();
            mpz_t ans;
            mpz_init(ans);
            if(op3==1)                     //将使用中国剩余定理还原密文
                CRT_RSA(&getc,&p,&q,&d,&ans);
            else if(op3==2)
                Fp(&ans, getc, d, N);      //将用密钥还原密文
            printf("Decryption succeeded!\n");
            gmp_printf("%Zd",ans);
        }
        else
        {
            printf("Key verification failed! Program terminated!\n"
                   "Feedback has been sent to the sender.");
            FILE *sender=fopen("../Exp07/RSA_work_inf/Sender_inf.txt","w+");
            fprintf(sender,"User authentication failed, please re encrypt!\n");
            fprintf(sender,"The ciphertext is:%s",C);
            fclose(sender);
        }
    }
    else
    {
        printf("What?-_-......");
    }
    printf("\nIs PEM coded? (1: Yes / 2: No)\n");
    scanf("%d",&op);
    getchar();
    if(op==1)
    {
        char Nhex[LEN],ehex[LEN],buf[LEN];
        unsigned char *buff=NULL;
        mpz_get_str(Nhex,16,N);     //将N转化为16进制
        mpz_get_str(ehex,16,e);     //将e转化为16进制
        FILE *public=fopen("../Exp07/RSA_work_inf/public_key.pem","w+");
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
        FILE *private=fopen("../Exp07/RSA_work_inf/private_key.pem","w+");
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
        printf("\nCoding succeeded!");
    }
    else
        printf("-_-");
}