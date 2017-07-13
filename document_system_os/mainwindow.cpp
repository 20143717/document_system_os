#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include <QMessageBox>
#include <QDir>
#include <string>
#include "admin.h"
#include "getnewuser.h"
#include "user.h"
#include "file_system.h"

USER user;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->File_system.hide();
    File_system.setParent(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    //qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
    //qDebug()<<"current currentPath: "<<QDir::currentPath();
    user.format_user();
    user.login_user.name=ui->lineEdit->text();
    user.login_user.password=ui->lineEdit_2->text();
    if(user.login_user.name == user.ordinary_user[0].name && user.login_user.password == user.ordinary_user[0].password){
        QMessageBox::information(this,"提示","管理员登录成功!");
        Admin.show();
    }
    for(int i=1;i<=user.user_number;++i){
        if(user.login_user.name== user.ordinary_user[i].name && user.login_user.password == user.ordinary_user[i].password){
            user.login_user.id=i;
            QMessageBox::information(this,"提示","用户登录成功!");
            //File_system.show();
            this->resize(634,449);
            this->File_system.show();

        }
    }
}
