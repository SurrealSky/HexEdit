#include<string>
#include"HexControl.h"
#include"HexView.h"
#include<LogLib\DebugLog.h>
#include"resource.h"

#define WM_EDIT_CUT		WM_USER+100
#define WM_COPY_VIEW	WM_USER+101
#define WM_COPY_BINARY	WM_USER+102
#define WM_COPY_STRING	WM_USER+103
#define WM_COPY_HEXSTRING	WM_USER+104
#define WM_PASTE_BINARY	WM_USER+105
#define WM_PASTE_STRING	WM_USER+106
#define WM_PASTE_HEXSTRING	WM_USER+107
#define WM_EDIT_SELECTALL	WM_USER+108
#define WM_EDIT_DELETE		WM_USER+109
#define WM_EDIT_CLEAR		WM_USER+110

//CopyPaste Type
#define CT_Binary		0
#define CT_BinaryString	1		//c array
#define CT_HEXString	2
#define CT_View			3


HexControl::HexControl()
{
}

HexControl::~HexControl()
{
}

void HexToStr(BYTE *pbSrc, int nLen, BYTE *dstbuffer)
{
	char ddl, ddh;
	int i;
	int j;
	for (i = 0, j = 0; i<nLen; i++, j += 2)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		dstbuffer[j] = ddh;
		dstbuffer[j + 1] = ddl;
	}
}

