#ifndef STRUCT_H
#define STRUCT_H
//定义一些结构体和基本数据结构

#include <iostream>
#include <QString>
#include <QVector>
#include "define.h"
#include <QFile>
#include "user.h"

using namespace std;

/*
*  block
*  size: 1024B
*  sumSize: 990KB
*/
struct block {
    int id;
    QString R;
};

/*
*  superBlock
*  size: 1024B
*/
struct Super_Block {
    int id;
    int iNodeTotalNum;
    int iNodeFreeNum;
    //0-32
    int dataBlockTotalNum;
    int dataBlockFreeNum;

    int iNodeFreeStack[INode_Free_Stack_Num];
    int dataBlockFreeStack[Data_Block_Free_Stack_Num];
    //int superBlockFlag;

};
/*
*  iNode
*  size: 32B
*  sumSize: 32KB
*/
struct tTime {
    char tdate[64];
    char ttime[64];
};
struct iNode {//i节点
    int id;//节点号
    int fileCount;//引用计数,表示有几个目录引用这个文件
    int size;
    //文件的大小,如果这是一个文件 这里表示文件所占的物理块个数，
    //如果是一个目录，表示该目录下已经有的文件个数,用于判断是不是超过10个了
    int fileMode;//文件类型,区分该i节点是文件(2)还是目录(1)还是用户, 1 means file, 0 means dir
    int userId;   //使用者ID
    int userRight[Directory_Item_Num];//使用者权限
    tTime time;
    int diskAddress[INode_Max_Num];
    //如果这个i结点是一个文件，Addr数组表示文件各个物理块位置编号
    //如果是一个目录，表示目录的各个文件or文件夹的SFD的下标,所以一个目录下最大有NADDR个文件/目录
    //如果是NADDR，那么Inode[0]存放的是用户，addr存放的是NADDR个用户的主文件夹的i结点的下标???????
};

/*
*  directoryItem
*  size: 16B
*/
struct Directory_Item {
    char name[Directory_Item_Length];
    int inode;
};
struct Dictory {
    struct Directory_Item item[Block_Size / Directory_Item_Size];
};

struct Main_File_Directory_Item {
    char name[Directory_Item_Length];	//文件名字
    int iNode;			//i结点的编号,-1表示没有
    char psw[20];
    Main_File_Directory_Item() {}
    Main_File_Directory_Item(char *n, int i) {
        strcpy(name, n);
        iNode = i;
        strcpy(name, "/");
    }
};
struct Main_File_Directory {
    int iNode;
    Main_File_Directory_Item item[Directory_Item_Num];
    int sz;

    Main_File_Directory() {
        iNode = -1;
        sz = Directory_Item_Num;
        for (int i = 0; i<Directory_Num; i++) {
            item[i].iNode = -1;
            strcpy(item[i].name,"/" );
            strcpy(item[i].psw,"/" );
        }
    }
    int size() {
        return sz;
    }
    void set(int id, Main_File_Directory_Item t) {
        item[id] = t;
    }
    void init() {
        iNode = -1;
        sz = Directory_Item_Num;
        for (int i = 0; i<Directory_Item_Num; i++) {
            item[i].iNode = -1;
            strcpy(item[i].name,"/" );
            strcpy(item[i].psw,"/" );
        }
    }
    Main_File_Directory_Item get(int i) {
        return item[i];
    }
    int add(char *name, int id) { //name为name的i结点id
        for (int i = 0; i<Directory_Item_Num; i++) {
            if (item[i].iNode == -1) {
                item[i].iNode = id;
                strcpy(item[i].name, name);
                return i; // 返回的是SFD的位置
            }
        }
        return -1;
    }
    void drop(int i) {
        item[i].iNode = -1;
    }
};

struct Symbol_File_Directory_Item {
    char name[Directory_Item_Length];	//文件名字
    int iNode;			//i结点的编号,-1表示没有
    Symbol_File_Directory_Item() {}
    Symbol_File_Directory_Item(char *n, int i) {
        strcpy(name, n);
        iNode = i;
        strcpy(name, "/");
    }
};
struct Symbol_File_Directory {
    int iNode;
    Symbol_File_Directory_Item item[Directory_Item_Num];
    int sz;
    int right;

    Symbol_File_Directory() {
        iNode = -1;
        sz = Directory_Item_Num;
        for (int i = 0; i<Directory_Item_Num; i++) {
            item[i].iNode = -1;
            strcpy(item[i].name,"/" );
        }
    }
    int size() {
        return sz;
    }
    void set(int id, Symbol_File_Directory_Item t) {
        item[id] = t;
    }
    void init() {
        iNode = -1;
        sz = Directory_Item_Num;
        for (int i = 0; i<Directory_Item_Num; i++) {
            item[i].iNode = -1;
            strcpy(item[i].name,"/" );
        }
    }
    Symbol_File_Directory_Item get(int i) {
        return item[i];
    }
    int add(char *name, int id) {//name为name的i结点id
        for (int i = 0; i<Directory_Item_Num; i++) {
            if (item[i].iNode== -1) {
                item[i].iNode = id;
                strcpy(item[i].name, name);
                //cout << "分配的SFD ID是" << i << endl;
                return i; // 返回的是SFD的位置
            }
        }
        return -1;
    }
    void drop(int i) {
        item[i].iNode = -1;
    }
};

struct i_node{
    int ID;
    int num;
}B[50];
struct zu{
    i_node B[50];
}A[20];

    const char SYSTEM[10] = "data.txt";	// 文件系统名称
    int username;
    QFile fp;
    Super_Block super_block;
    Main_File_Directory MFD;
    Symbol_File_Directory SFD;
    iNode inode[INode_Num];
    Symbol_File_Directory sfdTable[Directory_Num];
    block Z[Block_Number-Start_Block_Num];//ont  group
    QVector<QString>PWD;

#endif // STRUCT_H
