/*
本例程实现了飞腾派串口ttyAMA2的发送和接收。使用串口工具给飞腾派发送数据，接收到会在终端打印出来。并且会回复一个OK，表示收到数据，完成一次通信。
*/




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include<time.h>


const char default_path[] = "/dev/ttyAMA2";//根据具体的设备修改
int main(int argc, char *argv[])
{
   int fd;
   int res;
   char *path;
   char txbuf[512] = "This is tty send test.\n";
   char rxbuf[1024];
   //若无输入参数则使用默认终端设备
   if (argc > 1)
       path = argv[1];
   else
   path = (char *)default_path; //获取串口设备描述符
   fd = open(path, O_RDWR);//打开串口
   if (fd < 0) {
       printf("Fail to Open %s device\n", path);
       return 0;
   }

   /* 设置串口参数 */
   struct termios opt; 
   tcflush(fd, TCIFLUSH);//清空串口接收和发送缓冲区
   tcflush(fd, TCOFLUSH);
   tcgetattr(fd, &opt); // 获取串口参数 opt
   cfsetospeed(&opt, B115200); //设置串口输出波特率
   cfsetispeed(&opt, B115200);//设置串口输入波特率
   opt.c_cflag &= ~CSIZE;//设置数据位数
   opt.c_cflag |= CS8;
   opt.c_cflag &= ~PARENB;//校验位
   opt.c_iflag &= ~INPCK;
   opt.c_cflag &= ~CSTOPB;//设置停止位
   tcsetattr(fd, TCSANOW, &opt);//更新配置
   printf("Device %s is set to 115200bps,8N1\n",path);


   if(fcntl(fd,F_SETFL,0) < 0)//阻塞， 即使前面在open串口设备时设置的是非阻塞的，这里设为阻塞后，以此为准
   {
       printf("fcntl failed\n");
   }
   else{
       printf("fcntl=%d\n", fcntl(fd,F_SETFL,0));
   }
   fcntl(fd, F_SETFL, 0);  //串口阻塞

   /* 收发程序 */
     write(fd, txbuf, strlen(txbuf));
     
   while(1)
   {
       tcflush(fd, TCIFLUSH);//清空串口接收和发送缓冲区
       res = read(fd, rxbuf, sizeof(rxbuf)); //接收字符串
       if (res >0 ) 
      {
       rxbuf[res] = '\0';  //给接收到的字符串加结束符
      // printf("Receive res = %d bytes data: %s\n",res,rxbuf);
       printf("%s\n",rxbuf);      
       write(fd, "ok!\n", 4);  //回应发送字符串ok!
       //tcflush(fd, TCOFLUSH);
       res = 0;
      }     
   } 

   printf("read error,res = %d",res);

   close(fd);
   return 0;

}
