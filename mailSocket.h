//
// Created by 周绍龙 on 2017/11/8.
//

#ifndef LAB01_MAILSOCKET_HPP
#define LAB01_MAILSOCKET_HPP

#include <netinet/in.h>


class mailSocket {
public:
    mailSocket();
    int Connect(const char *host,int port);
    int sendData(const char *sendBuf);
    int recvData(char *recvBuf,int size);
    ~mailSocket();

private:
    int sockfd = -1;
    struct hostent *addr;
    struct sockaddr_in server;
};


#endif //LAB01_MAILSOCKET_HPP
