/*
以下以操作GPIO3_1作为输入和GPIO1_12作为输出的为例 ，说明GPIO的使用方法。
GPIO1_12归属于gpiochip1 ，gpiochip1对应的编号从480到495 ，GPIO1_12偏移12 ，所以对应的编号为 480+12=492。
同理 ，GPIO3_1的编号归属于gpiochip3 ，gpiochip3对应的编号从480到495，GPIO3_1偏移1，所以对应编号为 448+1=449
以下程序将GPIO1_12每2秒翻转一次电平，然后从GPIO3_1读回，执行5次后退出程序。
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
int main()
{
        int nCnt = 5;
        system("echo 492 > /sys/class/gpio/export");  
        system("echo out > /sys/class/gpio/gpio492/direction");
        system("echo 449 > /sys/class/gpio/export");  
        system("echo in > /sys/class/gpio/gpio449/direction");
        do
        {
                system("echo 0 > /sys/class/gpio/gpio492/value");
                sleep(1);//延时1s
                system("cat /sys/class/gpio/gpio449/value");
                system("echo 1 > /sys/class/gpio/gpio492/value");
                sleep(1);//延时1s
                system("cat /sys/class/gpio/gpio449/value");              
                nCnt--;
        }while(nCnt);
        system("echo 492 > /sys/class/gpio/unexport");
        system("echo 449 > /sys/class/gpio/unexport");
        printf("执行完毕!\n");
        return 0;
}