void StrToHex(BYTE *pbSrc, int nLen, BYTE *dstbuffer)
{
	std::string str;
	char h1, h2;
	BYTE s1, s2;
	int i;

	for (i = 0; i<nLen / 2; i++)
	{
		h1 = pbSrc[2 * i];
		h2 = pbSrc[2 * i + 1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;

		dstbuffer[i] = s1 * 16 + s2;
	}
}

//读出数据，输出成数组格式放到粘贴板
unsigned int SetPasteView(unsigned char *buffer, unsigned char *srcBuffer, unsigned int nLen)
{
	unsigned retlen = 0;

	std::string strContent = "";
	//先转换为小写
	unsigned int row = 0;
	unsigned int sub = 0x10;
	if (nLen % 16 == 0)
		row = nLen / 16;
	else
	{
		row = nLen / 16 + 1;
		sub = nLen % 16;
	}

	for (int r = 0; r < row; r++)
	{
		char ddl, ddh;
		int i;
		int rowlen = 0x10;
		if (r == row - 1)
			rowlen = sub;
		for (i = 0; i<rowlen; i++)
		{
			ddh = 48 + srcBuffer[i + r * 0x10] / 16;
			ddl = 48 + srcBuffer[i + r * 0x10] % 16;
			if (ddh > 57) ddh = ddh + 7;
			if (ddl > 57) ddl = ddl + 7;
			strContent.push_back(ddh);
			strContent.push_back(ddl);
			strContent.push_back(' ');
		}
		if (rowlen != 0x10)
		{
			strContent.insert(strContent.size(), (0x10 - rowlen) * 3, ' ');
		}
		//添加assic
		for (i = 0; i < rowlen; i++)
		{
			char c = srcBuffer[i + r * 0x10];
			if (c < ' ' || c > '~')
				c = '.';
			strContent.push_back(c);
			/*	*sBuffer++ = c;

			if (srcBuffer[i + r * 0x10]!=0)
			strContent.push_back(srcBuffer[i + r * 0x10]);
			else
			strContent.push_back('.');*/

		}
		if (r != row - 1)
		{
			strContent.push_back(0xd);
			strContent.push_back(0xa);
		}

	}
	retlen = strContent.size();
	memcpy(buffer, strContent.c_str(), strContent.size());
	return retlen;
}

//读出数据，输出成数组格式放到粘贴板
unsigned int SetPasteData(unsigned char *buffer, unsigned char *srcBuffer, unsigned int len)
{
	unsigned retlen = 0;

	char *dst = (char*)malloc(len * 2);
	memset(dst, 0, len * 2);
	char ddl, ddh;
	for (int i = 0; i<len; i++)
	{
		ddh = 48 + srcBuffer[i] / 16;
		ddl = 48 + srcBuffer[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		dst[i * 2] = ddh;
		dst[i * 2 + 1] = ddl;
	}

	unsigned int j = 0;
	for (int i = 0; i < len * 2;)
	{
		buffer[j++] = '0';
		buffer[j++] = 'x';
		buffer[j++] = dst[i++];
		buffer[j++] = dst[i++];
		buffer[j++] = ',';
	}

	retlen = j;
	return retlen;
}

unsigned __int64 GetCurSelLen(HWND m_hWndHexView)
{
	unsigned __int64 size = 0;
	HexView_GetSelSize(m_hWndHexView, &size);
	return size;
}

void GetCurSelData(HWND m_hWndHexView,BYTE *buffer, unsigned __int64 len)
{
	unsigned __int64 oldPos = 0;
	HexView_GetCurPos(m_hWndHexView, &oldPos);
	unsigned __int64 selStart = 0;
	HexView_GetSelStart(m_hWndHexView, &selStart);
	HexView_SetCurPos(m_hWndHexView, selStart);
	HexView_GetDataCur(m_hWndHexView, buffer, len);
	HexView_SetCurPos(m_hWndHexView, oldPos);
}

void SetData(HWND m_hWndHexView,BYTE *data, unsigned __int64 len)
{
	HexView_Clear(m_hWndHexView);
	HexView_SetDataCur(m_hWndHexView, data, len);
	HexView_SetCurPos(m_hWndHexView, 0);
	HexView_SetRedraw(m_hWndHexView, true);
}

void GetData(HWND m_hWndHexView,BYTE *data, unsigned __int64 len)
{
	unsigned __int64 selStart = 0;
	HexView_GetSelStart(m_hWndHexView, &selStart);
	unsigned __int64 selEnd = 0;
	HexView_GetSelEnd(m_hWndHexView, &selEnd);
	HexView_SetCurPos(m_hWndHexView, 0);
	HexView_GetDataCur(m_hWndHexView, data, len);
	HexView_SetSelStart(m_hWndHexView, selStart);
	HexView_SetSelEnd(m_hWndHexView, selEnd);
}

void OnEditCut(HWND m_hWndHexView)
{
	unsigned int oldEditMode = HexView_GetEditMode(m_hWndHexView);
	HexView_SetEditMode(m_hWndHexView, HVMODE_INSERT);
	HexView_Cut(m_hWndHexView);
	HexView_SetEditMode(m_hWndHexView, oldEditMode);
}

void OnCopyView(HWND m_hWndHexView)
{
	unsigned __int64 nSrcLength = GetCurSelLen(m_hWndHexView);
	if (nSrcLength > 0)
	{
		BYTE* lpSrcByte = new BYTE[nSrcLength];
		GetCurSelData(m_hWndHexView,lpSrcByte, nSrcLength);
		if (OpenClipboard(m_hWndHexView)) {
			EmptyClipboard();

			unsigned int nSelLength = nSrcLength;
			unsigned int nLength = 0;
			if (nSelLength < 16)
			{
				nLength = 16 * 3 + nSelLength;
			}
			else
			{
				if (nSelLength % 16 == 0)
					nLength = nSelLength * 4 + (nSelLength / 16 - 1) * 2;
				else
					nLength = (nSelLength / 16) * 4 * 16 + 16 * 3 + nSelLength / 16 * 2 + nSelLength % 16;
			}
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLength + 1);
			if (hMem == NULL) return;
			unsigned char *lpByte = (BYTE*)::GlobalLock(hMem);
			nLength = SetPasteView(lpByte, lpSrcByte, nSelLength);
			delete[]lpSrcByte;
			lpByte[nLength] = 0;
			GlobalUnlock(hMem);
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
		}
	}
}

void OnCopyBinary(HWND m_hWndHexView)
{
	HexView_Copy(m_hWndHexView);
}

void OnCopyString(HWND m_hWndHexView)
{
	/*COleDataSource* pDataSource = CreateDataSource(CT_BinaryString);
	if (pDataSource != NULL)
	pDataSource->SetClipboard();*/

	unsigned __int64 nSrcLength = GetCurSelLen(m_hWndHexView);
	if (nSrcLength > 0)
	{
		BYTE* lpSrcByte = new BYTE[nSrcLength];
		GetCurSelData(m_hWndHexView,lpSrcByte, nSrcLength);
		if (OpenClipboard(m_hWndHexView)) {
			EmptyClipboard();

			unsigned int nSelLength = nSrcLength;
			unsigned int nLength = nSelLength * 5;
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLength + 1);
			if (hMem == NULL) return;
			unsigned char *lpByte = (BYTE*)::GlobalLock(hMem);
			SetPasteData(lpByte, lpSrcByte, nSelLength);
			delete[]lpSrcByte;
			lpByte[nLength] = 0;

			GlobalUnlock(hMem);
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
		}
	}

}

