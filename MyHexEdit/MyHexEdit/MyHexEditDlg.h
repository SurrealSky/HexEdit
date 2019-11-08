
// MyHexEditDlg.h : 头文件
//

#pragma once
#include"../HexView/HexControl.h"


// CMyHexEditDlg 对话框
class CMyHexEditDlg : public CDialogEx
{
// 构造
public:
	CMyHexEditDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYHEXEDIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	HexControl mHexControl;
	HexControl mHexControl2;
	bool isInit;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
