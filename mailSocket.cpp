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

int mailSocket::sendDataSSL(const char *sendBuf) {
    int len = -1;
    if (sockfd < 0) {
        std::cerr << "链接套接字没有被初始化" << std::endl;
        return -1;
    }
    len = SSL_write(ssl, sendBuf, strlen(sendBuf));
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

int mailSocket::recvDataSSL(char *recvBuf, int size) {
    int len = -1;
    if (sockfd < 0) {
        std::cerr << "链接套接字没有被初始化" << std::endl;
        return -1;
    }
    memset(recvBuf, 0, 102400);
    len = SSL_read(ssl, recvBuf, size);
    return len;
}

mailSocket::~mailSocket() {
    close(sockfd);
}

void mailSocket::ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        free(line);
        X509_free(cert);
    } else
        printf("无证书信息！\n");
}

void mailSocket::initSSL()
{
    /* SSL 库初始化*/
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
}

void mailSocket::createSSL()
{
    /* 基于 ctx 产生一个新的 SSL */
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    /* 建立 SSL 连接 */
    if (SSL_connect(ssl) == -1)
        ERR_print_errors_fp(stderr);
    else {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);
    }
}