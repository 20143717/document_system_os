#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <queue>
#include <time.h>
#include "file_system.h"
#include "struct.h"
#include "user.h"
#include "format.h"

using namespace std;

/*
* getINodeOfCurrentPath()
* get current iNode
*/
extern Super_Block super_block;
extern Main_File_Directory MFD;
extern Symbol_File_Directory SFD;
extern iNode inode[INode_Num];
extern Symbol_File_Directory sfdTable[Directory_Num];
extern block Z[Block_Number-Start_Block_Num];//ont  group
extern QVector<QString>PWD;
extern USER user;

int getINodeOfCurrentPath() {
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

void cd (QString dir) {
    void cd_back();
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
void cd_back() {
    if(PWD.size()>1) {
        PWD.erase(PWD.end());
    }
}
void pwd() {

    QVector<QString>::iterator it;
    cout<<"/";
    for(it = PWD.begin();it!=PWD.end();it++) {
        cout<< PWD.at(it) <<"/";
    }
    cout<<endl;
}
void ls() {

    //judge wheather user have this right
    int iNode = getINodeOfCurrentPath();
    for(int i=0;i<Directory_Item_Num;i++) {
        //cout<<sfdTable[iNode].item[i].name<<endl;
        //cout<<strlen(sfdTable[iNode].item[i].name)<<endl;
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].iNode != -1) {
            if(strcmp(sfdTable[inode[iNode].diskAddress[0]].item[i].name,"/") != 0) {
                cout<<sfdTable[inode[iNode].diskAddress[0]].item[i].name<<" ";
            }
        }
    }
    cout<<endl;
}
void ll() {
    //需要添加时间

    int iNode = getINodeOfCurrentPath();
    cout<<"total "<<inode[iNode].size<<"B"<<endl;
    for(int i=0;i<Directory_Item_Num;i++) {
        if(sfdTable[inode[iNode].diskAddress[0]].item[i].iNode != -1) {
            if(strcmp(sfdTable[inode[iNode].diskAddress[0]].item[i].name,"/") != 0) {

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
                cout<<MFD.item[inode[tmp].userId].name<<" ";
                cout<<inode[tmp].size<<"B"<<" ";
                cout<<inode[tmp].time.tdate<<" "<<inode[tmp].time.ttime<<" ";
                cout<<sfdTable[inode[iNode].diskAddress[0]].item[i].name<<" ";
            }
            cout<<endl;
        }
    }
    cout<<endl;
}

int sfdMalloc(int getINode) {

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

void mkdir(string dir) {

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
    char namex[Name_Size];
    strcpy(namex, dir.c_str());
    if (sfdTable[inode[iNode].diskAddress[0]].add(namex, getINode) == -1)
    {
        cout<<"error: can't make more directories!";
        return ;
    }
    writenIn();
}
void rmf(string dir) {

    int iNode1 = getINodeOfCurrentPath();
    int iNode;
    for(int i=0;i<Directory_Item_Num;i++) {
        if(strcmp(sfdTable[inode[iNode1].diskAddress[0]].item[i].name,dir.c_str()) == 0) {
            iNode = sfdTable[inode[iNode1].diskAddress[0]].item[i].iNode;
            sfdTable[inode[iNode1].diskAddress[0]].item[i].iNode = -1;
            break;
        }
    }

    queue<int>Qtmp;
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
    writenIn();
}

void rename(string dir1,string dir2) {
    //cout<<dir1<<dir2<<endl;
    int iNode = getINodeOfCurrentPath();
    for(int i=0;i<Directory_Item_Num;i++) {
        //cout<<sfdTable[iNode].item[i].name<<endl;
        //cout<<strlen(sfdTable[iNode].item[i].name)<<endl;
        if(strcmp(sfdTable[inode[iNode].diskAddress[0]].item[i].name,dir1.c_str()) == 0) {
            strcpy(sfdTable[inode[iNode].diskAddress[0]].item[i].name,dir2.c_str());
            break;
        }
    }
    writenIn();
}


#endif // DIRECTORY_H
