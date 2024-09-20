/*
本案例用到40pin引脚中的引脚3和5，对应的IIC设备节点为"/dev/i2c-2"
*/

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
 
/* AHT10所对应的I2C控制器的设备节点 */ 
#define DEVICE_NAME    	"/dev/i2c-2"	

/* I2C设备地址 */
#define DEVICE_ADDR    0x38


int main()
{
	int fd,i,ret=0;
	
	/* 将要读取的数据buf*/
	unsigned char rd_buf[13] = {0x10};  
	
	/* 要写的数据buf，第0个元素是要操作设备的寄存器地址*/
	unsigned char wr_buf[13] = {0};     
    
	printf("hello,this is read_write i2c test \n");
	
	/* 打开AHT10对应的I2C控制器文件 */
	fd =open(DEVICE_NAME, O_RDWR);
	if (fd< 0) 
	{
		printf("open"DEVICE_NAME"failed \n");
	}

	/*设置AHT10的I2C设备地址*/
	if (ioctl(fd,I2C_SLAVE_FORCE, DEVICE_ADDR) < 0) 
	{            
		printf("set slave address failed \n");
	}
	
	/* 将要操作的寄存器首地址赋给wr_buf[0] */
	//初始化//		
	wr_buf[0] = 0xe1; //保持主机
	wr_buf[1] = 0x08;
	wr_buf[2] = 0x00;	
	/* 通过write函数完成向AHT10写数据的功能 */
	write(fd, wr_buf, 3);
    
    while(1)
    {
        ///开始数据采集//
        wr_buf[0] = 0xac;       //commond 测量
        wr_buf[1] = 0x33;       //data0
        wr_buf[2] = 0x00;	    //data1
        /* 通过write函数完成向eeprom写数据的功能 */
        write(fd, wr_buf, 3);
        sleep(1);
        
        /* 从wr_buf[0] = w_add的寄存器地址开始读取12个字节的数据 */
        ret=read(fd, rd_buf, 6);
      /*printf("ret is %d \r\n",ret);

        for(i=0;i<6;i++)
        {
            printf("rd_buf is :%d\n",rd_buf[i]);
        }
        */
        /*AHT10数据格式的整理*/
        int h1 = 0 ,t1 = 0;
        h1 = rd_buf[1];
        h1 = (h1<<8) | rd_buf[2];
        h1 = (h1<<8) | rd_buf[3];
        h1 = h1>>4;

        h1 = (h1*1000)/1024/1024;//相对湿度转换

        t1 = rd_buf[3];
        t1 = t1 & 0x0000000F;
        t1 = (t1<<8) | rd_buf[4];
        t1 = (t1<<8) | rd_buf[5];

        t1 = (t1*2000)/1024/1024 - 500;//相对温度转换
        printf("温度:%d%d.%d",t1/100,(t1/10)%10,t1%10);
        printf("湿度:%d%d.%d\r\n",h1/100,(h1/10)%10,h1%10);
        printf("\r\n");

        sleep(1);
    }


	/* 完成操作后，关闭AHT10对应的I2C控制器的设备文件 */
	close(fd);
	return 0;
}

