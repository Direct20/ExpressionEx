//////////////////////////////////////////////////////////////////////////////
//excp.h - �쳣����
//////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _EXCEP_H_
#define _EXCEP_H_
#include <setjmp.h>
//////////////////////////////////////////////////////////////////////////////
//��ת����
typedef struct
{
	//��ת������
	jmp_buf buf;
	//throw����
	const void* e;
}jmp_data;
//�쳣��Ϣ
typedef struct
{
	//�쳣����
	int c;
	//�ַ�������
	char str[256];
}exc;
//////////////////////////////////////////////////////////////////////////////
//����try���ǩ
#define DECLARE_TRY_LABEL(name)			extern jmp_data _jmpdata_##name
//����try���ǩ
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