//////////////////////////////////////////////////////////////////////////////
//expression.c - ������ʽ��ֵ
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
//���������
const int operators[][3] = {
	//�����Ԫ��,�����,��������ȼ�
	{1,'!',0},//һԪ�׳�
	{1,'+',1},//һԪ��
	{1,'-',1},//һԪ��
	{2,'^',2},//��Ԫ�˷�
	{2,'*',3},//��Ԫ��
	{2,'/',3},//��Ԫ��
	{2,'%',3},//��Ԫȡ��
	{2,'+',4},//��Ԫ��
	{2,'-',4},//��Ԫ��
	{0,'(',10}//������
};
//�쳣��Ϣ
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
	stack* stk_op;//�����ջ
	const char* prev = NULL;//ǰһ�����ʽ�ַ�ָ��(�����ո�)
	size_t len;//��׺���ʽ����
	double* out_exp = out_exp_postfix->exp;;//������ʽ����
	int* out_info = out_exp_postfix->inf;//������ʽ��Ϣ

	//��׺���ʽ���ȼ��:
	len = strlen(exp_infix);
	if (len > EXPRESSION_MAX_LEN)THROW(MAIN, exceptions[6]);//���ʽ����
	//�½�ջ:
	stk_op = stack_new(STACK_MAX_SIZE);//�½�ջ
	if (!stk_op)THROW(MAIN, exceptions[0]);//ջ����ʧ��
	//�������ʽ:
	for (; *exp_infix != '\0'; exp_infix++)
	{
		//�����հ��ַ�:
		if (isspace(*exp_infix))continue;
		//��������:
		if (isdigit(*exp_infix) || *exp_infix == '.')
		{
			double num;
			char* ptr = NULL;

			num = strtod(exp_infix, &ptr);
			*out_exp++ = num;//ֱ�ӽ����ַ���������ʽ��������
			*out_info++ = ET_NUMBER;//��ֵ

			exp_infix = ptr - 1;//�������������ַ�ǰһ���ַ�
		}
		//���������:
		else if (*exp_infix != '(' && is_operator(*exp_infix))//�������
		{
			element_t* top = NULL;//ջ��
			int pr;//��ǰ��������ȼ�
			int8_t ary;//��ǰ�����Ԫ��

			ary = strchr("+-", *exp_infix) &&
				(prev == NULL || (*prev != '.' && !isdigit(*prev) && *prev != '!')) ? 1 : 2;//�����Ԫ��
			ary = strchr("!", *exp_infix) ? 1 : ary;//����һԪ�����
			pr = get_op_priority(ary, *exp_infix);//��ǰ����������ȼ�

			while ((top = stack_top(stk_op)) &&
				(ary == 1 ? pr > get_op_priority(OPSTK_ARY(*top), OPSTK_CHAR(*top)) :
					pr >= get_op_priority(OPSTK_ARY(*top), OPSTK_CHAR(*top))))
				//һԪ������Ͷ�Ԫ���������Դ�,�ظ���ջ,ֱ��С�ڻ�ջ��
			{
				element_t* popped = stack_pop(stk_op);
				*out_exp++ = ele2db(OPSTK_CHAR(*popped));
				*out_info++ = MAKE_EXP_INFO_ELE(OPSTK_ARY(*popped), ET_OPERATOR);//�����
			}
			stack_push(stk_op, MAKE_OP_STACK_ELE(ary, *exp_infix));//��ջ
		}
		else if (*exp_infix == '(')
		{
			stack_push(stk_op, MAKE_OP_STACK_ELE(0, *exp_infix));//��ջ
		}
		else if (*exp_infix == ')')
		{
			element_t* popped = NULL;
			//ѭ����ջֱ������������:
			while ((popped = stack_pop(stk_op)) && OPSTK_CHAR(*popped) != '(')
			{
				*out_exp++ = ele2db(OPSTK_CHAR(*popped));
				*out_info++ = MAKE_EXP_INFO_ELE(OPSTK_ARY(*popped), ET_OPERATOR);//�����
			}
			if (!popped)
			{
				stack_del(&stk_op);
				THROW(MAIN, exceptions[1]);//���Ų�ƥ��
			}
		}
		else
		{
			stack_del(&stk_op);
			THROW(MAIN, exceptions[2]);//����������ַ�
		}
		prev = exp_infix;
	}

	//����ջ������ʣ��������:
	element_t* popped = NULL;
	while (popped = stack_pop(stk_op))
	{
		if (OPSTK_CHAR(*popped) == '(')
		{
			stack_del(&stk_op);
			THROW(MAIN, exceptions[1]);//���Ų�ƥ��
		}

		*out_exp++ = ele2db(OPSTK_CHAR(*popped));
		*out_info++ = MAKE_EXP_INFO_ELE(OPSTK_ARY(*popped), ET_OPERATOR);//�����
	}

	stack_del(&stk_op);

	out_exp_postfix->len = out_exp - out_exp_postfix->exp;
	return out_exp - out_exp_postfix->exp;
}
double atomic_calc(int8_t ary, char op, ...)
{
	va_list valist;

	va_start(valist, op);

	double a[10];//������
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

			if (i < 0)THROW(MAIN, exceptions[9]);//���ܲ�������
			if (a[0] - i > DOUBLE_ERROR)THROW(MAIN, exceptions[10]);//���ܲ���С��
			for (; i >= 1; ret *= i--);
			return ret;
		}
		default:
			THROW(MAIN, exceptions[3]);//��Ч�����
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
			//�����ԭʼ��ֵ,δ��������,����ֱ�ӱȽ�:
			if (a[1] == 0.)THROW(MAIN, exceptions[8]);//��������Ϊ0
			return a[0] / a[1];
		case '%':
			//�����ԭʼ��ֵ,δ��������,����ֱ�ӱȽ�:
			if (a[1] == 0.)THROW(MAIN, exceptions[8]);//��������Ϊ0
			return fmod(a[0], a[1]);
		case '^':
			return pow(a[0], a[1]);
		default:
			THROW(MAIN, exceptions[3]);//��Ч�����
			break;
		}
	default:
		THROW(MAIN, exceptions[3]);//��Ч�����
		break;
	}
	va_end(valist);
	return 0.;
}
double calc(const expr* exp_postfix)
{
	stack* stk_num;//������ջ
	const double* const exp_end_c = exp_postfix->exp + exp_postfix->len;//���ʽ��ֹλ��
	const double* exp = exp_postfix->exp;//���ʽ����
	const int* exp_info = exp_postfix->inf;//���ʽ��Ϣ
	//�½�ջ:
	stk_num = stack_new(STACK_MAX_SIZE);
	if (!stk_num)THROW(MAIN, exceptions[0]);
	//����:
	for (; exp != exp_end_c; exp++, exp_info++)
	{
		switch (EXPINF_TYPE(*exp_info))
		{
		case ET_NUMBER:
			stack_push(stk_num, db2ele(*exp));
			break;
		case ET_OPERATOR:
		{
			int8_t ary = EXPINF_ARY(*exp_info);//ȡ�����Ԫ��
			char op = (char)db2ele(*exp);//�����
			switch (ary)
			{
			case 1:
			{
				element_t* popped = stack_pop(stk_num);//��ջ��ȡ��������
				if (!popped)
				{
					stack_del(&stk_num);
					THROW(MAIN, exceptions[5]);//��Ч��׺���ʽ
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
					THROW(MAIN, exceptions[5]);//��Ч��׺���ʽ
				}
				//��pop����Ϊ�Ҳ�����:
				stack_push(stk_num, db2ele(atomic_calc(ary, op, ele2db(*popped[1]), ele2db(*popped[0]))));
				break;
			}
			default:
				stack_del(&stk_num);
				THROW(MAIN, exceptions[3]);//����������
				break;
			}
			break;
		}
		default:
			stack_del(&stk_num);
			THROW(MAIN, exceptions[4]);//��Ч��׺���ʽ��Ϣ
			break;
		}
	}
	if (stack_num(stk_num) != 1)
	{
		stack_del(&stk_num);
		THROW(MAIN, exceptions[5]);//���ʽ������Ϻ�ջ�в���һ��Ԫ��
	}
	double ret = ele2db(*stack_pop(stk_num));//������
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
