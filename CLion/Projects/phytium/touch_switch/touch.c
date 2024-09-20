/* 
本案例实现检测触摸开关的输出电平，使用到的引脚为GPIO2_10，将这个脚设置为输入，触摸开关的VCC接3.3V，GND接地。
触摸时输出为1，无触摸时输出为0
 */
//涉及到设备操作的,需包含以下头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//包含close/read/write/lseek等函数
#include <unistd.h>

//包含printf/sprintf函数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/gpio.h>
#define touch 474 //GPIO2_10
#define GPIO_PATH "/sys/class/gpio/"

int main(void)
{
    char path[100];
    char cmd_export[100];
    char buf[10];
    char *str;
    int fd;
    int cnt_w;      //写入的字节数
    int ret;
  
 //判断对应的操作文件夹是否存在，不存在自动创建
  
       sprintf(path, "%sgpio%d", GPIO_PATH, touch); 

        if (!access(path, 0))
            printf("%s 文件夹存在\n", path);
        else
        {
            printf("%s 文件夹不存在\n", path);
            sprintf(cmd_export, "echo %d > %sexport", touch, GPIO_PATH);//echo 474 > /sys/class/gpio/export
            system(cmd_export); //执行export命令
            if(!access(path, 0))    //访问文件夹,确认创建成功
                printf("%s 导出成功\n", path);
            else 
                return -1;
        }


//设置输出方向
       sprintf(path, "%sgpio%d/direction", GPIO_PATH, touch);
        fd = open(path, O_RDWR);    //linux系统函数,失败返回-1
        if(fd != -1)      //打开成功
        {
            printf("%s 打开成功\n", path);
            cnt_w = write(fd, "in", sizeof("in"));    //linux系统函数
            if(cnt_w <= 0)
            {
                printf("方向写入失败\n");
                return -1;
            }
            else 
                printf("方向写入成功\n");
            close(fd);     //linux系统函数,成功返回0
        }
        else 
        {
            printf("%s 文件打开失败\n", path);
            return -1;
        }

//检测触摸开关输出
   
    while(1)
    {
           sprintf(path, "%sgpio%d/value", GPIO_PATH, touch);       
            fd = open(path, O_RDWR);    //打开输入输出电平值设备
            if(fd != -1)
            {
                ret = read(fd,buf, sizeof(buf)); //读取电平值
                printf("检测到触摸开关输出为：%s\n",buf);     
                 usleep(200000);//延时200毫秒
                 close(fd);
              } 
            else 
            {
                printf("%s 文件打开失败\n", path); 
            }  

    }
    return 0;
}
