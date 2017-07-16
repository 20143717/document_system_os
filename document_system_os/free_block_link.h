#ifndef FREE_BLOCK_LINK_H
#define FREE_BLOCK_LINK_H

#include<math.h>
#include<stdio.h>
#include<stack>
#include<iostream>
#include<string>
#include <cstdlib>
#include "file_system.h"
#include "format.h"
#include "define.h"
#include "stacki.h"

using namespace std;
extern StackType *S;
const char*q;
int j=0;   //整数，用于记录二维数组A[j][k]

i_node data,*p_data;
/*
* initchengzu()
* init this stack
*/
void  initchengzu(int s_id){//开始空闲i节点号s_id
    int i;
    int k=0,m=0;
    int inodeshengyuNum=0;//剩余i节点的数量
    /*需要创建一个二维数组，如A[j].B[k],其中A[j]是存放组的基本单位，每个组有五十个i节点*/
    S=STInit(S);
    for(i=s_id;i<Block_Number;)
    {
        inodeshengyuNum=Block_Number-i;
        if(inodeshengyuNum<=49)
        {
             A[j].B[0].num++;
             A[j].B[k].ID=i;
             PushST(S,A[j].B[k]);//将剩余不足五十个i节点写进超级块ide栈S中
             k++;
             i++;
        }
        if(inodeshengyuNum>49)
        {
            for(m=0;m<50;m++)
            {
             A[j].B[k].ID=i;
             A[j].B[0].num++;
             k++;
             i++;
            }
             if((i-s_id) %50==0)//模怎么写了，不是mod吗？
             {
                  j=j+1;
                 A[j].B[0].num=0;
                 k=0;
              }
          }
        }
    }

/*
* fenpei()
* allocte block for an file
*/
int* fenpei(int Needing){//Needings是需要分配的块数
    int m=0;
    int *a = new int [100];
    for(int i=0;i<100;i++) {
        a[i]=-1;
    }

      while(1){
        if(Needing<=A[j].B[0].num)
        {
            for(int i=0;i<Needing;i++)////////////////////////////////////////////////////////////////
            {
                p_data=PopST(S);/////////这里是于其它程序的接口
                a[m]=p_data->ID;
                //cout<<A[j].B[0].num<<endl;
                m++;
                A[j].B[0].num--;
            }
            return a;
        }
        if(Needing>A[j].B[0].num)
        {
            while(STIsEmpty(S))//////////////////////////////////
            {
                p_data=PopST(S);
                a[m]=p_data->ID;
                m++;
            }
            j=j-1;
            for(int m=0;m<50;m++)
            {
                PushST(S,A[j].B[m]);//这个地方怎么写，我用指针*p1记录下了A[j]  PushST(S,A[j].B[k]);
            }//成组的A[j][0].S_num在初始化的时候赋值为50
        }
    }
}
/*
* huishou()
* revoke there block in a file
*/
void huishou(int Hui){//Hui是回收的块数
    while(1){
            int i=0;
        if(Hui+A[j].B[0].num<=50)
        {
            for(int i=A[j].B[0].num;i<Hui+A[j].B[0].num;i++)
            {
                A[j].B[i].ID=0;//这个地方应传进来一个节点号的数组，
                PushST(S,A[j].B[i]);
                A[j].B[0].num++;
            }
            return ;//break;
        }
        if(Hui+A[j].B[0].num>50)
        {
            while(1){
                PopST(S);
            }
           for(i=A[j].B[0].num;i<50;i++)
           {
               A[j].B[0].num++;
               A[j].B[i].ID=0;                                     //这应该传进来回收的块////////////////////////
           }
           Hui=Hui-A[j].B[0].num;
           j++;
           A[j].B[0].num=0;
        }
    }
}
void groupblock(QString writen, int diskAddress[]){
    int k=0,m=0;
    int *p, R_num,Needing;
    QString q=writen;
    //cout<<q[0]<<endl;
    R_num=writen.length();
    Needing=R_num/Block_Size+1;
    //initchengzu(0);
    p=fenpei(Needing);
    qDebug()<<"$$";
    for(int i=0;i<100;i++) {
        diskAddress[i] = p[i];
        //cout<<p[i]<<endl;
    }

    int ti=0;
    for(int i=0;i<R_num;i++)
    {
        if(ti<Block_Size)
        {
            Z[p[k]].R[m]=q[i];
            m++;
            ti++;
            //cout<< Z[p[k]].R[i];
        }
        if(ti == Block_Size)
        {
            ti = 0;
            k++;
            m=0;
        }
    }
    //cout<<endl;
    //cout<<writen.length()<<endl;
    //cout<<writen<<endl;
   // cout<<Needing<<endl;

}

#endif // FREE_BLOCK_LINK_H
