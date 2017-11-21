#include <iostream>
#include <map>
#include "mailSocket.h"
#include "mailManager.h"
#include "base64.h"

#define BUF_SIZE 1024
#define SMTP_PORT 25
#define POP3_PORT 110

using namespace std;

int main() {
    int ret = 0;
    const char smtp[] = "smtp.163.com";
    const char pop3[] = "pop3.163.com";
    char email[] = "15520793998@163.com";
    char password[] = "Zhou9468";
    char recvData[BUF_SIZE];
    mailSocket mail;
    map<int,char*> list;


//    smtp发送邮件部分
//    ret = mail.Connect(smtp, SMTP_PORT);
//    if (ret != 1) {
//        cout << "Client : Connect 连接失败" << endl;
//        exit(-1);
//    } else {
//        cout << "Client : " << smtp << " 连接成功 " << endl;
//    }
//    mail.recvData(recvData, BUF_SIZE);
//
//    mailManager manager(&mail);
//
//    if (manager.login_smtp(email, password)) {
//        cout << "登录成功" << endl;
//    }
//
//    manager.sendMail("1281188154@qq.com", "Hello", "1111");


//    pop3接收邮件部分
    ret = mail.Connect(pop3, POP3_PORT);
    if (ret != 1) {
        cout << "Client : Connect 连接失败" << endl;
        exit(-1);
    } else {
        cout << "Client : " << pop3 << " 连接成功 " << endl;
    }
    mail.recvData(recvData, BUF_SIZE);
    mailManager manager(&mail);
    if (manager.login_pop3(email, password)) {
        cout << "登录成功" << endl;
    }
    manager.listMail(&list);
    for (int i = 1; i <= list.size() ; ++i) {
        cout<<list[i]<<endl;
    }

    return 0;
}