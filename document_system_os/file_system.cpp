#include "file_system.h"
#include "ui_file_system.h"
#include <QDebug>
#include <QTextStream>
#include <windows.h>
#include <time.h>
#include <QTextCodec>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <qlistwidget.h>
#include <QInputDialog>
#include <QMenu>
#include <queue>
#include "user.h"
#include "define.h"
#include "struct.h"
#include "free_block_link.h"

extern Super_Block super_block;
extern Main_File_Directory MFD;
extern Symbol_File_Directory SFD;
extern iNode inode[INode_Num];
extern Symbol_File_Directory sfdTable[Directory_Num];
extern block Z[Block_Number-Start_Block_Num];//ont  group
extern QVector<QString>PWD;
extern USER user;

file_system::file_system(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::file_system)
{
    ui->setupUi(this);
    choose_name="/";
    choose_type="";
}

file_system::~file_system()
{
    delete ui;
}
void file_system::showEvent(QShowEvent *){
    QMessageBox::information(this,"提示","欢迎用户"+user.login_user.name+"登录!");
    ui->listWidget->setIconSize(QSize(80,70));
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    //run();
    ui->login_name->setText(user.login_user.name);
    ui->path->setText("/");
}
void file_system::contextMenuEvent(QContextMenuEvent * event){

    QMenu* popMenu = new QMenu(this);
    QListWidgetItem *choose_item = ui->listWidget->itemAt(event->x()-ui->listWidget->x(),event->y()-ui->listWidget->y());
    if(choose_item!=NULL){
        choose_name=choose_item->text();
        if(choose_item->whatsThis()=="file"){
            QAction *action1= new QAction("打开文件",this);
            QAction *action2= new QAction("删除文件",this);
            QAction *action5=new QAction("属性", this);
            connect(action1,SIGNAL(triggered()),this,SLOT(cat(choose_name)));
            connect(action2,SIGNAL(triggered()),this,SLOT(rm(choose_name)));
            connect(action5,SIGNAL(triggered()),this,SLOT(ll()));
            popMenu->addAction(action1);
            popMenu->addAction(action2);
            popMenu->addAction(action5);
            choose_type="file";
        }
        else{
            QAction *action1=new QAction("打开文件夹", this);
            QAction *action2=new QAction("删除文件夹", this);
            QAction *action5=new QAction("属性", this);
            QString file_name=choose_item->text();
            connect(action1,SIGNAL(triggered()),this,SLOT(cd(choose_name)));
            connect(action2,SIGNAL(triggered()),this,SLOT(rmf(choose_name)));
            connect(action5,SIGNAL(triggered()),this,SLOT(ls()));
            popMenu->addAction(action1);
            popMenu->addAction(action2);
            popMenu->addAction(action5);
            choose_type="folder";
        }
    }
    QAction *action3=new QAction("新建文件", this);
    connect(action3,SIGNAL(triggered()),this,SLOT(touch()));
    QAction *action4=new QAction("新建文件夹", this);
    connect(action4,SIGNAL(triggered()),this,SLOT(mkdir()));
    popMenu->addAction(action3);
    popMenu->addAction(action4);
    popMenu->exec(QCursor::pos());
}

