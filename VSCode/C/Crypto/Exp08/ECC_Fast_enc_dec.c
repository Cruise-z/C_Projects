//
// Created by 赵睿智 on 2022/5/19.
//

#include <stdio.h>
#include "../../.lib/ECC/ECC_enc_dec.h"

int main(){
    int op, cnt, num;
    EC C;
    init_EC(&C);
    point G;
    init_point(&G);
    gmp_scanf("%Zd%Zd%Zd", &C.p, &C.a, &C.b);
    gmp_scanf("%Zd%Zd", &G.x, &G.y);
    scanf("%d\n", &op);
    if(op == 1){
        point pm, pb;
        init_point(&pm);
        init_point(&pb);
        mpz_t k;
        mpz_init(k);
        gmp_scanf("%Zd%Zd", &pm.x, &pm.y);
        gmp_scanf("%Zd", &k);
        gmp_scanf("%Zd%Zd", &pb.x, &pb.y);
        scanf("%d", &num);
        Cp pre, ans;
        ECC_PK_fast_enc_pre(&pre, k, pb, C, G);
        ECC_PK_fast_enc(&ans, pm, pre, C);
        for(cnt = 0; cnt < num; cnt++){
            assign_point(&pm, ans.C2);
            ECC_PK_fast_enc(&ans, pm, pre, C);
        }
        gmp_printf("%Zd %Zd\n", ans.C1.x, ans.C1.y);
        gmp_printf("%Zd %Zd", ans.C2.x, ans.C2.y);
    }else if(op == 0){
        Cp cp;
        init_Cp(&cp);
        mpz_t nb;
        mpz_init(nb);
        gmp_scanf("%Zd%Zd", &cp.C1.x, &cp.C1.y);
        gmp_scanf("%Zd%Zd", &cp.C2.x, &cp.C2.y);
        gmp_scanf("%Zd", &nb);
        scanf("%d", &num);
        point pre, p;
        ECC_Pk_fast_dec_pre(&pre, cp.C1, nb, C);
        ECC_Pk_fast_dec(&p, cp, pre, C);
        for(cnt = 0; cnt < num; cnt++){
            assign_point(&cp.C2, p);
            ECC_Pk_fast_dec(&p, cp, pre, C);
        }
        gmp_printf("%Zd %Zd", p.x, p.y);
    }
    return 0;
}