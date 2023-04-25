//////////////////////////////////////////////////////////////////////////////
//expression.h - 计算表达式的值
//////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_
#include <stdbool.h>
#include <stdint.h>
#include "stack.h"
//////////////////////////////////////////////////////////////////////////////
//栈最大尺寸
#define STACK_MAX_SIZE						256
//表达式最大长度
#define EXPRESSION_MAX_LEN					384
//浮点数数值比较误差
#define DOUBLE_ERROR						1e-15
//生成运算符栈元素
#define MAKE_OP_STACK_ELE(ary,ch)			(((element_t)(ary)<<8)+(ch))
//从运算符栈元素取运算符元数
#define OPSTK_ARY(x)						((int8_t)((0xFF<<8&(x))>>8))
//从运算符栈元素取运算符
#define OPSTK_CHAR(x)						((char)((x)&0xFF))
//生成运算符信息
#define MAKE_EXP_INFO_ELE(ary,type)			((int)((type)+((ary)<<16)))
//从表达式信息取运算符元数
#define EXPINF_ARY(x)						((int8_t)(((x)&0xFF<<16)>>16))
//从表达式信息取后缀表达式元素类型
#define EXPINF_TYPE(x)						((int16_t)((x)&0xFFFF))
//////////////////////////////////////////////////////////////////////////////
//表达式信息
typedef enum
{
	ET_UNKNOWN,
	ET_NUMBER,
	ET_OPERATOR,
}ELE_TYPE;
//表达式
typedef struct
{
	//表达式数据
	double exp[EXPRESSION_MAX_LEN];
	//表达式信息
	int inf[EXPRESSION_MAX_LEN];
	//表达式长度
	size_t len;
}expr;
//////////////////////////////////////////////////////////////////////////////
//@brief 是否是运算符
//@param ch 被测试的字符
//@return 如果是则为true
bool is_operator(char ch);
//@brief 获取运算符优先级
//@param ary 元数
//@param op 运算符
//@return 优先级,从0开始,越大优先级越低.-1表示无效运算符.
int get_op_priority(int8_t ary, char op);
//@brief 调度场算法,将中缀表达式转换为后缀表达式
//@param exp_infix 待转换的前缀表达式.
//@param out_exp_postfix 转换后的后缀表达式
//@return 转换后的表达式的长度
size_t shunting_yard(const char* exp_infix, expr* out_exp_postfix);
//@brief 原子计算
//@param ary 运算符元数
//@param op 运算符
//@param ... double类型操作数x1,x2,x3,x4...
//@return 计算结果
double atomic_calc(int8_t ary, char op, ...);
//@brief 计算后缀表达式
//@param exp_postfix 后缀表达式
//@return 计算结果
double calc(const expr* exp_postfix);
//@brief element的数据直接内存拷贝到double
double ele2db(element_t ele);
//@brief double的数据直接内存拷贝到element
element_t db2ele(double db);
//////////////////////////////////////////////////////////////////////////////
#endif //_EXPRESSION_H_