void file_system::format(){
    switch(QMessageBox::question(this,"Warning","ALL DATA ON THIS FILESYSTEM WILL BE LOST!\nProceed with Format(Y/N)?",
                          QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok)){
        case QMessageBox::Ok:{
            //(2) init super_block
            /*if ((fp=fopen(SYSTEM, "wb+")) == NULL)
            {
                printf("Can't create file %s\n", SYSTEM);
                exit(0);
            }*/
            //ofstream ofstr4("data.txt", ios::out);
            QFile fll("./data.txt");
            QTextStream stream(&fll);
            if(!fll.open(QIODevice::WriteOnly | QIODevice::Append)){
                qDebug()<<"WA"<<endl;
            }
            fll.close();
            //init superblock
            super_block.id = 1;
            super_block.iNodeTotalNum = 32;
            super_block.iNodeFreeNum = 29;
            memset(super_block.iNodeFreeStack,0, sizeof(super_block.iNodeFreeStack));
            super_block.dataBlockTotalNum = 990;
            super_block.dataBlockFreeNum = 988;
            super_block.iNodeFreeStack[0] = 1;
            super_block.iNodeFreeStack[1] = 1;
            super_block.iNodeFreeStack[2] = 1;
            memset(super_block.dataBlockFreeStack, -1, sizeof(super_block.dataBlockFreeStack));
            //super_block.superBlockFlag = 0;
            if(fll.open(QIODevice::WriteOnly | QIODevice::Append)){
            stream << super_block.id << " " << super_block.iNodeTotalNum << " " <<
                      super_block.iNodeFreeNum << " " << super_block.dataBlockTotalNum << " " <<
                      super_block.dataBlockFreeNum <<" "<<endl;
            }
            for (int i = 0; i < INode_Free_Stack_Num; i++) {
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                stream << super_block.iNodeFreeStack[i] << " ";
            }
            if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                stream <<endl;

            for (int i = 0; i < Data_Block_Free_Stack_Num; i++) {
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                stream << super_block.dataBlockFreeStack[i] << " ";
            }
            if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
            stream << endl;

            /*there are some problem*/

            //(3) int Inode
            inode[0].id = 0;
            inode[0].fileCount = 0;
            inode[0].size = 0;
            inode[0].fileMode = -1;
            inode[0].userId = -1;
            memset(inode[0].userRight,-1,sizeof(inode[0].userRight));
            time_t t = time(0);
            strftime(inode[0].time.tdate,sizeof(inode[0].time.tdate),"%Y/%m/%d",localtime(&t));
            strftime(inode[0].time.ttime,sizeof(inode[0].time.ttime),"%X",localtime(&t));
            memset(inode[0].diskAddress,-1,sizeof(inode[0].diskAddress));

            inode[1].id = 1;
            inode[1].fileCount = 0;
            inode[1].size = 0;
            inode[1].fileMode = 0;
            inode[1].userId = 0;
            for(int i=0;i<Directory_Item_Num;i++) {
                if(i == 0 ) {
                    inode[1].userRight[i] = 7;
                } else {
                    inode[1].userRight[i] = 0;
                }
            }
            t = time(0);
            strftime(inode[1].time.tdate,sizeof(inode[1].time.tdate),"%Y/%m/%d",localtime(&t));
            strftime(inode[1].time.ttime,sizeof(inode[1].time.ttime),"%X",localtime(&t));
            memset(inode[1].diskAddress,-1,sizeof(inode[1].diskAddress));

            inode[2].id = 2;
            inode[2].fileCount = 0;
            inode[2].size = 0;
            inode[2].fileMode = 0;
            inode[2].userId = 0;
            for(int i=0;i<Directory_Item_Num;i++) {
                if(i == 0 ) {
                    inode[2].userRight[i] = 7;
                } else {
                    inode[2].userRight[i] = 0;
                }
            }
            t = time(0);
            strftime(inode[2].time.tdate,sizeof(inode[2].time.tdate),"%Y/%m/%d",localtime(&t));
            strftime(inode[2].time.ttime,sizeof(inode[2].time.ttime),"%X",localtime(&t));
            memset(inode[2].diskAddress,-1,sizeof(inode[2].diskAddress));
            inode[2].diskAddress[0] = 0;

            for(int i=3;i<INode_Num;i++) {
                inode[i].id = i;
                inode[i].fileCount = 0;
                inode[i].size = 0;
                inode[i].fileMode = -1;
                inode[i].userId = -1;
                memset(inode[i].userRight,-1,sizeof(inode[i].userRight));
                strcpy(inode[i].time.tdate, "/");
                strcpy(inode[i].time.ttime,"/");
                memset(inode[i].diskAddress,-1,sizeof(inode[i].diskAddress));
            }

            for (int i = 0; i<INode_Num; i++)
            {
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append)){
                stream << inode[i].id << " " << inode[i].fileCount << " " << inode[i].size << " " <<
                          inode[i].fileMode << " " << inode[i].userId << " ";}
                for(int j=0;j<Directory_Item_Num;j++) {
                    if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                        stream << inode[i].userRight[j]<<" ";
                }
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                    stream<<inode[i].time.tdate<<" "<<inode[i].time.ttime<<" ";
                for (int j=0; j < INode_Max_Num; j++)
                {
                    if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                        stream << inode[i].diskAddress[j] << " ";
                }
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                stream << endl;
            }

            MFD.init();  // only one mfd
            SFD.init();  //

            // write into a mfd
            MFD.item[0].name = "admin";
            MFD.item[0].psw="admin";
            MFD.item[0].iNode = 1;
            for (int i=0; i < MFD.size(); i++)//mfd
            {
                Main_File_Directory_Item item = MFD.get(i);
                if(i!=0) item.psw="/";
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                stream <<item.name<<" "<<item.psw<<" " << item.iNode << " ";
            }
            if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
            stream << endl;

            sfdTable[0].iNode = 2;
            for(int i=0;i<Directory_Num;i++) {
                for(int j=0;j<SFD.size();j++) {
                    sfdTable[i].item[j].name = "/";
                    sfdTable[i].item[j].iNode = -1;
                }
            }
            for(int i=0;i<Directory_Num;i++) {
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                stream << sfdTable[i].iNode<<" ";
                for (int j = 0; j < SFD.size(); j++)
                {
                    if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                    stream <<sfdTable[i].item[j].name<<" " << sfdTable[i].item[j].iNode<<" ";
                }
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                stream << endl;
            }

            //ofstr4 << "block"<<endl;

            // init freeblcok

            for(int i=0;i<Block_Number;i++) {
                for(int j=0;j<Block_Size;j++) {
                    Z[i].R[j] = '/';
                }
            }
            for(int i=0;i<Block_Number;i++) {
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                stream<<Z[i].R<<endl;
            }
            fll.flush();
            fll.close();
            PWD.clear();
        break;
    }
    case QMessageBox::Cancel:{
        QFile sys;
        if (!sys.exists()) {
            qDebug()<<"error: disk hasn't formatted!"<<endl;
            exit(0);
        }
        PWD.clear();
        break;}
    defalut:
        break;
    }
}

