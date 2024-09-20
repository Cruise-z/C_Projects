#include <stdio.h>
#include "gmp.h"

void Fib_solution1(long long int n,mpz_t *ans);
void init(mpz_t *arr,int len);

int main() {
    long long int n;
    mpz_t ans;
    mpz_init(ans);
    scanf("%lld",&n);
    Fib_solution1(n,&ans);
    gmp_printf("%Zd",ans);
    return 0;
}

void Fib_solution1(long long int n,mpz_t *ans)
{
    mpz_t arr[3];
    init(arr,3);
    mpz_set_ui(arr[0],1);
    mpz_set_ui(arr[1],1);
    long long int cnt;
    for(cnt=2;cnt<=n+1;cnt++)
        mpz_add(arr[cnt%3],arr[(cnt-1)%3],arr[(cnt-2)%3]);
    mpz_init_set(*ans,arr[cnt%3]);
}

void init(mpz_t *arr,int len)
{
    int cnt;
    for(cnt=0;cnt<len;cnt++)
        mpz_init(arr[cnt]);
}
