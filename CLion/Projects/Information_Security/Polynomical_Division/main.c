#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<stdbool.h>

typedef struct result {
	int num;			//分子
	int deno;			//分母
}re,*ptr;

int gcd(int a, int b);
void subtraction(ptr arrptr, int num1, int deno1, int num2, int deno2);

int main()
{
	int maxdegree1;
	printf("被除多项式的最高次数为：");
	scanf("%d", &maxdegree1);
	ptr arr1 = malloc(sizeof(re)*(maxdegree1 + 1));
	int cnt1 = 0;
	for (cnt1 = 0; cnt1 < maxdegree1 + 1; cnt1++)
	{
		scanf("%d", &arr1[cnt1].num);
		arr1[cnt1].deno = 1;
	}

	int maxdegree2;
	printf("作除多项式的最高次数为：");
	scanf("%d", &maxdegree2);
	ptr arr2 = malloc(sizeof(re)*(maxdegree2 + 1));
	int cnt2 = 0;
	for (cnt2 = 0; cnt2 < maxdegree2 + 1; cnt2++)
	{
		scanf("%d", &arr2[cnt2].num);
		arr2[cnt2].deno = 1;
	}

	int items = maxdegree1 - maxdegree2 + 1;
	ptr rearr = malloc(sizeof(re)*(items));
	int cnt3;
	for (cnt3 = 0; cnt3 < items; cnt3++)
	{
		int s1 = arr1[cnt3].num*arr2[0].deno;
		int s2 = arr1[cnt3].deno*arr2[0].num;
		int regcd = gcd(s1,s2);
		rearr[cnt3].num = s1 / regcd;
		rearr[cnt3].deno = s2 / regcd;
		int cnt4 = 0;
		for (cnt4 = 0; cnt4 < maxdegree2 + 1; cnt4++)
		{
			int Cnt = cnt3 + cnt4;
			subtraction(&arr1[Cnt], arr1[Cnt].num, arr1[Cnt].deno, arr2[cnt4].num*rearr[cnt3].num, arr2[cnt4].deno*rearr[cnt3].deno);
			//printf("%d/%d ", arr1[Cnt].num, arr1[Cnt].deno);
		}
	}
	int cnt;
	printf("\n结果为：\n");
	for (cnt = 0; cnt < items; cnt++)
	{
		if (rearr[cnt].num == 0)
			continue;
		if (rearr[cnt].deno == 1 || rearr[cnt].deno == -1)
			printf("%d * x^%d\n", rearr[cnt].num / rearr[cnt].deno, items - cnt - 1);
		else if(rearr[cnt].deno>0)
			printf("%d/%d * x^%d\n", rearr[cnt].num, rearr[cnt].deno, items - cnt - 1);
        else
            printf("%d/%d * x^%d\n", -rearr[cnt].num, -rearr[cnt].deno, items - cnt - 1);
	}
	printf("余项为：\n");
	for (cnt = items; cnt < maxdegree1 + 1; cnt++)
	{
		if (arr1[cnt].num == 0)
			continue;
		if (arr1[cnt].deno == 1 || arr1[cnt].deno == -1)
			printf("%d * x^%d\n", arr1[cnt].num / arr1[cnt].deno, maxdegree1 - cnt);
		else if(arr1[cnt].deno>0)
			printf("%d/%d * x^%d\n", arr1[cnt].num, arr1[cnt].deno, maxdegree1 - cnt);
        else
            printf("%d/%d * x^%d\n", -arr1[cnt].num, -arr1[cnt].deno, maxdegree1 - cnt);
	}
	return 0;
}

int gcd(int a, int b)
{
	if (a%b == 0)
		return b;
	else
		return gcd(b, a%b);
}

void subtraction(ptr arrptr, int num1, int deno1, int num2, int deno2)
{
	int numh = num1 * deno2 - num2 * deno1;
	int denoh = deno1 * deno2;
	int gcdh = gcd(numh, denoh);
	arrptr->num = numh / gcdh;
	arrptr->deno = denoh / gcdh;
}