void file_system::run(){
    init();
    QString order;
    QString pwd_tmp = "/root/";
    qDebug()<<user.login_user.name<<"@";
    QVector<QString>::iterator it;
    for(it = PWD.begin();it!=PWD.end();it++) {
        qDebug()<<"/"<<*it;
    }
    qDebug()<<"$ ";
    //用map参照
    /*while (true)
    {
        if(order.find("cat") == 0) {
            QString file = "";
            int i=3;
            while(order[i] == ' ' && i<order.size()) {
                i++;
            }
            if(i < order.size()) {
                for(;i<order.size();i++) {
                    if(order[i] == ' ')
                            break;
                    file+=order[i];
                }
                    if(file.size() > 0) {
                        cat(file);
                    }
                }
            } else if(order.find("cd") == 0) {
                QString dir = "";
                int i=2;
                while(order[i] == ' ' && i<order.size()) {
                    i++;
                }
                if(i < order.size()) {
                    for(;i<order.size();i++) {
                        if(order[i] == ' ')
                            break;
                        dir+=order[i];
                    }
                    if(dir.size() > 0) {
                        cd(dir);
                    }
                }
            } else if(order == "exit") {
                exit(0);
            } else if(order == "format"){
                sudoFormat() }
            } else if(order == "ll") {
                ll();
            } else if(order == "logout") {
                logout();
            } else if(order == "ls") {
                ls();
            } else if(order.find("mkdir") == 0) {
                QString dir = "";
                int i=5;
                while(order[i] == ' ' && i<order.size()) {
                    i++;
                }
                if(i < order.size()) {
                    for(;i<order.size();i++) {
                        if(order[i] == ' ')
                            break;
                        dir+=order[i];
                    }
                    if(dir.size() > 0) {
                        mkdir(dir);
                    }
                }
            } else if(order == "pwd") {
                pwd();
            } else if(order == "register") {
                testAndRegister();
            } else if(order.find("renamef") == 0) {
                QString dir1 = "";
                QString dir2 = "";
                int i=7;
                while(order[i] == ' ' && i<order.size()) {
                    i++;
                }
                if(i < order.size()) {
                    for(;i<order.size();i++) {
                        if(order[i] == ' ')
                            break;
                        dir1+=order[i];
                    }
                    while(order[i] == ' ' && i<order.size()) {
                        i++;
                    }
                    if(i < order.size()) {
                        for(;i<order.size();i++) {
                            if(order[i] == ' ')
                                break;
                            dir2+=order[i];
                        }
                        if(dir1.size() > 0&&dir2.size()>0) {
                            renamef(dir1,dir2);
                        }
                    }
                }
            } else if(order.find("rename") == 0) {
                QString dir1 = "";
                QString dir2 = "";
                int i=6;
                while(order[i] == ' ' && i<order.size()) {
                    i++;
                }
                if(i < order.size()) {
                    for(;i<order.size();i++) {
                        if(order[i] == ' ')
                            break;
                        dir1+=order[i];
                    }
                    while(order[i] == ' ' && i<order.size()) {
                        i++;
                    }
                    if(i < order.size()) {
                        for(;i<order.size();i++) {
                            if(order[i] == ' ')
                                break;
                            dir2+=order[i];
                        }
                        if(dir1.size() > 0&&dir2.size()>0) {
                            rename(dir1,dir2);
                        }
                    }
                }
            } else if(order.find("rmf") == 0) {
                QString dir = "";
                int i=3;
                while(order[i] == ' ' && i<order.size()) {
                    i++;
                }
                if(i < order.size()) {
                    for(;i<order.size();i++) {
                        if(order[i] == ' ')
                            break;
                        dir+=order[i];
                    }
                    if(dir.size() > 0) {
                        rmf(dir);
                    }
                }
            } else if(order.find("touch") == 0) {
                QString file = "";
                int i=5;
                while(order[i] == ' ' && i<order.size()) {
                    i++;
                }
                if(i < order.size()) {
                    for(;i<order.size();i++) {
                        if(order[i] == ' ')
                            break;
                        file+=order[i];
                    }
                    if(file.size() > 0) {
                        touch(file);
                    }
                }
            } else if(order.find("vim") == 0) {
                QString file = "";
                int i=3;
                while(order[i] == ' ' && i<order.size()) {
                    i++;
                }
                if(i < order.size()) {
                    for(;i<order.size();i++) {
                        if(order[i] == ' ')
                            break;
                        file+=order[i];
                    }
                    if(file.size() > 0) {
                        vim(file);
                    }
                }
            }

            QString pwd_tmp = "/root/";
            //cout<<loginUser.username<<"@";
            QVector<QString>::iterator it;
            for(it = PWD.begin();it!=PWD.end();it++) {
                cout<<"/"<<*it;
            }
            cout<<"$ ";}*/
}

