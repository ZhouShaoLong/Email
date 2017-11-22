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

int mailManager::listMail(std::map<int, string> *map1) {
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
        temp = p;
        map1->insert(pair<int, string>(i, temp));
        //TODO 杨旭的任务：这里是得到的列表，使用top能够得到邮件的头部信息，解码存放到map1中
        socket->sendData("top 1");

    }

    return (int) map1->size();
}

//获取邮件正文，填入的参数是序号，序号是获取邮件列表的时候得到的
int mailManager::detailMail(char *index) {
    int i = 0, bytes = 0;
    cout << "-----邮件内容-----" << endl;
    char a[100] = "retr ";
    strcat(a, index);
    socket->sendData(a);
    socket->sendData("\r\n");
    socket->recvData(recvData, 17);
    if (strncmp(recvData, "+OK", 3) == 0) {
        cout << "获取邮件正文成功" << endl;
        while (recvData[i + 4] >= '0' && recvData[i + 4] <= '9') {
            bytes = bytes * 10 + (recvData[i + 4] - '0');
            i++;
        }
    } else if(strncmp(recvData, "-ERR", 4) == 0){
        return 0;
    }
    cout << "邮件内容有 " << bytes << " 字节" << endl;
    context = new char[bytes + 1];
    socket->recvData(context, bytes + 1);
    cout << context << endl;
    //TODO 杨旭的任务：context里存放的是邮件的头部和正文，将正文解码出来
    return bytes;
}