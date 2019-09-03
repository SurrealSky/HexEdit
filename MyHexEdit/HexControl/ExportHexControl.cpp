#include "stdafx.h"
#include "HexControlInclude.h"
#include "HexEditViewPage.h"


CWnd* HexEditControl::ShowHexControl(HWND hMainWnd, int x,int y,int cx,int cy)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HWND ret_hwnd = 0;
	// 此处为普通函数体
	CHexEditViewPage *pDlg = new  CHexEditViewPage();
	CWnd * pMainWnd = CWnd::FromHandle(hMainWnd);
	ASSERT(pMainWnd);
	BOOL retValue = pDlg->Create(IDD_HEXEDIT_VIEW_PAGE, pMainWnd);
	if (!retValue)
	{
		AfxMessageBox("创建包含树列表控件的对话框失败了!");
		return 0;
	}
	pDlg->SetResizeStyle(FALSE);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->MoveWindow(x,y,cx,cy, TRUE);
	return pDlg;
}

CWnd* HexEditControl::ShowHexControlEx(HWND hMainWnd, unsigned int x, unsigned int y, unsigned int cx, unsigned int cy, BOOL isCanMove, BOOL isCanResize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HWND ret_hwnd = 0;
	// 此处为普通函数体
	CHexEditViewPage *pDlg = new  CHexEditViewPage(isCanMove);
	CWnd * pMainWnd = CWnd::FromHandle(hMainWnd);
	ASSERT(pMainWnd);
	BOOL retValue = pDlg->Create(IDD_HEXEDIT_VIEW_PAGE, pMainWnd);
	if (!retValue)
	{
		AfxMessageBox("创建包含树列表控件的对话框失败了!");
		return 0;
	}
	pDlg->SetResizeStyle(isCanResize);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->MoveWindow(x,y,cx,cy,TRUE);
	return pDlg;
}

void HexEditControl::SetData(CWnd *p, BYTE *data, unsigned __int64 len)
{
	CHexEditViewPage *pDlg = static_cast<CHexEditViewPage*>(p);
	pDlg->SetData(data, len);
}

 unsigned __int64 HexEditControl::GetDataLength(CWnd* p)
 {
	 CHexEditViewPage *pDlg = static_cast<CHexEditViewPage*>(p);
	 return pDlg->GetDataLen();
 }

 void HexEditControl::GetData(CWnd *p, BYTE *data, unsigned __int64 len)
 {
	 CHexEditViewPage *pDlg = static_cast<CHexEditViewPage*>(p);
	 pDlg->GetData(data, len);
 }