void file_system::init(){
    QFile file("./data.txt");
    if (!file.exists())
    {
        format();
        load();
        char tmp[2];
        gets(tmp);
    }
    else
    {
        load();
    }
}

void file_system::load(){
    QFile file("./data.txt");
    QTextStream stream(&file);

    stream >> super_block.id >> super_block.iNodeTotalNum >>
              super_block.iNodeFreeNum >> super_block.dataBlockTotalNum >>
              super_block.dataBlockFreeNum ;
    for (int i = 0; i < INode_Free_Stack_Num; i++) {
        stream >> super_block.iNodeFreeStack[i];
        qDebug() << super_block.iNodeFreeStack[i];
    }

    for (int i = 0; i < Data_Block_Free_Stack_Num; i++) {
        stream >> super_block.dataBlockFreeStack[i];
        qDebug() << super_block.dataBlockFreeStack[i];
    }
    for (int i = 0; i<INode_Num; i++)
    {

        stream >> inode[i].id >> inode[i].fileCount >> inode[i].size >>
                  inode[i].fileMode >> inode[i].userId;
        for(int j=0;j<Directory_Item_Num;j++) {
            stream >> inode[i].userRight[j];
        }
        stream >> inode[i].time.tdate>>inode[i].time.ttime;
        for (int j = 0; j < INode_Max_Num; j++)
        {
            stream >> inode[i].diskAddress[j];
        }
    }
    for (int i = 0; i < MFD.size(); i++)//mfd
    {
        Main_File_Directory_Item item;
        stream >> item.name >> item.psw >> item.iNode ;
        if (i == 0) {
            MFD.item[0].iNode = 2;
            MFD.item[0].name=user.login_user.name;
            MFD.item[0].psw=user.login_user.password;
        }
        else
        {
            MFD.add(item.name, item.iNode);
            MFD.item[i].psw=item.psw;
        }
    }

    for (int j = 0; j < Directory_Num; j++) {//sfd
        stream >> sfdTable[j].iNode;
        qDebug() << sfdTable[j].iNode;
        for (int i = 0; i < SFD.size(); i++)
        {
            Symbol_File_Directory_Item item;
            stream >> item.name >> item.iNode   ;
            qDebug() << item.iNode<<" "<<item.name;
            sfdTable[j].add(item.name, item.iNode);
            qDebug() << sfdTable[j].item[i].name << endl;
        }
    }

    //cout << sfdtable[0].id;
    //char tmp[20];
    //ifstr2>>tmp;
    for(int i=0;i<Block_Number;i++)
    {
       stream >> Z[i].R;
    }
    /*initchengzu(0);*/
    file.close();
    MFD.iNode = 1;
    //when open this system, we are at /root/
    PWD.push_back("root");
    //cout<<PWD[0]<<endl;
}

