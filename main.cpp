#include <iostream>
#include <map>
#include "mailSocket.h"
#include "mailManager.h"

#define BUF_SIZE 1024
#define SMTP_PORT 465   //无ssl为25
#define POP3_PORT 995   //无ssl为110 有ssl为995

using namespace std;

int main() {
    int ret = 0;
    const char smtp[] = "smtp.qq.com";
    const char pop3[] = "pop.qq.com";
    //char email[] = "18756645662@163.com";
    //char password[] = "xmf1261616671";
    char email[] = "menfer@foxmail.com";
    char password[] = "kqklxgrsfwyuffja";
    char recvData[BUF_SIZE];
    mailSocket mail;
    map<int, string> list;


    //smtp发送邮件部分
    mail.initSSL();
    mail.createSSL();
    mailManager manager(&mail);
    ret = mail.Connect(smtp, SMTP_PORT);
    if (ret != 1) {
        cout << "Client : Connect 连接失败" << endl;
        exit(-1);
    } else {
        cout << "Client : " << smtp << " 连接成功 " << endl;
    }
    //mail.recvDataSSL(recvData, BUF_SIZE);


    if (manager.login_smtpSSL(email, password)) {
        cout << "登录成功" << endl;
    }

    manager.sendMailSSL("1261616671@qq.com", "Hello", "Hello\nMy Friend");


//    pop3接收邮件部分
    /*ret = mail.Connect(pop3, POP3_PORT);
    if (ret != 1) {
        cout << "Client : Connect 连接失败" << endl;
        exit(-1);
    } else {
        cout << "Client : " << pop3 << " 连接成功 " << endl;
    }
    mail.recvData(recvData, BUF_SIZE);
    mailManager manager(&mail);
    mail.initSSL();
    mail.createSSL();
    if (manager.login_pop3SSL(email, password)) {
        cout << "登录成功" << endl;
    }*/
/*    manager.listMailSSL(&list);
    for (int i = 1; i <= list.size(); ++i) {
        cout << list[i] << endl;
    }*/
    return 0;
}