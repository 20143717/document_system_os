#ifndef USER_H
#define USER_H

#include <QString>
#include <QFile>
#include <QTextCodec>
#include <QDebug>
#include <QTextCodec>
#include <QTextStream>
#include <QIODevice>

struct User{
    int id;
    QString name;
    QString password;
    bool power;
};

class USER
{
public:
    USER() {}
    ~USER() {}
    int user_number;
    User ordinary_user[10];
    User login_user;
    void format_user(){
        ordinary_user[0].id=0;
        ordinary_user[0].name="admin";
        ordinary_user[0].password="admin";
        ordinary_user[0].power=1;
        user_number=0;
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QFile file("./user.txt");
        QTextStream stream(&file);
        int ans=0;
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"WA"<<endl;
        }
        while(!stream.atEnd()){
            ans++;
            QString a=stream.readLine();
            QString b=stream.readLine();
            //qDebug()<<a<<"!!!"<<b<<endl;
            ordinary_user[ans].name=a;
            ordinary_user[ans].password=b;
            ordinary_user[ans].id=ans;
            ordinary_user[ans].power=0;
        }
        user_number=ans;
        file.close();
    }
};

#endif // USER_H
