#pragma once
#include"resource.h"


// CHexEditViewPage 对话框

//CopyPaste Type
#define CT_Binary		0
#define CT_BinaryString	1		//c array
#define CT_HEXString	2
#define CT_View			3

class CHexEditViewPage : public CDialogEx
{
	DECLARE_DYNAMIC(CHexEditViewPage)

public:
	CHexEditViewPage(CWnd* pParent = NULL);   // 标准构造函数
	CHexEditViewPage(BOOL isA);
	virtual ~CHexEditViewPage();

// 对话框数据
	enum { IDD = IDD_HEXEDIT_VIEW_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnEditCut();
	afx_msg void OnCopyView();
	afx_msg void OnCopyBinary();
	afx_msg void OnCopyString();
	afx_msg void OnCopyHexstring();
	afx_msg void OnPasteBinary();
	afx_msg void OnPasteString();
	afx_msg void OnPasteHexstring();
	afx_msg void OnEditSelectall();
	afx_msg void OnEditDelete();
	afx_msg void OnEditClear();
	afx_msg void OnEditDetail();
private:
	CHexEditViewPage *m_pTipDlg;
	BOOL isCanMove;
	BOOL isCanReszie;
private:
	void DeleteSelection();
	COleDataSource* CreateDataSource(unsigned int type);
	BOOL DoPasteData(COleDataObject* pDataObject, unsigned int type);
	unsigned int GetPasteData(unsigned char *buffer, unsigned int len);
	unsigned int SetPasteView(unsigned char *buffer, unsigned char *srcBuffer, unsigned int nLen);
	unsigned int SetPasteData(unsigned char *buffer, unsigned char *srcBuffer, unsigned int len);
	void HexToStr(BYTE *pbSrc, int nLen, BYTE *dstbuffer);
	void StrToHex(BYTE *pbSrc, int nLen, BYTE *dstbuffer);
public:
	HWND hwndHV;
	static WNDPROC mDefWindowProc;
public:
	void SetResizeStyle(BOOL isA);
	unsigned __int64 GetCurSelLen();
	void GetCurSelData(BYTE *buffer, unsigned __int64);
	unsigned __int64 GetDataLen();
	void GetData(BYTE *buffer, unsigned __int64);
	void SetData(BYTE *buffer, unsigned __int64);
	static LRESULT CALLBACK HexViewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