void OnCopyHexstring(HWND m_hWndHexView)
{
	//COleDataSource* pDataSource = CreateDataSource(CT_HEXString);
	//if (pDataSource != NULL)
	//	pDataSource->SetClipboard();
	unsigned __int64 nSrcLength = GetCurSelLen(m_hWndHexView);
	if (nSrcLength > 0)
	{
		BYTE* lpSrcByte = new BYTE[nSrcLength];
		GetCurSelData(m_hWndHexView,lpSrcByte, nSrcLength);
		if (OpenClipboard(m_hWndHexView)) {
			EmptyClipboard();

			unsigned int nSelLength = nSrcLength;
			unsigned int nLength = nSelLength * 2;
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLength + 1);
			if (hMem == NULL) return;
			unsigned char *lpByte = (BYTE*)::GlobalLock(hMem);
			HexToStr(lpSrcByte, nSelLength, lpByte);
			delete[]lpSrcByte;
			lpByte[nLength] = 0;

			GlobalUnlock(hMem);
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
		}
	}
}

void OnPasteBinary(HWND m_hWndHexView)
{
	HexView_Paste(m_hWndHexView);
}

//把粘贴板数据按照固定格式读出
unsigned int GetPasteData(unsigned char *buffer, unsigned int len)
{
	//先判断格式
	//if (buffer[0] != '{'||buffer[len - 1] != '}') return 0;
	unsigned int retlen = 0;
	char *dst1 = (char*)malloc(len);
	memset(dst1, 0, len);
	int j = 0;
	for (int i = 0; i <= len - 4;)
	{
		if (buffer[i] == 0x30 && buffer[i + 1] == 0x78)
		{
			dst1[j++] = buffer[i + 2];
			dst1[j++] = buffer[i + 3];
			i += 4;
			continue;
		}
		i++;
	}
	memset(buffer, 0, len);
	retlen = j / 2;
	//在进行转换
	char h1, h2;
	BYTE s1, s2;
	int i;

	for (i = 0; i * 2<j; i++)
	{
		h1 = dst1[2 * i];
		h2 = dst1[2 * i + 1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;

		buffer[i] = s1 * 16 + s2;
	}
	free(dst1);
	return retlen;
}

void OnPasteString(HWND m_hWndHexView)
{
	//COleDataObject dataObject;
	////DeleteSelection();
	//if (dataObject.AttachClipboard())
	//	DoPasteData(&dataObject, CT_BinaryString);
	if (OpenClipboard(m_hWndHexView)) {
		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData != NULL) {
			unsigned char* lpByte = (unsigned char*)GlobalLock(hData);
			unsigned int nLength = GlobalSize(hData);

			nLength = GetPasteData(lpByte, nLength);
			HexView_SetDataCur(m_hWndHexView, lpByte, nLength);
			HexView_SetRedraw(m_hWndHexView, true);

			GlobalUnlock(hData);
		}
		CloseClipboard();
	}
}

void OnPasteHexstring(HWND m_hWndHexView)
{
	//COleDataObject dataObject;
	////DeleteSelection();
	//if (dataObject.AttachClipboard())
	//	DoPasteData(&dataObject, CT_HEXString);
	if (OpenClipboard(m_hWndHexView)) {
		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData != NULL) {
			unsigned char* lpByte = (unsigned char*)GlobalLock(hData);
			unsigned int nLength = GlobalSize(hData);

			BYTE *buffer = (BYTE*)malloc(nLength / 2 + 1);
			memset(buffer, 0, nLength / 2 + 1);
			StrToHex(lpByte, nLength, buffer);
			nLength = nLength / 2;
			memcpy(lpByte, buffer, nLength);
			free(buffer);
			buffer = NULL;
			HexView_SetDataCur(m_hWndHexView, lpByte, nLength);
			HexView_SetRedraw(m_hWndHexView, true);

			GlobalUnlock(hData);
		}
		CloseClipboard();
	}
}

void OnEditSelectall(HWND m_hWndHexView)
{
	// TODO:  在此添加命令处理程序代码
	HexView_SelectAll(m_hWndHexView);
}

