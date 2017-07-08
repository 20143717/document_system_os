#include "admin.h"
#include "ui_admin.h"
#include <qlistwidget.h>
#include <qlistview.h>
#include "getnewuser.h"
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QtWidgets/QListWidget>
#include "user.h"

extern USER user;
admin::admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin)
{
    ui->setupUi(this);
}

admin::~admin()
{
    delete ui;
}

void admin::on_pushButton_1_clicked(){
    getnewuser.show();
}

void admin::on_pushButton_2_clicked(){
    deleteolduser.show();
}

void admin::on_pushButton_3_clicked(){
    this->reject();
}
void admin::on_pushButton_4_clicked(){
    ui->list->clear();
    //qDebug("%d",user.user_number);
    for(int i=1;i<=user.user_number;++i){
        QListWidgetItem *a = new QListWidgetItem;
        a->setText("用户名为:"+user.ordinary_user[i].name+"  密码为:"+user.ordinary_user[i].password);
        ui->list->addItem(a);
    }
}
