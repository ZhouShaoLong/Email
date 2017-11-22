//
// Created by 周绍龙 on 2017/11/15.
//

#include <iostream>
#include <cstring>
#include "mailManager.h"

using namespace std;

mailManager::mailManager(mailSocket *socket) {
    if (socket != nullptr) {
        this->socket = socket;
    } else {
        std::cerr << "mailSocket未初始化" << std::endl;
        exit(-1);
    }
}

bool mailManager::login_smtp(const char *email, const char *password) {
    int retCode = 0, i = 0;

    this->email = email;
    this->password = password;

    socket->sendData("HELO ");
    //HELO后面有个空格
    socket->sendData(email);
    socket->sendData("\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData("auth login\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData(base64_encode((char *) email, (int) strlen(email)).c_str());
    socket->sendData("\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData(base64_encode((char *) password, (int) strlen(password)).c_str());
    socket->sendData("\r\n");
    socket->recvData(recvData, BUF_SIZE);

    for (i = 0; i < 3; ++i) {
        retCode = retCode * 10 + (recvData[i] - '0');
    }


    if (retCode != 235) {
        std::cout << "error : " << recvData << std::endl;
        return false;
    }

    return true;
}

bool mailManager::login_pop3(const char *email, const char *password) {
    socket->sendData("user ");
    socket->sendData(email);
    socket->sendData("\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData("pass ");
    socket->sendData(password);
    socket->sendData("\r\n");
    socket->recvData(recvData, BUF_SIZE);

    if (strncmp(recvData, "+OK", 3) == 0) {
        return true;
    } else if (strncmp(recvData, "+ERR", 4) == 0) {
        return false;
    }
}

int mailManager::sendMail(const char *recipients, const char *subject, const char *content) {

    socket->sendData("mail from <");
    socket->sendData(email);
    socket->sendData(">\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData("rcpt to <");
    socket->sendData("1281188154@qq.com");
    socket->sendData(">\r\n");
    socket->recvData(recvData, BUF_SIZE);


    socket->sendData("data\r\n");
    socket->recvData(recvData, BUF_SIZE);
    cout << recvData << endl;

    socket->sendData("subject:");
    socket->sendData("1\r\n");
    socket->sendData("\r\n\r\n");
    socket->sendData("2\r\n");
    socket->sendData(".\r\n");
    socket->recvData(recvData, BUF_SIZE);
    cout << recvData << endl;

    socket->sendData("quit\r\n");
    socket->recvData(recvData, BUF_SIZE);
    cout << "Server : " << recvData << endl;

    return 0;
}

int mailManager::listMail(std::map<int, char *> *map1) {
    list.clear();
    string temp;
    char *p;
    int i = 0;

    socket->sendData("stat\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData("list\r\n");
    socket->recvData(recvData, BUF_SIZE);

    p = strtok(recvData, "\r\n");
    while (p) {
        i++;
        p = strtok(nullptr, "\r\n");
        if (*p == '.') {
            break;
        }
        map1->insert(pair<int, char *>(i, p));
    }

    socket->sendData("quit\r\n");
    socket->recvData(recvData, BUF_SIZE);


    return (int) map1->size();
}