void file_system::writein(){
    QFile file("./data.txt");
    QTextStream stream(&file);
    //superblock
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    stream << super_block.id << " " << super_block.iNodeTotalNum << " " <<
              super_block.iNodeFreeNum << " " << super_block.dataBlockTotalNum << " " <<
              super_block.dataBlockFreeNum <<" " << endl;
    for (int i = 0; i < INode_Free_Stack_Num; i++) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        stream << super_block.iNodeFreeStack[i] << " ";
    }
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        stream << endl;
    for (int i = 0; i < Data_Block_Free_Stack_Num; i++) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
            stream << super_block.dataBlockFreeStack[i] << " ";
    }
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    stream << endl;
    //inode
    for (int i = 0; i < INode_Num; i++)
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        stream << inode[i].id << " " << inode[i].fileCount << " " << inode[i].size << " " <<
                  inode[i].fileMode << " " << inode[i].userId << " ";
        for(int j=0;j<Directory_Item_Num;j++) {
            if(file.open(QIODevice::WriteOnly | QIODevice::Append))
                stream << inode[i].userRight[j]<<" ";
            }
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
            stream<<inode[i].time.tdate<<" "<<inode[i].time.ttime<<" ";
        for (int j = 0; j < INode_Max_Num; j++)
        {
            if(file.open(QIODevice::WriteOnly | QIODevice::Append))
                stream << inode[i].diskAddress[j] << " ";
        }
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
            stream << endl;
    }

    //mfd, sfd
    for (int i = 0; i < MFD.size(); i++)//mfd
    {
        Main_File_Directory_Item item = MFD.get(i);
        //cout<< sfdm.id << " " << sfdm.name << " " << sfdm.psw << " ";
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        stream  << item.name << " "<<item.psw<<" " << item.iNode << " ";
    }
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    stream << endl;
    for (int j = 0; j < Directory_Num;j++) {//sfd
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        stream <<sfdTable[j].iNode<<" ";
        for (int i = 0; i < SFD.size(); i++)
        {
            Symbol_File_Directory_Item item = sfdTable[j].get(i);
            //cout<< sfdm.id << " " << sfdm.name << " ";
            if(file.open(QIODevice::WriteOnly | QIODevice::Append))
            stream << item.name << " "<< item.iNode << " " ;
            //cout << sfdm.name << "MM";
        }
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        stream << endl;
    }
    //ofstr4 << "block"<<endl;
    for(int i=0;i<Block_Number;i++) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
            stream<<Z[i].R<<endl;
    }
    file.flush();
    file.close();
}

int file_system::iNodeMalloc() {
    if (super_block.iNodeFreeNum <= 0) {
        return -1;
    }
    super_block.iNodeFreeNum--;
    //inode[INODENUM];
    int j;
    for (int i = 0; i < INode_Free_Stack_Num; i++)
    {
        if (!super_block.iNodeFreeStack[i])
        {
            super_block.iNodeFreeStack[i] = 1;
            j = i;
            break;
        }

    }
    return j;
}

void file_system::sudoFormat(){
    void run();
    if(user.login_user.name== "root" ) {
        remove("data.txt");
        format();
        load();
    } else{
        qDebug()<<"error: you don't have this right!"<<endl;
        return ;
    }
}

