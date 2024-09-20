/*
本案例实现呼吸灯效果，使用到的引脚为PWM2(GPIO1_1)，将这个引脚设置为输出，LED负极接地。
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

#define LED1_PIN 0 //PWM2_0 = 2+0
#define LED2_PIN 492 //GPIO1_12 = 480+12
#define LED3_PIN 491 //GPIO1_11 = 480+11


#define PWM_PATH "/sys/class/pwm/pwmchip2/"

int setargs(int *led_table, int led_pos, char *args, char *input);

int main(void){

    int led_table[4] = {LED1_PIN};
    char path[100];
    char cmd_export[100];
    int i;

    char *str;
    int fd;
    int cnt_w;      //写入的字节数

    //判断LED对应的操作文件夹是否存在，不存在自动创建
    for(i = 0; i < 3; i++){
        sprintf(path, "%spwm%d", PWM_PATH, 2+led_table[i]); //sys/class/pwm/pwmchip2/pwm2

        if (!access(path, 0))
            printf("%s 文件夹存在\n", path);
        else{
            printf("%s 文件夹不存在\n", path);
            sprintf(cmd_export, "echo %d > %sexport", led_table[i], PWM_PATH);//echo 0 > /sys/class/pwm/pwmchip2/export
            system(cmd_export); //执行export命令

            if(!access(path, 0))    //访问文件夹,确认创建成功
                printf("%s 导出成功\n", path);
            else
                return -1;
        }
    }

    //设置基本参数
    for(i = 0; i < 1; i++){
        setargs(led_table, 0, "period", "50000");
        setargs(led_table, 0, "polarity", "normal");
        setargs(led_table, 0, "enable", "1");
    }

    //呼吸灯效果
    while(1){
        char arg[5];
        for(i = 10000; i < 40000; i++){
            sprintf(arg, "%d", i);
            setargs(led_table, 0, "duty_cycle", arg);
            sleep(0.1);
        }
        for(i = 40000; i > 10000; i--){
            sprintf(arg, "%d", i);
            setargs(led_table, 0, "duty_cycle", arg);
            sleep(0.1);
        }
    }
    return 0;
}


int setargs(int *led_table, int led_pos, char *args, char *input){
    char path[100];
    int fd;
    int cnt_w;      //写入的字节数

    sprintf(path, "%spwm%d/%s", PWM_PATH, 2+led_table[led_pos], args); // /sys/class/pwm/pwmchip2/pwm2/polarity
    fd = open(path, O_RDWR);    //linux系统函数,失败返回-1
    if(fd != -1){//打开成功
        printf("%s 打开成功\n", path);
        cnt_w = write(fd, input, strlen(input));    //linux系统函数
        if(cnt_w <= 0){
            printf("参数写入失败\n");
            return -1;
        }
        else
            printf("参数写入成功\n");
        close(fd);     //linux系统函数,成功返回0
    }
    else{
        printf("%s 文件打开失败\n", path);
        return -1;
    }
}