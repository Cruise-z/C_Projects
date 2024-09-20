#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<time.h>

#define BUF_LEN  100

int main(void)
{
 int ClientFd;
 char send_buff[BUF_LEN];
 char recv_buff[BUF_LEN]= {0};
 struct sockaddr ServerAddr;
 int addr_size = 0;
 struct sockaddr_in  ServerSockAddr;
 
 /* 创建客户端socket */
 if (-1 == (ClientFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)))
 {
  printf("socket error!\n");
  exit(1);
 }
 
 /* 向服务器发起请求 */
    memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));  
    ServerSockAddr.sin_family = PF_INET;
    ServerSockAddr.sin_addr.s_addr = inet_addr("192.168.1.100");
    ServerSockAddr.sin_port = htons(5000);
 
 addr_size = sizeof(ServerAddr);
 
 while (1)
 {

  /* 发送数据到服务端 */
  char send_buff[]={"Hello udp!\n"};
  sendto(ClientFd, send_buff, strlen(send_buff), 0, (struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));
  sleep(1);
  /* 接受服务端的返回数据 */
  recvfrom(ClientFd, recv_buff, BUF_LEN, 0, &ServerAddr, &addr_size);
  printf("Receive from server:%s\n",recv_buff);
  
  memset(recv_buff, 0, BUF_LEN);   // 重置缓冲区
 }
 
 close(ClientFd);   // 关闭套接字
 
 return 0;
}