int file_system::getINodeOfCurrentPath() {
    int iNode=1;
    if(PWD.size() > 1) {

        bool flag = 0;
        for(int i=0;i<MFD.size();i++) {
            if(MFD.item[i].name == PWD[1]) {
                iNode = MFD.item[i].iNode;
                flag = 1;
                break;
            }
        }
        if(flag == 0) {
            iNode = 2;
            QVector<QString>::iterator it;
            for(it = PWD.begin()+1;it!=PWD.end();it++) {
                bool flag = 0;
                for(int j=0;j<Directory_Item_Num;j++) {
                    if(sfdTable[inode[iNode].diskAddress[0]].item[j].name == *it ) {
                        iNode = sfdTable[inode[iNode].diskAddress[0]].item[j].iNode;
                        flag = 1;
                    }
                }
                if(flag == 0) {
                cout<<"error: current path is wrong"<<endl;
                exit(0);
                }
            }
        } else {

            QVector<QString>::iterator it;
            if(PWD.size() >= 2) {
                for(it = PWD.begin()+2;it!=PWD.end();it++) {
                    bool flag = 0;
                    for(int j=0;j<Directory_Item_Num;j++) {
                        if(sfdTable[inode[iNode].diskAddress[0]].item[j].name == *it ) {
                            iNode = sfdTable[inode[iNode].diskAddress[0]].item[j].iNode;
                            flag = 1;
                        }
                    }
                    if(flag == 0) {
                        cout<<"error: current path is wrong"<<endl;
                        exit(0);
                    }
                }
            }
        }
    } else {
        iNode = 2;
    }

    return iNode;
}

void file_system::cd(QString dir){
    if(dir == "..") {
        cd_back();
    } else {
        int iNode = getINodeOfCurrentPath();
        bool flag = 0;
        for(int i=0;i<Directory_Item_Num;i++) {
            if(sfdTable[inode[iNode].diskAddress[0]].item[i].name == dir) {
                flag = 1;
                PWD.push_back(dir);
                break;
            }
        }

        if(flag == 0) {
            cout <<"error: not exist this dir"<<endl;
        }
    }
}

void file_system::cd_back(){
    if(PWD.size()>1) {
        PWD.erase(PWD.end());
    }
}
void file_system::pwd(){

    for(int i=0;i<PWD.size();++i){
        qDebug()<<PWD.at(i)<<"/";
    }
    qDebug()<<endl;
}
void file_system::ls() {

    //judge wheather user have this right
    int iNode = getINodeOfCurrentPath();
    for(int i=0;i<Directory_Item_Num;i++) {
        //cout<<sfdTable[iNode].item[i].name<<endl;
        //cout<<strlen(sfdTable[iNode].item[i].name)<<endl;
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].iNode != -1) {
            if(sfdTable[inode[iNode].diskAddress[0]].item[i].name!="/") {
                qDebug()<<sfdTable[inode[iNode].diskAddress[0]].item[i].name<<" ";
            }
        }
    }
    qDebug()<<endl;
}
void file_system::ll() {
    //需要添加时间

    int iNode = getINodeOfCurrentPath();
    cout<<"total "<<inode[iNode].size<<"B"<<endl;
    for(int i=0;i<Directory_Item_Num;i++) {
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].iNode != -1) {
            if(sfdTable[inode[iNode].diskAddress[0]].item[i].name!= "/") {

                int tmp = sfdTable[inode[iNode].diskAddress[0]].item[i].iNode;
                switch (inode[tmp].userRight[user.login_user.id]){
                    case 0:
                        cout<<"---"<<" ";
                        break;
                    case 1:
                        cout<<"r--"<<" ";
                        break;
                    case 2:
                        cout<<"-w-"<<" ";
                        break;
                    case 3:
                        cout<<"rw-"<<" ";
                        break;
                    case 4:
                        cout<<"--x"<<" ";
                        break;
                    case 5:
                        cout<<"r-x"<<" ";
                        break;
                    case 6:
                        cout<<"-wx"<<" ";
                        break;
                    case 7:
                        cout<<"rwx"<<" ";
                        break;
                }
                qDebug()<<MFD.item[inode[tmp].userId].name<<" ";
                qDebug()<<inode[tmp].size<<"B"<<" ";
                qDebug()<<inode[tmp].time.tdate<<" "<<inode[tmp].time.ttime<<" ";
                qDebug()<<sfdTable[inode[iNode].diskAddress[0]].item[i].name<<" ";
            }
            cout<<endl;
        }
    }
    cout<<endl;
}

