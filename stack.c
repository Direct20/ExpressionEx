//////////////////////////////////////////////////////////////////////////////
//stack.c - Õ»
//////////////////////////////////////////////////////////////////////////////
#include "stack.h"
stack* stack_new(size_t max_size)
{
	//ÉêÇëstack½á¹¹ÌåÄÚ´æ
	stack* p = malloc(sizeof(stack));
	if (!p)return NULL;//ÉêÇëÊ§°Ü

	//ÉêÇëÔªËØÄÚ´æ
	p->elements = calloc(max_size, sizeof(element_t));
	if (!p->elements)return NULL;//ÉêÇëÊ§°Ü

	p->ptr = p->elements;

	return p;
}
void stack_del(stack** stack_)
{
	if (!stack_ || !*stack_)return;
	free((*stack_)->elements);
	free(*stack_);
	*stack_ = NULL;
}
element_t* stack_push(stack* stack_, element_t element)
{
	if (!stack_)return NULL;
	*stack_->ptr = element;
	stack_->ptr++;
	return stack_->ptr - 1;
}
element_t* stack_pop(stack* stack_)
{
	if (!stack_)return NULL;
	if (!stack_top(stack_))return NULL;//Õ»¿Õ
	return (--stack_->ptr);
}
element_t* stack_top(stack* stack_)
{
	if (!stack_)return NULL;
	if (stack_->ptr == stack_->elements)return NULL;//Õ»¿Õ
	return stack_->ptr - 1;
}
size_t stack_num(stack* stack_)
{
	if (!stack_)return 0;
	return (size_t)(stack_->ptr - stack_->elements);
}