#include <stdio.h>
#include "stdlib.h"

int *temp=NULL;

int reversePairs(int *nums,int length);
int merge(int *nums,int left,int right);

int main()
{
    int len,cnt;
    scanf("%d",&len);
    int *arr=malloc(sizeof(int)*len);
    for(cnt=0;cnt<len;cnt++)
        scanf("%d",&arr[cnt]);
    int ans= reversePairs(arr,len);
    printf("%d",ans);
    return 0;
}

int reversePairs(int *nums,int length)
{
    if(length==0)
        return 0;
    temp=malloc(sizeof(int)*length);
    return merge(nums,0,length-1);
}

int merge(int *nums,int left,int right)
{
    if(left>=right)
        return 0;
    int mid=(left+right)/2;
    //Step1 左右问题求解
    int sum=merge(nums,left,mid)+merge(nums,mid+1,right);
    //i为左边索引,j为右边索引
    int i=left,j=0;
    //Step2 : 求解左右逆序对
    while (i <= mid) {
        j = mid + 1;
        while (j <= right && nums[i] > (long) nums[j] * 2) {
            j++;
        }
        sum += j - mid - 1;
        i++;
    }
    //Step3 : 归并排序, 合并流程
    i = left;
    j = mid + 1;
    int k = 0;
    while (i <= mid && j <= right) {
        if (nums[i] < nums[j]) {
            temp[k++] = nums[i++];
        } else {
            temp[k++] = nums[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = nums[i++];
    }
    while (j <= right) {
        temp[k++] = nums[j++];
    }
    for (int index = 0; index <= right - left; index++) {
        nums[left + index] = temp[index];
    }
    return sum;
}
//https://blog.csdn.net/u010003835/article/details/88404234?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_title~default-1.no_search_link&spm=1001.2101.3001.4242.2