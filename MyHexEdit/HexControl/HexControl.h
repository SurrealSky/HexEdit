// HexControl.h : HexControl DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHexControlApp
// �йش���ʵ�ֵ���Ϣ������� HexControl.cpp
//

class CHexControlApp : public CWinApp
{
public:
	CHexControlApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
