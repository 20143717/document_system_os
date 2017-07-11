#ifndef FORMAT_H
#define FORMAT_H
#pragma once
#include <stdio.h>
#include <QFile>
#include "file_system.h"
#include "define.h"
#include <iostream>
#include <fstream>
#include "fstream"
#include <stdlib.h>
#include <vector>
#include "free_block_link.h"
#include <time.h>
#include "format.h"
#include "struct.h"
#include "user.h"

using namespace std;
extern Super_Block super_block;
extern Main_File_Directory MFD;
extern Symbol_File_Directory SFD;
extern iNode inode[INode_Num];
extern Symbol_File_Directory sfdTable[Directory_Num];
extern block Z[Block_Number-Start_Block_Num];//ont  group
extern QVector<QString>PWD;
extern USER user;
/*
* format()
* format the disk
* (1)init block
* (2)init superBlock; include init
* (3)init iNode
*/
/*
*  load()
*  if the disk has been formated, we need to load
*/
/*
* writeIn()
* if anything in this file system changes, writeIn is needed
*/

/*
* iNodeMalloc()
* apply for an iNode
*/
int iNodeMalloc() {
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

/*
* sudoFormat()
* cmd: format
*/
void sudoFormat() {
    void run();
    if(strcmp(user.login_user.name, "root") == 0) {
        remove("data.txt");
        format();
        load();
    } else {
        cout<<"error: you don't have this right!"<<endl;
        return ;
    }
}

#endif // FORMAT_H
