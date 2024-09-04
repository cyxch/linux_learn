/**
 * server.c
 * 服务端
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 4096
#define PORT    8000

int main(void) {
    /*定义服务器监听套接字，连接套接字*/
    int listen_fd = -1, connect_fd = -1;
    /*定义服务器对应的套接字地址*/
    struct sockaddr_in ser_addr;
    /*服务器接收和发送缓冲区*/
    char sendbuf[MAXLINE], recvbuf[MAXLINE];

    /*初始化套接字地址结构体*/
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(PORT);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /*创建套接字*/
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        /*创建失败*/
        printf("create socket error:%s(error:%d)\n", strerror(errno), errno);
        exit(0);
    }

    /*绑定套接字和本地IP地址，端口*/
    if(bind(listen_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) == -1) {
        /*绑定失败*/
        printf("bind socket error:%s(errno:%d)\n", strerror(errno), errno);
        exit(0);
    }

    /*监听*/
    if(listen(listen_fd, 10) == -1) {
        printf("listen socket error:%s(errno:%d)\n", strerror(errno), errno);
        exit(0);
    }

    printf("等待客户端发起连接请求\n");

    while(1) {
        /*接受客户端请求*/
        if((connect_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL)) == -1) {
            printf("accept socket error:%s(errno:%d)\n", strerror(errno), errno);
            continue;
        }

        while(1) {
            /*读取客户端发送的信息*/
            ssize_t len = read(connect_fd, recvbuf, sizeof(recvbuf));
            if(len < 0) {
                if(errno == EINTR) {
                    continue;
                }
                exit(0);
            }

            printf("接受客户端请求：%s\n", recvbuf);

            /*向客户端发送消息*/
            printf("回复客户端消息：");
            fgets(sendbuf, sizeof(sendbuf), stdin);
            write(connect_fd, sendbuf, sizeof(sendbuf));
        }

        close(connect_fd);
    }

    close(listen_fd);
}