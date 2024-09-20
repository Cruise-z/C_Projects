#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include<time.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define ERR_MSG(err_code) do {                                     \
    err_code = errno;                                              \
    fprintf(stderr, "ERROR code: %d \n", err_code);                \
    perror("PERROR message");                                      \
} while (0)

const int BUF_LEN = 100;

int main(void)
{
    /* 配置 Server Sock 信息。*/
    struct sockaddr_in srv_sock_addr;
    memset(&srv_sock_addr, 0, sizeof(srv_sock_addr));
    srv_sock_addr.sin_family = AF_INET;
    srv_sock_addr.sin_addr.s_addr = inet_addr("192.168.1.100");
    srv_sock_addr.sin_port = htons(5000);

    int cli_socket_fd = 0;
    char send_buff[BUF_LEN];
    char recv_buff[BUF_LEN];

    /* 永循环从终端接收输入，并发送到 Server。*/


        /* 创建 Client Socket。*/
        if (-1 == (cli_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
        {
            printf("Create socket ERROR.\n");
            ERR_MSG(errno);
            exit(EXIT_FAILURE);
        }
         else printf("Create socket success!\n");
        /* 连接到 Server Sock 信息指定的 Server。*/
        if (-1 == connect(cli_socket_fd,
                          (struct sockaddr *)&srv_sock_addr,
                          sizeof(srv_sock_addr)))
        {
            printf("Connect to server ERROR.\n");
            ERR_MSG(errno);
            exit(EXIT_FAILURE);
        }
        else printf("Connect to server success!.\n");
   while (1) { 
        /* 发送到建立连接的 Server Socket。*/
      
        char send_buff[]={"hello tcp!\n"};
        send(cli_socket_fd, send_buff, BUF_LEN, 0);
        sleep(1);
        /* 从建立连接的 Server 接收数据。*/
            
        memset(recv_buff, 0, BUF_LEN);    
        recv(cli_socket_fd, recv_buff, BUF_LEN, 0);
         printf("Receive from server: %s\n", recv_buff);    
    }
    // close(cli_socket_fd);/* 每次 Client 请求和响应完成后，关闭连接。*/  
    return 0;
}
