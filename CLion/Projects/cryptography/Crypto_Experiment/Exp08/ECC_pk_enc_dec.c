//
// Created by 赵睿智 on 2022/5/13.
//

#include <stdio.h>
#include "../library_file/ECC/ECC_enc_dec.h"

int main(){
    int op;
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
        Cp ans;
        ECC_PK_enc(&ans, pm, k, pb, C, G);
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
        point p;
        ECC_PK_dec(&p, cp, nb, C);
        gmp_printf("%Zd %Zd", p.x, p.y);
    }
    return 0;
}