//
// Created by 周绍龙 on 2017/11/8.
//

#ifndef LAB01_MAILSOCKET_HPP
#define LAB01_MAILSOCKET_HPP

#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


class mailSocket {
public:
    mailSocket();
    int Connect(const char *host,int port);
    int sendData(const char *sendBuf);
    int recvData(char *recvBuf,int size);
    int sendDataSSL(const char *sendBuf);
    int recvDataSSL(char *recvBuf,int size);
    void ShowCerts(SSL * ssl);
    void initSSL();
    void createSSL();
    ~mailSocket();

private:
    int sockfd = -1;
    struct hostent *addr;
    struct sockaddr_in server;
    SSL_CTX *ctx;
    SSL *ssl;
};


#endif //LAB01_MAILSOCKET_HPP
