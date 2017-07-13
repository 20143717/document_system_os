#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include <QVector>
#include <QString>
#include <iostream>
#include "directory.h"
#include <QDebug>
#include "define.h"

using namespace std;

int createFile(QString fname);
int iNodeMalloc();

void touch(QString fname)
{
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
int createFile(QString fname)
{
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
    writenIn();
    return 1;
}

void vim(QString file) {

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
    getline(cin,content);
    int diskAddress[100];
    memset(diskAddress,-1,sizeof(diskAddress));
    groupblock(content,diskAddress);

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
        writenIn();
    }
}

void renamef(QString dir1,QString dir2) {
    //cout<<dir1<<dir2<<endl;
    int iNode = getINodeOfCurrentPath();
    for(int i=0;i<Directory_Item_Num;i++) {
        //cout<<sfdTable[iNode].item[i].name<<endl;
        //cout<<strlen(sfdTable[iNode].item[i].name)<<endl;
        if(strcmp(sfdTable[inode[iNode].diskAddress[0]].item[i].name, dir1.c_str()) == 0) {
            strcpy(sfdTable[inode[iNode].diskAddress[0]].item[i].name, dir2.c_str());
            break;
        }
    }
    writenIn();
}

void cat(QString file) {
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
            printf("%s",Z[inode[fileINode].diskAddress[i]].R);
            //puts(Z[inode[fileINode].diskAddress[i]].R);
        }

    }
    cout<<endl;
}


#endif // FILE_H
