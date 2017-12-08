//
// Created by 周绍龙 on 2017/11/15.
//

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
    this->email = email;
    this->password = password;

    socket->sendData("user ");
    socket->sendData(this->email);
    socket->sendData("\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData("pass ");
    socket->sendData(this->password);
    socket->sendData("\r\n");
    socket->recvData(recvData, BUF_SIZE);

    if (strncmp(recvData, "+OK", 3) == 0) {
        return true;
    } else if (strncmp(recvData, "+ERR", 4) == 0) {
        return false;
    }
}

bool mailManager::login_pop3SSL(const char *email, const char *password) {
    this->email = email;
    this->password = password;

    socket->sendDataSSL("user ");
    socket->sendDataSSL(this->email);
    socket->sendDataSSL("\r\n");
    socket->recvDataSSL(recvData, BUF_SIZE);

    socket->sendDataSSL("pass ");
    socket->sendDataSSL(this->password);
    socket->sendDataSSL("\r\n");
    socket->recvDataSSL(recvData, BUF_SIZE);

    if (strncmp(recvData, "+OK", 3) == 0) {
        return true;
    } else if (strncmp(recvData, "+ERR", 4) == 0) {
        return false;
    }
}

int mailManager::sendMail(const char *recipients, const char *subject, const char *content) {

    string sendString;

    socket->sendData("mail from <");
    socket->sendData(email);
    socket->sendData(">\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData("rcpt to <");
    socket->sendData(recipients);
    socket->sendData(">\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData("data\r\n");
    socket->recvData(recvData, BUF_SIZE);

    sendString = "From: ";
    sendString += email;
    sendString += "\r\n";

    sendString += "To: ";
    sendString += recipients;
    sendString += "\r\n";

    sendString += "Subject: ";
    sendString += subject;
    sendString += "\r\n";

    sendString += "MIME-Version: 1.0";
    sendString += "\r\n";

    sendString += "Content-Type: multipart/mixed;boundary=qwert";
    sendString += "\r\n";
    sendString += "\r\n";

    socket->sendData(sendString.c_str());

    sendString = "--qwert\r\n";
    sendString += "Content-Type: text/plain;";
    sendString += "charset=\"gb2312\"\r\n\r\n";
    sendString += content;
    sendString += "\r\n\r\n";

    socket->sendData(sendString.c_str());

    sendString = "--qwert--";
    sendString += "\r\n.\r\n";

    socket->sendData(sendString.c_str());
    socket->recvData(recvData, BUF_SIZE);

    return 0;
}

int mailManager::addAttachment(const char *file) {

    return 0;
}

int mailManager::deleteAttachment(const char *file) {
    return 0;
}

int mailManager::deleteAllAttachment() {
    return 0;
}

int mailManager::listMail(std::map<int, string> *map1) {
    map.clear();
    string temp;
    char *p;
    int i = 0;

    socket->sendData("stat\r\n");
    socket->recvData(recvData, BUF_SIZE);

    socket->sendData("list\r\n");
    socket->recvData(recvData, BUF_SIZE);
    cout<<recvData<<endl;
    socket->recvData(recvData, BUF_SIZE);
    cout<<recvData<<endl;
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
//        socket->sendData("top 1");

    }

    return (int) map1->size();
}

int mailManager::listMailSSL(std::map<int, string> *map1) {
    map.clear();
    string temp;
    char *p;
    int i = 0;

    socket->sendDataSSL("stat\r\n");
    socket->recvDataSSL(recvData, BUF_SIZE);

    socket->sendDataSSL("list\r\n");
    socket->recvDataSSL(recvData, BUF_SIZE);
    cout<<recvData<<endl;
    socket->recvDataSSL(recvData, BUF_SIZE);
    cout<<recvData<<endl;
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
//        socket->sendData("top 1");

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
    socket->recvData(recvData, 18);
    if (strncmp(recvData, "+OK", 3) == 0) {
        cout << "获取邮件正文成功" << endl;
        while (recvData[i + 4] >= '0' && recvData[i + 4] <= '9') {
            bytes = bytes * 10 + (recvData[i + 4] - '0');
            i++;
        }
    } else if (strncmp(recvData, "-ERR", 4) == 0) {
        return 0;
    }
    cout << "邮件内容有 " << bytes << " 字节" << endl;
    context = new char[bytes + 1];

    while (socket->recvData(recvData, BUF_SIZE) > 0) {
        strncat(context, recvData, BUF_SIZE);
        memset(recvData, 0, BUF_SIZE);
    }

    cout << context;

    //TODO 杨旭的任务：context里存放的是邮件的头部和正文，将正文解码出来
    return bytes;
}

int mailManager::detailMailSSL(char *index) {
    int i = 0, bytes = 0;
    cout << "-----邮件内容-----" << endl;
    char a[100] = "retr ";
    strcat(a, index);
    socket->sendDataSSL(a);
    socket->sendDataSSL("\r\n");
    socket->recvDataSSL(recvData, 18);
    if (strncmp(recvData, "+OK", 3) == 0) {
        cout << "获取邮件正文成功" << endl;
        while (recvData[i + 4] >= '0' && recvData[i + 4] <= '9') {
            bytes = bytes * 10 + (recvData[i + 4] - '0');
            i++;
        }
    } else if (strncmp(recvData, "-ERR", 4) == 0) {
        return 0;
    }
    cout << "邮件内容有 " << bytes << " 字节" << endl;
    context = new char[bytes + 1];

    while (socket->recvDataSSL(recvData, BUF_SIZE) > 0) {
        strncat(context, recvData, BUF_SIZE);
        memset(recvData, 0, BUF_SIZE);
    }

    cout << context;

    //TODO 杨旭的任务：context里存放的是邮件的头部和正文，将正文解码出来
    return bytes;
}

mailManager::~mailManager() {
    socket->sendData("quit\r\n");
    socket->recvData(recvData, BUF_SIZE);
}