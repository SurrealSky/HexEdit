
// MyHexEditDlg.h : ͷ�ļ�
//

#pragma once
#include"../HexView/HexControl.h"


// CMyHexEditDlg �Ի���
class CMyHexEditDlg : public CDialogEx
{
// ����
public:
	CMyHexEditDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYHEXEDIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