void OnEditDelete(HWND m_hWndHexView)
{
	// TODO:  在此添加命令处理程序代码
	unsigned __int64 selStart = 0;
	HexView_GetSelStart(m_hWndHexView, &selStart);

	unsigned __int64 len = 0;
	HexView_GetFileSize(m_hWndHexView, &len);

	BYTE *saveData = new BYTE[len];
	GetData(m_hWndHexView,saveData, len);
	unsigned __int64 selSize = 0;
	HexView_GetSelSize(m_hWndHexView, &selSize);
	if (selSize != len)
	{
		std::string str;
		str.append((char*)saveData, len);
		str.erase(selStart, selSize);
		SetData(m_hWndHexView,(BYTE*)str.c_str(), str.size());
		HexView_SetSelStart(m_hWndHexView, selStart);
		HexView_SetSelEnd(m_hWndHexView, selStart);
	}
	else
		HexView_Clear(m_hWndHexView);
}

void OnEditClear(HWND m_hWndHexView)
{
	// TODO:  在此添加命令处理程序代码
	HexView_Clear(m_hWndHexView);
}

WNDPROC mDefWindowProc=0;
LRESULT CALLBACK MyHexViewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CHAR)
	{
		if (GetKeyState(VK_CONTROL) < 0)
		{
			// Check for clipboard commands
			switch ((UINT)wParam)
			{
			case 0x18:	// Ctrl+X - Cut
			{
				unsigned int oldEditMode = HexView_GetEditMode(hwnd);
				HexView_SetEditMode(hwnd, HVMODE_INSERT);
				HexView_Cut(hwnd);
				HexView_SetEditMode(hwnd, oldEditMode);
			}
			break;
			case 0x03:	// Ctrl+C - Copy
				HexView_Copy(hwnd);
				break;
			case 0x16:	// Ctrl+V - Paste
				HexView_Paste(hwnd);
				break;
			case 0x01:// Ctrl+A - Select All
				HexView_SelectAll(hwnd);
				break;
			}
		}
	}
	else if (msg == WM_COMMAND)
	{
		SHORT nMenuID = LOWORD(wParam);
		switch (nMenuID)
		{
			case WM_EDIT_CUT:
			{
				OnEditCut(hwnd);
			}break;
			case WM_COPY_VIEW:
			{
				OnCopyView(hwnd);
			}break;
			case WM_COPY_BINARY:
			{
				OnCopyBinary(hwnd);
			}break;
			case WM_COPY_STRING:
			{
				OnCopyString(hwnd);
			}break;
			case WM_COPY_HEXSTRING:
			{
				OnCopyHexstring(hwnd);
			}break;
			case WM_PASTE_BINARY:
			{
				OnPasteBinary(hwnd);
			}break;
			case WM_PASTE_STRING:
			{
				OnPasteString(hwnd);
			}break;
			case WM_PASTE_HEXSTRING:
			{
				OnPasteHexstring(hwnd);
			}break;
			case WM_EDIT_SELECTALL:
			{
				OnEditSelectall(hwnd);
			}break;
			case WM_EDIT_DELETE:
			{
				OnEditDelete(hwnd);
			}break;
			case WM_EDIT_CLEAR:
			{
				OnEditClear(hwnd);
			}break;
		}
	}
	return mDefWindowProc(hwnd, msg, wParam, lParam);
}

