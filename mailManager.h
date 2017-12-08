//
// Created by 周绍龙 on 2017/11/15.
//

#ifndef LAB01_MAILMANAGER_H
#define LAB01_MAILMANAGER_H

#include <iostream>
#include <cstring>
#include <map>
#include <list>
#include "mailSocket.h"
#include "base64.h"

#define BUF_SIZE 1024

using namespace std;

struct FILEINFO{
    char fname[128];
    char fpath[256];
};

class mailManager {
public:
    explicit mailManager(mailSocket *socket);
    bool login_smtp(const char *email, const char *password);
    bool login_pop3(const char *email, const char *password);
    bool login_pop3SSL(const char *email, const char *password);
    int sendMail(const char *recipients, const char *subject, const char *content);
    int listMail(map<int, string> *list);
    int listMailSSL(map<int, string> *list);
    int detailMail(char *index);
    int detailMailSSL(char *index);
    int addAttachment(const char *file);
    int deleteAttachment(const char *file);
    int deleteAllAttachment();
    ~mailManager();

private:
    const char *email, *password;
    mailSocket *socket;
    char sendData[BUF_SIZE], recvData[BUF_SIZE];
    std::map<int, string> map;
    std::list<FILEINFO *> list;
    char *context;
    char *top;
};


#endif //LAB01_MAILMANAGER_H
