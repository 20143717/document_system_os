#include "deleteolduser.h"
#include "ui_deleteolduser.h"
#include "user.h"
#include <QMessageBox>
extern USER user;

DeleteOldUser::DeleteOldUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteOldUser)
{
    ui->setupUi(this);
}

DeleteOldUser::~DeleteOldUser()
{
    delete ui;
}

void DeleteOldUser::on_certain_clicked(){
    QString name=ui->name->text();
    QString password=ui->password->text();
    bool exist=0;
    int pos=0;
    for(int i=1;i<=user.user_number;++i){
        if(user.ordinary_user[i].name==name){
            exist=1;
            pos=i;break;
        }
    }
    if(!exist){
        QMessageBox::information(this,"提示","该用户不存在!");
        ui->name->clear();
        ui->password->clear();
    }
    else{
        if(password!=user.ordinary_user[0].password){
            QMessageBox::information(this,"提示","管理员密码错误!");
            ui->name->clear();;
            ui->password->clear();
        }
        else{
            user.user_number--;
            for(int i=pos;i<=user.user_number;++i){
                user.ordinary_user[i].id=user.ordinary_user[i+1].id;
                user.ordinary_user[i].name=user.ordinary_user[i+1].name;
                user.ordinary_user[i].password=user.ordinary_user[i+1].password;
                user.ordinary_user[i].power=user.ordinary_user[i+1].power;
            }
            QFile fll("./user.txt");
            QTextStream stream(&fll);
            if(fll.open(QFile::WriteOnly| QIODevice::Truncate)){
            }
            for(int i=1;i<=user.user_number;++i){
                fll.close();
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append)){
                    stream<<user.ordinary_user[i].name<<endl;
                    stream<<user.ordinary_user[i].password<<endl;
                }
            }
            fll.close();
            QMessageBox::information(this,"提示","成功删除该用户");
            ui->name->clear();
            ui->password->clear();
        }
    }
}

void DeleteOldUser::on_clear_clicked(){
    ui->name->clear();
    ui->password->clear();
}
