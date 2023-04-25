//////////////////////////////////////////////////////////////////////////////
//stack.h - 栈
//////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _STACK_H_
#define _STACK_H_
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////////////
typedef long long element_t;
//栈
typedef struct
{
	//栈内存空间
	element_t* elements;
	//指向栈顶的指针
	element_t* ptr;
}stack;
//@brief 新建栈
//@param max_size 最大尺寸
//@return 若创建成功,返回栈指针.否则返回NULL.
stack* stack_new(size_t max_size);
//@brief 删除栈,指针将被自动置零.对空指针将不作操作..
//@param stack_ 栈
void stack_del(stack** stack_);
//@brief 压栈
//@param stack_ 栈
//@param element 元素
//@return 被压入的元素在栈中的指针.压入失败返回NULL.
element_t* stack_push(stack* stack_, element_t element);
//@brief 出栈
//@param stack_ 栈
//@return 被弹出的元素在栈中的指针.栈空或弹出失败返回NULL.
element_t* stack_pop(stack* stack_);
//@brief 取栈顶元素指针
//@param stack_ 栈
//@return 栈顶元素指针.栈空或获取失败返回NULL.
element_t* stack_top(stack* stack_);
//@brief 获取栈中元素个数
//@param stack_ 栈
//@return 栈中元素个数.
size_t stack_num(stack* stack_);
//////////////////////////////////////////////////////////////////////////////
#endif //_STACK_H_