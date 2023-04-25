//////////////////////////////////////////////////////////////////////////////
//expression.c - 计算表达式的值
//////////////////////////////////////////////////////////////////////////////
#include "expression.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <locale.h>
#include <math.h>
#include "excp.h"
#pragma warning(disable:4996)
//////////////////////////////////////////////////////////////////////////////
DEFINE_TRY_LABEL(MAIN);
//////////////////////////////////////////////////////////////////////////////
//运算符定义
const int operators[][3] = {
	//运算符元数,运算符,运算符优先级
	{1,'!',0},//一元阶乘
	{1,'+',1},//一元加
	{1,'-',1},//一元减
	{2,'^',2},//二元乘方
	{2,'*',3},//二元乘
	{2,'/',3},//二元除
	{2,'%',3},//二元取余
	{2,'+',4},//二元加
	{2,'-',4},//二元减
	{0,'(',10}//左括号
};
//异常信息
const exc exceptions[] =
{
	{1000,"Failed to create the stack."},
	{1001,"The bracket does not match."},
	{1002,"Invalid expression."},
	{1003,"Unexpected operator in converted postfix expression."},
	{1004,"Invalid converted postfix expression info."},
	{1005,"Invalid converted postfix expression."},
	{1006,"The expression is too long.(Longer than 384 characters)"},
	{1007,"Failed to allocate memory."},
	{1008,"Cannot divide by 0."},
	{1009,"Factorial operator cannot operate a negative number."},
	{1010,"Factorial operator cannot operate a fraction."}
};
//////////////////////////////////////////////////////////////////////////////
int main()
{
	setlocale(LC_ALL, "");
	TRY(MAIN)
	{
		expr* exp_postfix;
		exp_postfix = malloc(sizeof(expr));
		if (!exp_postfix)THROW(MAIN, exceptions[7]);
	BEGIN:
		while (true)
		{
			char exp_infix[EXPRESSION_MAX_LEN * 3];
			double result;

			puts("Enter an expression:");
			scanf("%[^\n]", exp_infix);
			getchar();

			shunting_yard(exp_infix, exp_postfix);


			//for (int i = 0; i < exp_postfix->len; i++)
			//{
			//	switch (EXPINF_TYPE(exp_postfix->inf[i]))
			//	{
			//	case ET_NUMBER:
			//		printf("%lf", exp_postfix->exp[i]);
			//		break;
			//	case ET_OPERATOR:
			//		putchar((char)db2ele(exp_postfix->exp[i]));
			//		break;
			//	}
			//	putchar(' ');
			//}
			//putchar('\n');



			result = calc(exp_postfix);

			printf("Value= %lf\n", result);

			putchar('\n');
		}
		free(exp_postfix);
	}
	CATCH()
	{
		const exc* e = CATCH_var(MAIN);
		fprintf(stderr, "Error %d: %s\n\n", e->c, e->str);
		goto BEGIN;
	}
	return 0;
}
bool is_operator(char ch)
{
	int i;
	for (i = 0; i < sizeof(operators) / 3 / sizeof(int); i++)
		if (operators[i][1] == ch)
			return true;
	return false;
}
int get_op_priority(int8_t ary, char op)
{
	int i;
	for (i = 0; i < sizeof(operators) / 3 / sizeof(int); i++)
		if (operators[i][0] == ary && operators[i][1] == op)
			return operators[i][2];
	return -1;
}
size_t shunting_yard(const char* exp_infix, expr* out_exp_postfix)
{
	stack* stk_op;//运算符栈
	const char* prev = NULL;//前一个表达式字符指针(不含空格)
	size_t len;//中缀表达式长度
	double* out_exp = out_exp_postfix->exp;;//输出表达式数据
	int* out_info = out_exp_postfix->inf;//输出表达式信息

	//中缀表达式长度检查:
	len = strlen(exp_infix);
	if (len > EXPRESSION_MAX_LEN)THROW(MAIN, exceptions[6]);//表达式过长
	//新建栈:
	stk_op = stack_new(STACK_MAX_SIZE);//新建栈
	if (!stk_op)THROW(MAIN, exceptions[0]);//栈创建失败
	//解析表达式:
	for (; *exp_infix != '\0'; exp_infix++)
	{
		//跳过空白字符:
		if (isspace(*exp_infix))continue;
		//解析数字:
		if (isdigit(*exp_infix) || *exp_infix == '.')
		{
			double num;
			char* ptr = NULL;

			num = strtod(exp_infix, &ptr);
			*out_exp++ = num;//直接将数字放入输出表达式数据数组
			*out_info++ = ET_NUMBER;//数值

			exp_infix = ptr - 1;//跟进到非数字字符前一个字符
		}
		//解析运算符:
		else if (*exp_infix != '(' && is_operator(*exp_infix))//是运算符
		{
			element_t* top = NULL;//栈顶
			int pr;//当前运算符优先级
			int8_t ary;//当前运算符元数

			ary = strchr("+-", *exp_infix) &&
				(prev == NULL || (*prev != '.' && !isdigit(*prev) && *prev != '!')) ? 1 : 2;//运算符元数
			ary = strchr("!", *exp_infix) ? 1 : ary;//其他一元运算符
			pr = get_op_priority(ary, *exp_infix);//当前运算符的优先级

			while ((top = stack_top(stk_op)) &&
				(ary == 1 ? pr > get_op_priority(OPSTK_ARY(*top), OPSTK_CHAR(*top)) :
					pr >= get_op_priority(OPSTK_ARY(*top), OPSTK_CHAR(*top))))
				//一元运算符和二元运算符区别对待,重复弹栈,直到小于或栈空
			{
				element_t* popped = stack_pop(stk_op);
				*out_exp++ = ele2db(OPSTK_CHAR(*popped));
				*out_info++ = MAKE_EXP_INFO_ELE(OPSTK_ARY(*popped), ET_OPERATOR);//运算符
			}
			stack_push(stk_op, MAKE_OP_STACK_ELE(ary, *exp_infix));//入栈
		}
		else if (*exp_infix == '(')
		{
			stack_push(stk_op, MAKE_OP_STACK_ELE(0, *exp_infix));//入栈
		}
		else if (*exp_infix == ')')
		{
			element_t* popped = NULL;
			//循环弹栈直到弹出左括号:
			while ((popped = stack_pop(stk_op)) && OPSTK_CHAR(*popped) != '(')
			{
				*out_exp++ = ele2db(OPSTK_CHAR(*popped));
				*out_info++ = MAKE_EXP_INFO_ELE(OPSTK_ARY(*popped), ET_OPERATOR);//运算符
			}
			if (!popped)
			{
				stack_del(&stk_op);
				THROW(MAIN, exceptions[1]);//括号不匹配
			}
		}
		else
		{
			stack_del(&stk_op);
			THROW(MAIN, exceptions[2]);//出现意外的字符
		}
		prev = exp_infix;
	}

	//弹出栈中所有剩余的运算符:
	element_t* popped = NULL;
	while (popped = stack_pop(stk_op))
	{
		if (OPSTK_CHAR(*popped) == '(')
		{
			stack_del(&stk_op);
			THROW(MAIN, exceptions[1]);//括号不匹配
		}

		*out_exp++ = ele2db(OPSTK_CHAR(*popped));
		*out_info++ = MAKE_EXP_INFO_ELE(OPSTK_ARY(*popped), ET_OPERATOR);//运算符
	}

	stack_del(&stk_op);

	out_exp_postfix->len = out_exp - out_exp_postfix->exp;
	return out_exp - out_exp_postfix->exp;
}
double atomic_calc(int8_t ary, char op, ...)
{
	va_list valist;

	va_start(valist, op);

	double a[10];//操作数
	switch (ary)
	{
	case 1:
		a[0] = va_arg(valist, double);
		switch (op)
		{
		case '+':
			return a[0];
		case '-':
			return -a[0];
		case '!':
		{
			long long i = (long long)a[0];
			double ret = 1.;

			if (i < 0)THROW(MAIN, exceptions[9]);//不能操作负数
			if (a[0] - i > DOUBLE_ERROR)THROW(MAIN, exceptions[10]);//不能操作小数
			for (; i >= 1; ret *= i--);
			return ret;
		}
		default:
			THROW(MAIN, exceptions[3]);//无效运算符
			break;
		}
	case 2:
		a[0] = va_arg(valist, double);
		a[1] = va_arg(valist, double);
		switch (op)
		{
		case '+':
			return a[0] + a[1];
		case '-':
			return a[0] - a[1];
		case '*':
			return a[0] * a[1];
		case '/':
			//输入的原始数值,未参与运算,可以直接比较:
			if (a[1] == 0.)THROW(MAIN, exceptions[8]);//除数不能为0
			return a[0] / a[1];
		case '%':
			//输入的原始数值,未参与运算,可以直接比较:
			if (a[1] == 0.)THROW(MAIN, exceptions[8]);//除数不能为0
			return fmod(a[0], a[1]);
		case '^':
			return pow(a[0], a[1]);
		default:
			THROW(MAIN, exceptions[3]);//无效运算符
			break;
		}
	default:
		THROW(MAIN, exceptions[3]);//无效运算符
		break;
	}
	va_end(valist);
	return 0.;
}
double calc(const expr* exp_postfix)
{
	stack* stk_num;//操作数栈
	const double* const exp_end_c = exp_postfix->exp + exp_postfix->len;//表达式终止位置
	const double* exp = exp_postfix->exp;//表达式数据
	const int* exp_info = exp_postfix->inf;//表达式信息
	//新建栈:
	stk_num = stack_new(STACK_MAX_SIZE);
	if (!stk_num)THROW(MAIN, exceptions[0]);
	//计算:
	for (; exp != exp_end_c; exp++, exp_info++)
	{
		switch (EXPINF_TYPE(*exp_info))
		{
		case ET_NUMBER:
			stack_push(stk_num, db2ele(*exp));
			break;
		case ET_OPERATOR:
		{
			int8_t ary = EXPINF_ARY(*exp_info);//取运算符元数
			char op = (char)db2ele(*exp);//运算符
			switch (ary)
			{
			case 1:
			{
				element_t* popped = stack_pop(stk_num);//从栈中取出操作数
				if (!popped)
				{
					stack_del(&stk_num);
					THROW(MAIN, exceptions[5]);//无效后缀表达式
				}
				stack_push(stk_num, db2ele(atomic_calc(ary, op, ele2db(*popped))));
				break;
			}
			case 2:
			{
				element_t* popped[2];
				popped[0] = stack_pop(stk_num);
				popped[1] = stack_pop(stk_num);
				if (!popped[0] || !popped[1])
				{
					stack_del(&stk_num);
					THROW(MAIN, exceptions[5]);//无效后缀表达式
				}
				//先pop的作为右操作数:
				stack_push(stk_num, db2ele(atomic_calc(ary, op, ele2db(*popped[1]), ele2db(*popped[0]))));
				break;
			}
			default:
				stack_del(&stk_num);
				THROW(MAIN, exceptions[3]);//意外的运算符
				break;
			}
			break;
		}
		default:
			stack_del(&stk_num);
			THROW(MAIN, exceptions[4]);//无效后缀表达式信息
			break;
		}
	}
	if (stack_num(stk_num) != 1)
	{
		stack_del(&stk_num);
		THROW(MAIN, exceptions[5]);//表达式运算完毕后栈中并非一个元素
	}
	double ret = ele2db(*stack_pop(stk_num));//运算结果
	stack_del(&stk_num);
	return ret;
}
double ele2db(element_t ele)
{
	double ret;
	memcpy(&ret, &ele, sizeof(element_t));
	return ret;
}
element_t db2ele(double db)
{
	element_t ret;
	memcpy(&ret, &db, sizeof(double));
	return ret;
}
