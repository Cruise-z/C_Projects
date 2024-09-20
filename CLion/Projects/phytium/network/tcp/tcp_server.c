#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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
    srv_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 即 0.0.0.0 表示监听本机所有的 IP 地址。
    srv_sock_addr.sin_port = htons(5000);      //监听端口号

    /* 创建 Server Socket。*/
    int srv_socket_fd = 0;
    if (-1 == (srv_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
        printf("Create socket file descriptor ERROR.\n");
        ERR_MSG(errno);
        exit(EXIT_FAILURE);
    }
    /* 设置 Server Socket 选项。*/
    int optval = 1;
    if (setsockopt(srv_socket_fd,
                   SOL_SOCKET,    // 表示套接字选项的协议层。
                   SO_REUSEADDR,  // 表示在绑定地址时允许重用本地地址。这样做的好处是，当服务器进程崩溃或被关闭时，可以更快地重新启动服务器，而不必等待一段时间来释放之前使用的套接字。
                   &optval,
                   sizeof(optval)) < 0)
    {
        printf("Set socket options ERROR.\n");
        ERR_MSG(errno);
        exit(EXIT_FAILURE);
    }

    /* 绑定 Socket 与 Sock Address 信息。*/
    if (-1 == bind(srv_socket_fd,
                   (struct sockaddr *)&srv_sock_addr,
                   sizeof(srv_sock_addr)))
    {
        printf("Bind socket ERROR.\n");
        ERR_MSG(errno);
        exit(EXIT_FAILURE);
    }

    /* 开始监听 Client 发出的连接请求。*/
    if (-1 == listen(srv_socket_fd, 10))
    {
        printf("Listen socket ERROR.\n");
        ERR_MSG(errno);
        exit(EXIT_FAILURE);
    }

    /* 初始化 Client Sock 信息存储变量。*/
    struct sockaddr cli_sock_addr;
    memset(&cli_sock_addr, 0, sizeof(cli_sock_addr));
    int cli_sockaddr_len = sizeof(cli_sock_addr);

    int cli_socket_fd = 0;

    int recv_len = 0;
    char recv_buff[] = {0};

    /* 永远接受 Client 的连接请求。*/
   
        if (-1 == (cli_socket_fd = accept(srv_socket_fd,
                                          (struct sockaddr *)(&cli_sock_addr),  // 填充 Client Sock 信息。
                                          (socklen_t *)&cli_sockaddr_len)))
        {
            printf("Accept connection from client ERROR.\n");
            ERR_MSG(errno);
            exit(EXIT_FAILURE);
        }
 while (1)
    {
        /* 接收指定 Client Socket 发出的数据，*/
        memset(recv_buff, 0, BUF_LEN); 
        if ((recv_len = recv(cli_socket_fd, recv_buff, BUF_LEN, 0)) < 0)
        {
            printf("Receive from client ERROR.\n");
            ERR_MSG(errno);
            exit(EXIT_FAILURE);
        }
        printf("Receive data from client: %s\n", recv_buff);

        /* 发送ok给指定的 Client Socket。*/
      
       char send_buff[]={"Receive ok!\n"};
        send(cli_socket_fd, send_buff, BUF_LEN, 0);
        sleep(1);

        /* 每处理完一次 Client 请求，即关闭连接。*/
        //close(cli_socket_fd);
        
    }

   // close(srv_socket_fd);
    return EXIT_SUCCESS;
}