int file_system::sfdMalloc(int getINode) {

    int tmp = -1;
    bool flag = 0;
    for(int i=0;i<Directory_Num;i++) {
        if(sfdTable[i].iNode == -1) {
            sfdTable[i].iNode = getINode;
            tmp = i;
            flag = 1;
            break;
        }
    }

    if(flag != 1) {
        return -1;
    }
    return tmp;
}

void file_system::mkdir() {
    QString dir="123";
    int getINode = iNodeMalloc();
    if(getINode == -1) {
        printf("error: can't mkdir more directories!\n");
        return ;
    }
    // ensure the filename is unique
    int iNode = getINodeOfCurrentPath();
    for(int i=0;i<Directory_Item_Num;i++) {
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].name == dir) {
            cout<<"error: this dir or file has existed!"<<endl;
            return;
        }
    }

    inode[getINode].id = getINode;
    inode[getINode].fileCount = 0;
    inode[getINode].size = 0;
    inode[getINode].fileMode = 0;
    inode[getINode].userId = user.login_user.id;
    for(int i=0;i<Directory_Item_Num;i++) {
        if(i == user.login_user.id) {
            inode[getINode].userRight[i] = 7;
        } else {
            inode[getINode].userRight[i] = 0;
        }
    }
    time_t t = time(0);
    strftime(inode[getINode].time.tdate,sizeof(inode[getINode].time.tdate),"%Y/%m/%d",localtime(&t));
    strftime(inode[getINode].time.ttime,sizeof(inode[getINode].time.ttime),"%X",localtime(&t));
    int tmpsfd = sfdMalloc(getINode);
    if( tmpsfd == -1) {
        cout<<"error: cant't mkdir more dir!"<<endl;
        return;
    } else {
        inode[getINode].diskAddress[0] = tmpsfd;
    }

    inode[iNode].fileCount +=1;
    inode[iNode].size+=inode[getINode].size;
    QString namex;
    namex=dir;
    if (sfdTable[inode[iNode].diskAddress[0]].add(namex, getINode) == -1)
    {
        cout<<"error: can't make more directories!";
        return ;
    }
    writein();
}

void file_system::rmf(QString dir) {

    int iNode1 = getINodeOfCurrentPath();
    int iNode;
    for(int i=0;i<Directory_Item_Num;i++) {
        if(sfdTable[inode[iNode1].diskAddress[0]].item[i].name==dir) {
            iNode = sfdTable[inode[iNode1].diskAddress[0]].item[i].iNode;
            sfdTable[inode[iNode1].diskAddress[0]].item[i].iNode = -1;
            break;
        }
    }

    queue<int> Qtmp;
    while(!Qtmp.empty()) {Qtmp.pop();}
    Qtmp.push(iNode);
    //cout<<Qtmp.size()<<endl;
    while(!Qtmp.empty()) {
        int tmp = Qtmp.front();Qtmp.pop();
        super_block.iNodeFreeStack[tmp] = 0;
        for(int i=0;i<Directory_Item_Num;i++) {
            if(sfdTable[inode[iNode].diskAddress[0]].item[i].iNode!=-1) {
                Qtmp.push(sfdTable[inode[iNode].diskAddress[0]].item[i].iNode);
                sfdTable[inode[iNode].diskAddress[0]].item[i].iNode = -1;
                //cout<<sfdTable[inode[iNode].diskAddress[0]].item[i].iNode<<" "<<Qtmp.size()<<endl;
            }
        }
    }
    writein();
}

void file_system::rename(QString dir1,QString dir2) {
    //cout<<dir1<<dir2<<endl;
    int iNode = getINodeOfCurrentPath();
    for(int i=0;i<Directory_Item_Num;i++) {
        //cout<<sfdTable[iNode].item[i].name<<endl;
        //cout<<strlen(sfdTable[iNode].item[i].name)<<endl;
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].name==dir1) {
            sfdTable[inode[iNode].diskAddress[0]].item[i].name==dir2;
            break;
        }
    }
    writein();
}

void file_system::touch(){
    QString fname="123";
    while(1)
    {
        //cout << "Please input the size of the file in Byte: ";
        //cin >> fsize;
        //if (fsize <0 * BLOCKSIZE || fsize >10 * BLOCKSIZE) {
            //cout << "Error! File size should be 0~10240 !" << endl;
        //}
        if (createFile(fname)) {
            qDebug() << "Create " << fname << " sucessfully!" << endl;
            break;
        }
        else {
            qDebug() << "Error! Failed to create file! " << endl;
        }
    }
}

