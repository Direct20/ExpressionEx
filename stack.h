//////////////////////////////////////////////////////////////////////////////
//stack.h - ջ
//////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _STACK_H_
#define _STACK_H_
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////////////
typedef long long element_t;
//ջ
typedef struct
{
	//ջ�ڴ�ռ�
	element_t* elements;
	//ָ��ջ����ָ��
	element_t* ptr;
}stack;
//@brief �½�ջ
//@param max_size ���ߴ�
//@return �������ɹ�,����ջָ��.���򷵻�NULL.
stack* stack_new(size_t max_size);
//@brief ɾ��ջ,ָ�뽫���Զ�����.�Կ�ָ�뽫��������..
//@param stack_ ջ
void stack_del(stack** stack_);
//@brief ѹջ
//@param stack_ ջ
//@param element Ԫ��
//@return ��ѹ���Ԫ����ջ�е�ָ��.ѹ��ʧ�ܷ���NULL.
element_t* stack_push(stack* stack_, element_t element);
//@brief ��ջ
//@param stack_ ջ
//@return ��������Ԫ����ջ�е�ָ��.ջ�ջ򵯳�ʧ�ܷ���NULL.
element_t* stack_pop(stack* stack_);
//@brief ȡջ��Ԫ��ָ��
//@param stack_ ջ
//@return ջ��Ԫ��ָ��.ջ�ջ��ȡʧ�ܷ���NULL.
element_t* stack_top(stack* stack_);
//@brief ��ȡջ��Ԫ�ظ���
//@param stack_ ջ
//@return ջ��Ԫ�ظ���.
size_t stack_num(stack* stack_);
//////////////////////////////////////////////////////////////////////////////
#endif //_STACK_H_