//
// Created by shengym on 2019-07-07.
//

#include "lib/common.h"

static int count;

static void recvfrom_int(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}

int main(int argc, char **argv) {
    printf("服务端启动\n");
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    printf("套接字描述符：%d\n", socket_fd);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);

    bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    socklen_t client_len;
    char message[MAXLINE];
    count = 0;

    signal(SIGINT, recvfrom_int); //信号处理函数。目的是在响应“Ctrl+C”退出时，打印出收到的报文总数

    struct sockaddr_in client_addr;
    client_len = sizeof(client_addr);
    for (;;) {
        int n = recvfrom(socket_fd, message, MAXLINE, 0, (struct sockaddr *) &client_addr, &client_len);
        message[n] = 0;
        printf("received %d bytes: %s\n", n, message);

        /*******计算客户端IP地址*******/
        int client_ip[4] = {0, 0, 0, 0};
        in_addr_t c_addr = client_addr.sin_addr.s_addr;
        for (int i = 0; i < 4 && c_addr != 0; i++){
            client_ip[i] = c_addr & 0xff;
            c_addr = c_addr >> 8;
        }
        printf("客户端IP地址：%d.%d.%d.%d\n", client_ip[0], client_ip[1], client_ip[2], client_ip[3]);

        char send_line[MAXLINE];
        sprintf(send_line, "Hi, %s", message);

        sendto(socket_fd, send_line, strlen(send_line), 0, (struct sockaddr *) &client_addr, client_len);

        count++;
    }

}


