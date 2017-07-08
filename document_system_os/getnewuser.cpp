#include "getnewuser.h"
#include "ui_getnewuser.h"
#include <QMessageBox>
#include "user.h"
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>

extern USER user;

GetNewUser::GetNewUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetNewUser)
{
    ui->setupUi(this);
}

GetNewUser::~GetNewUser()
{
    delete ui;
}

void GetNewUser::on_certain_clicked(){
    QString name_new=ui->name->text();
    QString password_new=ui->password->text();
    QString password_again=ui->again->text();
    if(password_again!=password_new){
        QMessageBox::information(this,"提示","两次密码不一致，请重新输入");
        ui->name->clear();
        ui->password->clear();
        ui->again->clear();
    }
    else{
        /*QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QFile file("./user.txt");
        if(!file.open(QIODevice::ReadWrite|QIODevice::Text)){
            qDebug()<<"Can't open the file!"<<endl;
        }
        QTextStream stream(&file);
        if(!stream.atEnd()){
            QString a=stream.readLine();
            user.user_number=a.toInt();
        }
        int ans=0;
        while(!stream.atEnd()){
            ans++;
            QString a=stream.readLine();
            QString b=stream.readLine();
            user.ordinary_user[ans].name=a;
            user.ordinary_user[ans].password=b;
            user.ordinary_user[ans].id=ans;
            user.ordinary_user[ans].power=0;
        }
        file.close();*/
        //qDebug()<<"!"<<user.user_number;
        bool f=0;
        for(int i=1;i<=user.user_number;++i){
            if(user.ordinary_user[i].name==name_new){
                QMessageBox::information(this,"提示","该用户名已存在，请重新输入");
                ui->name->clear();
                ui->password->clear();
                ui->again->clear();
                f=1;
            }
        }
        if(f==0){
            user.user_number++;

            user.ordinary_user[user.user_number].name=name_new;
            user.ordinary_user[user.user_number].password=password_new;
            user.ordinary_user[user.user_number].power=0;
            user.ordinary_user[user.user_number].id=user.user_number;

            QFile fll("./user.txt");
            if(!fll.open(QIODevice::ReadWrite|QIODevice::Text)){
                qDebug()<<"WA"<<endl;
            }
            QTextStream stream(&fll);
            if(fll.open(QFile::WriteOnly| QIODevice::Truncate)){

                stream<<user.user_number<<endl;
            }
            else qDebug()<<"MMP";
            for(int i=1;i<=user.user_number;++i){
                if(fll.open(QIODevice::ReadWrite | QIODevice::Truncate)){
                    stream<<user.ordinary_user[i].name;
                    stream<<user.ordinary_user[i].password;
                }
            }
            fll.close();

            QMessageBox::information(this,"提示","成功添加该用户");
            ui->name->clear();
            ui->password->clear();
            ui->again->clear();
        }
    }
}

void GetNewUser::on_clear_clicked(){
    ui->name->clear();
    ui->password->clear();
    ui->again->clear();
}
