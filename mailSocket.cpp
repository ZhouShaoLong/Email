//
// Created by 周绍龙 on 2017/11/8.
//

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include "mailSocket.h"

mailSocket::mailSocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket error : " << std::endl;
        exit(-1);
    }
}

int mailSocket::Connect(const char *host, int port) {
    addr = gethostbyname(host);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    memcpy((char *) &server.sin_addr, (char *) addr->h_addr, (size_t) addr->h_length);

    if (connect(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr)) < 0) {
        std::cerr << "Connect error : " << std::endl;
        exit(-1);
    }


    return 1;
}

int mailSocket::sendData(const char *sendBuf) {
    int len = -1;
    if (sockfd < 0) {
        std::cerr << "链接套接字没有被初始化" << std::endl;
        return -1;
    }
    len = (int) send(sockfd, sendBuf, strlen(sendBuf), 0);
    return len;
}

int mailSocket::recvData(char *recvBuf, int size) {
    int len = -1;
    if (sockfd < 0) {
        std::cerr << "链接套接字没有被初始化" << std::endl;
        return -1;
    }

    struct timeval timeout = {3,0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(struct timeval));

    len = (int) recv(sockfd, recvBuf, (size_t) size, 0);
    return len;
}

mailSocket::~mailSocket() {
    close(sockfd);
}