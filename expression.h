//////////////////////////////////////////////////////////////////////////////
//expression.h - ������ʽ��ֵ
//////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_
#include <stdbool.h>
#include <stdint.h>
#include "stack.h"
//////////////////////////////////////////////////////////////////////////////
//ջ���ߴ�
#define STACK_MAX_SIZE						256
//���ʽ��󳤶�
#define EXPRESSION_MAX_LEN					384
//��������ֵ�Ƚ����
#define DOUBLE_ERROR						1e-15
//���������ջԪ��
#define MAKE_OP_STACK_ELE(ary,ch)			(((element_t)(ary)<<8)+(ch))
//�������ջԪ��ȡ�����Ԫ��
#define OPSTK_ARY(x)						((int8_t)((0xFF<<8&(x))>>8))
//�������ջԪ��ȡ�����
#define OPSTK_CHAR(x)						((char)((x)&0xFF))
//�����������Ϣ
#define MAKE_EXP_INFO_ELE(ary,type)			((int)((type)+((ary)<<16)))
//�ӱ��ʽ��Ϣȡ�����Ԫ��
#define EXPINF_ARY(x)						((int8_t)(((x)&0xFF<<16)>>16))
//�ӱ��ʽ��Ϣȡ��׺���ʽԪ������
#define EXPINF_TYPE(x)						((int16_t)((x)&0xFFFF))
//////////////////////////////////////////////////////////////////////////////
//���ʽ��Ϣ
typedef enum
{
	ET_UNKNOWN,
	ET_NUMBER,
	ET_OPERATOR,
}ELE_TYPE;
//���ʽ
typedef struct
{
	//���ʽ����
	double exp[EXPRESSION_MAX_LEN];
	//���ʽ��Ϣ
	int inf[EXPRESSION_MAX_LEN];
	//���ʽ����
	size_t len;
}expr;
//////////////////////////////////////////////////////////////////////////////
//@brief �Ƿ��������
//@param ch �����Ե��ַ�
//@return �������Ϊtrue
bool is_operator(char ch);
//@brief ��ȡ��������ȼ�
//@param ary Ԫ��
//@param op �����
//@return ���ȼ�,��0��ʼ,Խ�����ȼ�Խ��.-1��ʾ��Ч�����.
int get_op_priority(int8_t ary, char op);
//@brief ���ȳ��㷨,����׺���ʽת��Ϊ��׺���ʽ
//@param exp_infix ��ת����ǰ׺���ʽ.
//@param out_exp_postfix ת����ĺ�׺���ʽ
//@return ת����ı��ʽ�ĳ���
size_t shunting_yard(const char* exp_infix, expr* out_exp_postfix);
//@brief ԭ�Ӽ���
//@param ary �����Ԫ��
//@param op �����
//@param ... double���Ͳ�����x1,x2,x3,x4...
//@return ������
double atomic_calc(int8_t ary, char op, ...);
//@brief �����׺���ʽ
//@param exp_postfix ��׺���ʽ
//@return ������
double calc(const expr* exp_postfix);
//@brief element������ֱ���ڴ濽����double
double ele2db(element_t ele);
//@brief double������ֱ���ڴ濽����element
element_t db2ele(double db);
//////////////////////////////////////////////////////////////////////////////
#endif //_EXPRESSION_H_