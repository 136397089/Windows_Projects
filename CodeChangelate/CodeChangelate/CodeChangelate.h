
// CodeChangelate.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCodeChangelateApp: 
// �йش����ʵ�֣������ CodeChangelate.cpp
//

class CCodeChangelateApp : public CWinApp
{
public:
	CCodeChangelateApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCodeChangelateApp theApp;