void HexControl::CreateHexView(HINSTANCE hinstance,HWND hParent)
{
	m_hWndParent = hParent;
	m_hWndHexView = ::CreateHexView(m_hWndParent);
	// set the right-click context menu

	//创建菜单
	HMENU hMenu = CreatePopupMenu();
	// do we have a selection?
	/*UINT fSelection = (m_nSelectionStart == m_nSelectionEnd) ?
		MF_DISABLED | MF_GRAYED : MF_ENABLED;*/
	UINT fSelection = (0 == 0) ?
		MF_DISABLED | MF_GRAYED : MF_ENABLED;
	// is there text on the clipboard?
	UINT fClipboard = IsClipboardFormatAvailable(CF_TEXT) ?
		MF_ENABLED : MF_GRAYED | MF_DISABLED;
	UINT fCanUndo = /*CanUndo()*/ TRUE? MF_ENABLED : MF_GRAYED | MF_DISABLED;
	UINT fCanRedo = /*CanRedo()*/TRUE ? MF_ENABLED : MF_GRAYED | MF_DISABLED;
	AppendMenu(hMenu, MF_STRING | fCanUndo, WM_EDIT_CUT, ("Cu&t"));
	//copy子菜单
	HMENU submenucopy= CreatePopupMenu();
	AppendMenu(submenucopy, MF_STRING | fCanUndo, WM_COPY_BINARY, "Default");
	AppendMenu(submenucopy, MF_SEPARATOR, 0, 0);
	AppendMenu(submenucopy, MF_STRING | fCanUndo, WM_COPY_VIEW, "Current View");
	AppendMenu(submenucopy, MF_STRING | fCanUndo, WM_COPY_STRING, "C Array");
	AppendMenu(submenucopy, MF_STRING | fCanUndo, WM_COPY_HEXSTRING, "HEX String");
	AppendMenu(hMenu, MF_BYPOSITION | MF_POPUP, UINT(submenucopy), ("C&opy"));

	//paste子菜单
	HMENU submenupaste = CreatePopupMenu();
	AppendMenu(submenupaste, MF_STRING | fCanUndo, WM_PASTE_BINARY, "Default");
	AppendMenu(submenupaste, MF_SEPARATOR, 0, 0);
	AppendMenu(submenupaste, MF_STRING | fCanUndo, WM_PASTE_STRING, "C Array");
	AppendMenu(submenupaste, MF_STRING | fCanUndo, WM_PASTE_HEXSTRING, "HEX String");
	AppendMenu(hMenu, MF_BYPOSITION | MF_POPUP, UINT(submenupaste), ("P&aste"));

	AppendMenu(hMenu, MF_STRING | fCanUndo, WM_EDIT_DELETE, "&Delete");
	AppendMenu(hMenu, MF_STRING | fCanUndo, WM_EDIT_CLEAR, "Clear");
	AppendMenu(hMenu, MF_STRING | fCanUndo, WM_EDIT_SELECTALL, "&Selete All");
	
	HexView_SetContextMenu(m_hWndHexView, hMenu);

	// set the default styles
	//DWORD dwStyle = HVS_RESIZEBAR | HVS_ALWAYSVSCROLL | HVS_ALWAYSHSCROLL | HVS_FORCE_FIXEDCOLS;// | HVS_FITTOWINDOW;
	DWORD dwStyle = HVS_RESIZEBAR | HVS_ALWAYSDELETE;// | HVS_FITTOWINDOW;
	HexView_SetStyle(m_hWndHexView, -1, dwStyle);
	HexView_SetDataShift(m_hWndHexView, 0);
	// 设置字体
	HFONT g_hHexViewFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	::SendMessage(m_hWndHexView, WM_SETFONT, (WPARAM)g_hHexViewFont, 0);
	HexView_SetFontSpacing(m_hWndHexView, 0, 0);
	//设置颜色
	HexView_SetColor(m_hWndHexView, HVC_MODIFY, RGB(28, 28, 28));
	//设置数据显示样式
	HexView_SetGrouping(m_hWndHexView, 1);
	//设置窗口位置
	::SetWindowPos(m_hWndHexView, m_hWndParent, 0,0,100, 50, SWP_NOZORDER | SWP_SHOWWINDOW);
	//定位消息处理函数
	mDefWindowProc = (WNDPROC)GetWindowLong(m_hWndHexView, GWL_WNDPROC);
	SetWindowLong(m_hWndHexView, GWL_WNDPROC, (LONG)MyHexViewWndProc);
}

void HexControl::SetPosition(int x, int y, int width, int height)
{
	SurrealDebugLog::DebugLog(SurrealDebugLog::string_format("pos:x=%d,y=%d,width=%d,height=%d", x, y, width, height));
	//CClientDC dc(this);
	//::GetDC(m_hWndHexView);
	//OnPrepareDC(&dc);
	//dc.DPtoLP(&point);
	POINT point[2];
	point[0].x = x;
	point[0].y = y;
	point[1].x = x + width;
	point[1].y = y + height;
	int result=DPtoLP(GetDC(m_hWndParent), point, 2);
	SurrealDebugLog::DebugLog(SurrealDebugLog::string_format("point1:x=%d,y=%d;point2:x=%d,y=%d", point[0].x, point[0].y, point[1].x, point[1].y));
	::SetWindowPos(m_hWndHexView, m_hWndParent, x, y, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
}

void HexControl::SetData(BYTE *data, unsigned __int64 len)
{
	::SetData(m_hWndHexView, data, len);
}

unsigned __int64 HexControl::GetDataLen()
{
	unsigned __int64 len = 0;
	HexView_GetFileSize(m_hWndHexView, &len);
	return len;
}

void HexControl::GetData(BYTE *data, unsigned __int64 len)
{
	::GetData(m_hWndHexView, data, len);
}

void HexControl::SetDataPtr(DWORD_PTR ptr)
{
	ptr = ptr;
}

DWORD_PTR HexControl::GetDataPtr()
{
	return ptr;
}
