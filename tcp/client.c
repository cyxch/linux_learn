/**
 * client.c
 * 客户端
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 4096
#define PORT    8000

int main(void) {
    /*定义客户端套接字*/
    int cli_fd = -1;
    /*定义想连接的服务器地址*/
    struct sockaddr_in ser_addr;
    /*发送和接受缓冲区*/
    char sendbuf[MAXLINE], recvbuf[MAXLINE];

    /*初始化服务器套接字地址*/
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(PORT);
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /*创建套接字*/
    if((cli_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        /*创建失败*/
        printf("create socket error:%s(error:%d)\n", strerror(errno), errno);
        exit(0);
    }

    /*向服务器发送连接请求*/
    if((connect(cli_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr))) == -1) {
        printf("connect socket error:%s(error:%d)\n", strerror(errno), errno);
        exit(0);
    }

    while(1) {
        /*向服务器发送信息*/
        printf("向服务器发送信息：");
        fgets(sendbuf, sizeof(sendbuf), stdin);
        write(cli_fd, sendbuf, sizeof(sendbuf));

        /*从服务器接受信息*/
        ssize_t len = read(cli_fd, recvbuf, sizeof(recvbuf));
        if(len < 0) {
            if(errno == EINTR) {
                continue;
            }
            exit(0);
        }
        printf("服务器回复：%s\n", recvbuf);
    }

    close(cli_fd);
}