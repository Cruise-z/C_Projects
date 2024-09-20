#include<stdio.h>
#include<stdlib.h>

typedef struct polynomial {
    long long int quo;
    long long int exp;
}poly;

int cmp(const void *a, const void *b)
{
    poly *aptr = (poly*)a;
    poly *bptr = (poly*)b;
    if (aptr->exp != bptr->exp)
        return(((aptr->exp) > (bptr->exp)) ? -1 : 1);
    return 0;
}

int main()
{
    poly shi1[1024], shi2[1024], re[1024];
    char c = ' ';
    int len1 = 0, len2 = 0;
    while (c != '\n')
    {
        //shi1[len1].quo=1;
        scanf("%lld%lld%c",&shi1[len1].quo, &shi1[len1].exp, &c);
        len1++;
    }
    c = ' ';
    while (c != '\n')
    {
        //shi2[len2].quo=1;
        scanf("%lld%lld%c",&shi2[len2].quo, &shi2[len2].exp, &c);
        len2++;
    }
    int cnt1, cnt2, len = 0;
    for (cnt1 = 0; cnt1 < len1; cnt1++)
    {
        for (cnt2 = 0; cnt2 < len2; cnt2++)
        {
            re[len].quo = shi1[cnt1].quo * shi2[cnt2].quo;
            re[len].exp = shi1[cnt1].exp + shi2[cnt2].exp;
            len++;
        }
    }
    qsort(re, len, sizeof(re[0]), cmp);
    for (cnt1 = 0; cnt1 < len; cnt1++)
    {
        if (cnt1 < len - 1 && re[cnt1].exp == re[cnt1 + 1].exp)
        {
            re[cnt1 + 1].quo += re[cnt1].quo;
            re[cnt1].quo = 0;
        }
    }
    for (cnt1 = 0; cnt1 < len-1; cnt1++)
    {
        if (re[cnt1].quo != 0)
            printf("%lld * x^%lld + ", re[cnt1].quo%5, re[cnt1].exp);
    }
        printf("%lld * x^%lld + ", re[cnt1].quo%5, re[cnt1].exp);
    return 0;
}