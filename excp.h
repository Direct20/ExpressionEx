//////////////////////////////////////////////////////////////////////////////
//excp.h - 异常处理
//////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _EXCEP_H_
#define _EXCEP_H_
#include <setjmp.h>
//////////////////////////////////////////////////////////////////////////////
//跳转数据
typedef struct
{
	//跳转缓冲区
	jmp_buf buf;
	//throw数据
	const void* e;
}jmp_data;
//异常信息
typedef struct
{
	//异常代码
	int c;
	//字符串描述
	char str[256];
}exc;
//////////////////////////////////////////////////////////////////////////////
//声明try块标签
#define DECLARE_TRY_LABEL(name)			extern jmp_data _jmpdata_##name
//定义try块标签
#define DEFINE_TRY_LABEL(name)			jmp_data _jmpdata_##name
//try
#define TRY(label_name)					if(!setjmp(_jmpdata_##label_name.buf))
//catch
#define CATCH()							else
#define CATCH_var(label_name)			_jmpdata_##label_name.e
//throw
#define THROW(label_name,x)				_jmpdata_##label_name.e=&(x),\
										longjmp(_jmpdata_##label_name.buf,1)
//////////////////////////////////////////////////////////////////////////////
#endif //_EXCEP_H_