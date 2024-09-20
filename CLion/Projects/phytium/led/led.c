/* 
本案例实现流水灯效果，使用到的引脚为GPIO3_1，GPIO1_12，GPIO1_11，将这3个脚设置为输出，LED负极接地。
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

#define LED1_PIN 449 //GPIO3_1 = 448+1
#define LED2_PIN 492 //GPIO1_12 = 480+12
#define LED3_PIN 491 //GPIO1_11 = 480+11


#define GPIO_PATH "/sys/class/gpio/"

int main(void){

    int led_table[4] = {LED1_PIN, LED2_PIN, LED3_PIN};
    char path[100];
    char cmd_export[100];
    int i;
    
    char *str;
    int fd;
    int cnt_w;      //写入的字节数

    //判断LED对应的操作文件夹是否存在，不存在自动创建
    for(i = 0; i < 3; i++){
        sprintf(path, "%sgpio%d", GPIO_PATH, led_table[i]); //sys/class/gpio/gpio449

        if (!access(path, 0))
            printf("%s 文件夹存在\n", path);
        else{
            printf("%s 文件夹不存在\n", path);
            sprintf(cmd_export, "echo %d > %sexport", led_table[i], GPIO_PATH);//echo 449 > /sys/class/gpio/export
            system(cmd_export); //执行export命令

            if(!access(path, 0))    //访问文件夹,确认创建成功
                printf("%s 导出成功\n", path);
            else 
                return -1;
        }
    }

    //设置输出方向
    for(i = 0; i < 3; i++){
        sprintf(path, "%sgpio%d/direction", GPIO_PATH, led_table[i]);
        fd = open(path, O_RDWR);    //linux系统函数,失败返回-1
        if(fd != -1){//打开成功
            printf("%s 打开成功\n", path);
            cnt_w = write(fd, "out", 3);    //linux系统函数
            if(cnt_w <= 0){
                printf("方向写入失败\n");
                return -1;
            }
            else 
                printf("方向写入成功\n");
            close(fd);     //linux系统函数,成功返回0
        }
        else{
            printf("%s 文件打开失败\n", path);
            return -1;
        }
    }

    //流水灯效果
    while(1){
        for(i = 0; i < 3; i++){
            sprintf(path, "%sgpio%d/value", GPIO_PATH, led_table[i]);
            
            fd = open(path, O_RDWR);    //linux系统函数
            if(fd != -1){
                write(fd, "0", 1);
                lseek(fd, 0, SEEK_SET); //重新定位到文件起始
                printf("%s 写入0\n", path);
                sleep(1);

                write(fd, "1", 1);
                printf("%s 写入1\n", path);
                lseek(fd, 0, SEEK_SET);

                close(fd);
            }
            else{
                printf("%s 文件打开失败\n", path);
            }  
        }
    }
    return 0;
}
