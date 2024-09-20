#include <stdio.h>

#define LEN 1024

typedef struct{
    int len;
    int arr[LEN];
}create,*ptr;

int sort(ptr sump,int from,int to,int upper,int lower);
int countsum(ptr nump,int lower,int upper);

int main()
{
    create des;
    scanf("%d",&(des.len));
    int cnt;
    for(cnt=0;cnt<des.len;cnt++)
        scanf("%d",&(des.arr[cnt]));
    int upper,lower;
    scanf("%d",&lower);
    scanf("%d",&upper);
    int ans= countsum(&des,lower,upper);
    printf("%d",ans);
    return 0;
}

//实现主函数
int sort(ptr sump,int from,int to,int upper,int lower)
{
    if (from > to) return 0;
    if (from == to) return (lower <= sump->arr[from] && sump->arr[from] <= upper) ? 1 : 0;
    //两个对于特殊状况的前置判断
    int count = 0;
    int m = (from + to) / 2;
    //递归调用，解决将数组分开后每个数组中的情况
    count = sort(sump, from, m, lower, upper) + sort(sump, m+1, to, lower, upper);
    //利用归并的思想解决区间包含数组分割点的状况
    for(int i=from, j=m+1, k=m+1; i<=m; i++) {
        while (j<=to && sump->arr[j] - sump->arr[i] < lower) j++;
        while (k<=to && sump->arr[k] - sump->arr[i] <= upper) k++;
        count += k - j;
    }
    create merged;
    merged.len=to-from+1;
    for(int i=0, j=from, k=m+1; i<merged.len; i++) {
        if (j>m)
            merged.arr[i] = sump->arr[k++];
        else if (k>to)
            merged.arr[i] = sump->arr[j++];
        else if (sump->arr[j] <= sump->arr[k])
            merged.arr[i] = sump->arr[j++];
        else
            merged.arr[i] = sump->arr[k++];
    }
    int cnt;
    for(cnt=0;cnt<merged.len;cnt++)
        sump->arr[from+cnt]=merged.arr[cnt];
    sump->len=(from+merged.len>sump->len)?from+merged.len:sump->len;
}

int countsum(ptr nump,int lower,int upper)
{
    if(nump->len==0)
        return 0;
    create sums;
    //sums数组算的是累计和数组
    sums.len=nump->len;
    sums.arr[0]=nump->arr[0];
    for(int i=1; i<nump->len; i++)
        sums.arr[i] = sums.arr[i-1] + nump->arr[i];
    return sort(&sums, 0, sums.len-1, lower, upper);
}