#ifndef STACKI_H
#define STACKI_H
#include <stdio.h>
#include "file_system.h"
#include <iostream>
#include <fstream>
#include "fstream"
#include <stdlib.h>
#include <vector>
#define MAXLEN 50
struct StackType
{
    i_node data[MAXLEN+1];
    int top;
};
/******************初始化栈结构****************/
StackType *STInit(StackType *p)
{
    //StackType *p;
    if(p=new StackType)			//申请栈空间
    {
        p->top=0;					//设置栈顶为0
        return p;					//返回栈顶指针
    }
    return NULL;
}
/****************判断空栈**********************/
int STIsEmpty(StackType *s)
{
    int t;
    t=s->top;					//通过栈顶的值进行判断
    return t;
}
/**********************判断满栈****************/
int STIsFull(StackType *s)
{
    int t;
    t=(s->top==MAXLEN);
    return t;
}
/**********************清空栈**********************/
void STClear(StackType *s)
{
    s->top=0;
}
/********************释放空间********************/
void STFree(StackType *s)
{
    delete s;
}
/**********************入栈***********************/
int PushST(StackType *s,i_node data)
{
    if((s->top)+1>MAXLEN)
    {
        cout<<"栈溢出"<<endl;
        return 0;
    }
    s->data[++s->top]=data;					//将元素压入栈
    return 1;
}
/************************出栈***********************/
i_node * PopST(StackType *s)
{
    if(s->top==0)
    {
        cout<<"栈为空，不能再输出！"<<endl;
        exit(0);
    }
    return &(s->data[s->top--]);
}
/**********************读取点结构*******************/
i_node *PeekST(StackType *s)
{
    if(s->top==0)
    {
        cout<<"栈已空"<<endl;
        exit(0);
    }
    return &(s->data[s->top]);
}

StackType *S;

#endif // STACKI_H
