#include <stdio.h>

#define LEN 1024

typedef struct arr{
    int len;
    int con[LEN];
}create,*ptr;

void sort(ptr numsp,ptr smallerp,ptr posp,int from,int to);
create countsmaller(ptr nums);

int main() {
    create nums;
    scanf("%d",&(nums.len));
    int cnt;
    for(cnt=0;cnt<nums.len;cnt++)
        scanf("%d",&(nums.con[cnt]));
    create ans=countsmaller(&nums);
    for(cnt=0;cnt<ans.len;cnt++)
        printf("%d ",ans.con[cnt]);
    return 0;
}

void sort(ptr numsp,ptr smallerp,ptr posp,int from,int to)
{
    if(from>=to)
        return;
    int m=(from+to)/2;
    sort(numsp,smallerp,posp,from,m);
    sort(numsp,smallerp,posp,m+1,to);
    create merged;
    merged.len=to-from+1;
    int i=from, j=m+1, k=0, jump = 0;
    while (i<=m || j<=to)
    {
        if (i > m){
            jump++;
            merged.con[k++] = posp->con[j++];
        } else if (j > to) {
            smallerp->con[posp->con[i]] += jump;
            merged.con[k++] = posp->con[i++];
        } else if (numsp->con[posp->con[i]] <= numsp->con[posp->con[j]]) {
            smallerp->con[posp->con[i]] += jump;
            merged.con[k++] = posp->con[i++];
        } else {
            jump++;
            merged.con[k++] = posp->con[j++];
        }
    }
    for(int p=0; p<merged.len; p++)
        posp->con[from+p] = merged.con[p];
}

//该函数是对主程序的一个封装
create countsmaller(ptr nums)
{
    create smaller,pos;
    smaller.len=nums->len;
    pos.len=nums->len;
    for(int i=0; i<pos.len; i++)
        pos.con[i] = i;
    sort(nums, &smaller, &pos, 0, nums->len-1);
    create re;
    re.len=nums->len;
    int cnt;
    for(cnt=0;cnt<re.len;cnt++)
        re.con[cnt]=smaller.con[cnt];
    return re;
}