int file_system::createFile(QString fname){
    //fsize = (fsize%BLOCKSIZE == 0) ? (fsize / BLOCKSIZE) : (fsize / BLOCKSIZE + 1);
    int getINode = iNodeMalloc();
    if (getINode == -1)
    {
        printf("error: can't touch more files!\n");
        return 0;
    }

    int iNode = getINodeOfCurrentPath();
    for(int i=0;i<Directory_Item_Num;i++) {
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].name == fname) {
            cout<<"error: this dir or file has existed!"<<endl;
            return 0;
        }
    }

    inode[getINode].id = getINode;
    inode[getINode].fileCount = 0;
    inode[getINode].size = 0;
    inode[getINode].fileMode = 1;
    inode[getINode].userId = user.login_user.id;
    for(int i=0;i<Directory_Item_Num;i++) {
        if(i == user.login_user.id) {
            inode[getINode].userRight[i] = 7;
        } else {
            inode[getINode].userRight[i] = 0;
        }
    }
    time_t t = time(0);
    strftime(inode[getINode].time.tdate,sizeof(inode[getINode].time.tdate),"%Y/%m/%d",localtime(&t));
    strftime(inode[getINode].time.ttime,sizeof(inode[getINode].time.ttime),"%X",localtime(&t));
    inode[iNode].fileCount +=1;
    inode[iNode].size+=inode[getINode].size;
    QString namex;
    namex = fname;
    if (sfdTable[inode[iNode].diskAddress[0]].add(namex, getINode) == -1)
    {
        cout<<"error: can't make more directories!";
        return 0;
    }
    writein();
    return 1;
}

void file_system::vim(QString file){

    int iNode = getINodeOfCurrentPath();
    bool flag = 0;
    for(int i=0;i<Directory_Item_Num;i++) {
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].name == file) {
            flag = 1;
            break;
        }
    }
    if(flag == 0) {
        cout <<"error: not exist this file!"<<endl;
        return ;
    }

    //cout<<file<<endl;
    QString content;
    //getline(cin,content);
    int diskAddress[100];
    memset(diskAddress,-1,sizeof(diskAddress));
    /*groupblock(content,diskAddress);*/

    int fileINode;
    for(int i=0;i<Directory_Item_Num;i++) {
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].name == file) {
            fileINode = sfdTable[inode[iNode].diskAddress[0]].item[i].iNode;
            break;
        }
    }

    int sum=0;
    for(int i=0;i<100;i++) {
        if(diskAddress[i]!=-1) {
            sum++;
        }
    }

    cout<<"sum:"<<sum<<endl;
    if(sum > INode_Max_Num) {
        cout<<"error: this file is too large!"<<endl;
        return ;
    } else {
        for(int i=0;i<sum;i++) {
            inode[fileINode].diskAddress[i] = diskAddress[i];
        }
        writein();
    }
}

void file_system::renamef(QString dir1,QString dir2) {
    //cout<<dir1<<dir2<<endl;
    int iNode = getINodeOfCurrentPath();
    for(int i=0;i<Directory_Item_Num;i++) {
        //cout<<sfdTable[iNode].item[i].name<<endl;
        //cout<<strlen(sfdTable[iNode].item[i].name)<<endl;
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].name== dir1) {
            sfdTable[inode[iNode].diskAddress[0]].item[i].name = dir2;
            break;
        }
    }
    writein();
}

void file_system::cat(QString file) {
    //cout<<file<<endl;
    int iNode = getINodeOfCurrentPath();

    int fileINode;
    for(int i=0;i<Directory_Item_Num;i++) {
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].name == file) {
            fileINode = sfdTable[inode[iNode].diskAddress[0]].item[i].iNode;
            break;
        }
    }

    for(int i=0;i<INode_Max_Num;i++) {
        if(inode[fileINode].diskAddress[i] != -1) {
            //cout<<inode[fileINode].diskAddress[i]<<endl;
            qDebug()<<Z[inode[fileINode].diskAddress[i]].R<<endl;
            //puts(Z[inode[fileINode].diskAddress[i]].R);
        }

    }
    cout<<endl;
}

