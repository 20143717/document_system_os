#ifndef DEFINE_H
#define DEFINE_H

#define Block_Size 32			//块大小
#define Block_Number 199     //块个数
#define INode_Free_Stack_Num 32    //i节点空闲栈
#define Data_Block_Free_Stack_Num 10	//数据空闲栈
//#define DISK_ADDRESS_NUM 20        //每个i节点最多指向10块，addr[0]~addr[9]
#define INode_Max_Num 20
#define Directory_Item_Length 20      //the length of filename or username
#define Directory_Item_Size 16		//大小
#define Name_Size 8   //名字长度
#define Directory_Item_Num 10 // SFD大小
#define Directory_Num 1024
#define INode_Num 32  //
#define Start_Block_Num 4////
#define TRUE 1
#define FALSE 0
//#define null ((void *)0)
#define CMD_Size 20

#endif // DEFINE_H
