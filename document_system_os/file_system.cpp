#include "file_system.h"
#include "ui_file_system.h"
#include "run.h"
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include<QFileDialog>

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
}

file_system::~file_system()
{
    delete ui;
}
void file_system::showEvent(QShowEvent *){
    QMessageBox::information(this,"提示","欢迎用户"+user.login_user.name+"登录!");
    run();
    ui->login_name->setText(user.login_user.name);
}

void file_system::format(){
    switch(QMessageBox::question(this,"Warning","ALL DATA ON THIS FILESYSTEM WILL BE LOST!\nProceed with Format(Y/N)?",
                          QMessageBox::Ok|QMessageBox::No,QMessageBox::Ok)){
        case QMessageBox::Ok:
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
            strcpy(MFD.item[0].name,"root");
            strcpy(MFD.item[0].psw, "123456");
            MFD.item[0].iNode = 1;
            for (int i=0; i < MFD.size(); i++)//mfd
            {
                Main_File_Directory_Item item = MFD.get(i);
                if(i!=0) strcpy(item.psw, "/");
                if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
                stream <<item.name<<" "<<item.psw<<" " << item.iNode << " ";
            }
            if(fll.open(QIODevice::WriteOnly | QIODevice::Append))
            stream << endl;

            sfdTable[0].iNode = 2;
            for(int i=0;i<Directory_Num;i++) {
                for(int j=0;j<SFD.size();j++) {
                    strcpy(sfdTable[i].item[j].name,"/");
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
    case QMessageBox::No:
        QFile sys;
        if (!sys.exists()) {
            qDebug()<<"error: disk hasn't formatted!"<<endl;
            exit(0);
        }
        PWD.clear();
        break;
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
    while (getline(cin,order))
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
                sudoFormat();
            } else if(order == "help"){
                help();
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
            cout<<loginUser.username<<"@";
            QVector<QString>::iterator it;
            for(it = PWD.begin();it!=PWD.end();it++) {
                cout<<"/"<<*it;
            }
            cout<<"$ ";
        }
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
        qDebug() << sfdm.id << " " << sfdm.name << " " << sfdm.psw << " ";
        if (i == 0) {
            MFD.item[0].iNode = 2;
            MFD.item[0].name=user.login_user.name;
            MFD.item[0].psw=user.login_user.password;
        }
        else
        {
            MFD.add(item.name, item.iNode);
            strcpy(MFD.item[i].psw, item.psw);
        }
    }

    for (int j = 0; j < Directory_Num; j++) {//sfd
        stream >> sfdTable[j].iNode;
        qDebug() << sfdtable[j].id;
        for (int i = 0; i < SFD.size(); i++)
        {
            Symbol_File_Directory_Item item;
            stream >> item.name >> item.iNode   ;
            qDebug() << item.iNode<<" "<<item.name;
            sfdTable[j].add(item.name, item.iNode);
            qDebug() << sfdtable[j].item[i].name << endl;
        }
    }

    //cout << sfdtable[0].id;
    //char tmp[20];
    //ifstr2>>tmp;
    for(int i=0;i<Block_Number;i++)
    {
       stream >> Z[i].R;
    }
    initchengzu(0);
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
