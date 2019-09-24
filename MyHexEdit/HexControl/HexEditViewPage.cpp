// HexEditViewPage.cpp : 实现文件
//

#include "stdafx.h"
#include<string>
#include "HexControl.h"
#include "HexEditViewPage.h"
#include "afxdialogex.h"

// CHexEditViewPage 对话框

WNDPROC CHexEditViewPage::mDefWindowProc = 0;

IMPLEMENT_DYNAMIC(CHexEditViewPage, CDialogEx)

CHexEditViewPage::CHexEditViewPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHexEditViewPage::IDD, pParent)
{
	isCanMove=FALSE;
	isCanReszie=FALSE;
	m_pTipDlg = NULL;
	ptr = 0;
}

CHexEditViewPage::CHexEditViewPage(BOOL isA)
: CDialogEx(CHexEditViewPage::IDD, NULL)
{
	isCanMove = isA;
	m_pTipDlg = NULL;
	ptr = 0;
}

CHexEditViewPage::~CHexEditViewPage()
{
	if (NULL != m_pTipDlg)
	{
		delete m_pTipDlg;
		m_pTipDlg = NULL;
	}
}

void CHexEditViewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHexEditViewPage, CDialogEx)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_NCMOUSEMOVE()
	ON_COMMAND(ID_EDIT_CUT, &CHexEditViewPage::OnEditCut)
	ON_COMMAND(ID_COPY_VIEW, &CHexEditViewPage::OnCopyView)
	ON_COMMAND(ID_COPY_BINARY, &CHexEditViewPage::OnCopyBinary)
	ON_COMMAND(ID_COPY_STRING, &CHexEditViewPage::OnCopyString)
	ON_COMMAND(ID_COPY_HEXSTRING, &CHexEditViewPage::OnCopyHexstring)
	ON_COMMAND(ID_PASTE_BINARY, &CHexEditViewPage::OnPasteBinary)
	ON_COMMAND(ID_PASTE_STRING, &CHexEditViewPage::OnPasteString)
	ON_COMMAND(ID_PASTE_HEXSTRING, &CHexEditViewPage::OnPasteHexstring)
	ON_COMMAND(ID_EDIT_SELECTALL, &CHexEditViewPage::OnEditSelectall)
	ON_COMMAND(ID_EDIT_DELETE, &CHexEditViewPage::OnEditDelete)
	ON_COMMAND(ID_EDIT_CLEAR32772, &CHexEditViewPage::OnEditClear)
	ON_COMMAND(ID_EDIT_DETAIL, &CHexEditViewPage::OnEditDetail)
END_MESSAGE_MAP()


// CHexEditViewPage 消息处理程序


BOOL CHexEditViewPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	hwndHV = CreateHexView(m_hWnd);
	// set the right-click context menu
	HMENU hMenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_HEXCTRLMENU));
	HexView_SetContextMenu(hwndHV, GetSubMenu(hMenu, 0));
	// set the default styles
	//DWORD dwStyle = HVS_RESIZEBAR | HVS_ALWAYSVSCROLL | HVS_ALWAYSHSCROLL | HVS_FORCE_FIXEDCOLS;// | HVS_FITTOWINDOW;
	DWORD dwStyle = HVS_RESIZEBAR | HVS_ALWAYSDELETE;// | HVS_FITTOWINDOW;
	HexView_SetStyle(hwndHV, -1, dwStyle);
	HexView_SetDataShift(hwndHV, 0);
	// 设置字体
	HFONT g_hHexViewFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	::SendMessage(hwndHV, WM_SETFONT, (WPARAM)g_hHexViewFont, 0);
	HexView_SetFontSpacing(hwndHV, 0, 0);
	//设置颜色
	HexView_SetColor(hwndHV, HVC_MODIFY, RGB(28, 28, 28));
	//设置数据显示样式
	HexView_SetGrouping(hwndHV, 1);
	//设置窗口位置
	CRect   m_rt;
	GetClientRect(&m_rt);
	::SetWindowPos(hwndHV, m_hWnd, m_rt.left, m_rt.top, m_rt.Width(), m_rt.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
	//定位消息处理函数
	CHexEditViewPage::mDefWindowProc=(WNDPROC)GetWindowLong(hwndHV, GWL_WNDPROC);
	SetWindowLong(hwndHV, GWL_WNDPROC, (LONG)CHexEditViewPage::HexViewWndProc);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

LRESULT CALLBACK CHexEditViewPage::HexViewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
	return mDefWindowProc(hwnd, msg, wParam, lParam);
}

void CHexEditViewPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CRect   m_rt;
	GetClientRect(&m_rt);
	if (isCanMove)
		m_rt.right -= 10;
	::MoveWindow(hwndHV, m_rt.left, m_rt.top, m_rt.Width(), m_rt.Height(), TRUE);
}

LRESULT CHexEditViewPage::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (isCanMove)
	{
		CRect rc1;
		GetWindowRect(&rc1);
		CRect rc2;
		::GetWindowRect(hwndHV, &rc2);
		rc1.left = rc2.right;
		if (rc1.PtInRect(point))
		{
			return HTCAPTION;
		}
	}
	return CDialogEx::OnNcHitTest(point);
}

void CHexEditViewPage::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (isCanMove)
	{
		CRect rc1;
		GetWindowRect(&rc1);
		CRect rc2;
		::GetWindowRect(hwndHV, &rc2);
		rc1.left = rc2.right;
		if (rc1.PtInRect(point))
		{
			::SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		}
	}
	CDialogEx::OnNcMouseMove(nHitTest, point);
}

void CHexEditViewPage::SetResizeStyle(BOOL isA)
{
	isCanReszie = isA;
	if (isCanReszie)
	{
		this->ModifyStyle(0, WS_SIZEBOX);
	}
}

void CHexEditViewPage::OnEditCut()
{
	unsigned int oldEditMode = HexView_GetEditMode(hwndHV);
	HexView_SetEditMode(hwndHV, HVMODE_INSERT);
	HexView_Cut(hwndHV);
	HexView_SetEditMode(hwndHV, oldEditMode);
}

void CHexEditViewPage::OnCopyView()
{
	COleDataSource* pDataSource = CreateDataSource(CT_View);
	if (pDataSource != NULL)
		pDataSource->SetClipboard();
}

void CHexEditViewPage::OnCopyBinary()
{
	//COleDataSource* pDataSource = CreateDataSource(CT_Binary);
	//if (pDataSource != NULL)
	//	pDataSource->SetClipboard();
	HexView_Copy(hwndHV);
}

void CHexEditViewPage::OnCopyString()
{
	COleDataSource* pDataSource = CreateDataSource(CT_BinaryString);
	if (pDataSource != NULL)
		pDataSource->SetClipboard();
}

void CHexEditViewPage::OnCopyHexstring()
{
	COleDataSource* pDataSource = CreateDataSource(CT_HEXString);
	if (pDataSource != NULL)
		pDataSource->SetClipboard();
}

void CHexEditViewPage::DeleteSelection()
{
}

void CHexEditViewPage::OnPasteBinary()
{
	//COleDataObject dataObject;
	////DeleteSelection();
	//if (dataObject.AttachClipboard())
	//	DoPasteData(&dataObject, CT_Binary);
	HexView_Paste(hwndHV);
}

void CHexEditViewPage::OnPasteString()
{
	COleDataObject dataObject;
	//DeleteSelection();
	if (dataObject.AttachClipboard())
		DoPasteData(&dataObject, CT_BinaryString);
}

void CHexEditViewPage::OnPasteHexstring()
{
	COleDataObject dataObject;
	//DeleteSelection();
	if (dataObject.AttachClipboard())
		DoPasteData(&dataObject, CT_HEXString);
}

void CHexEditViewPage::OnEditSelectall()
{
	// TODO:  在此添加命令处理程序代码
	HexView_SelectAll(hwndHV);
}

void CHexEditViewPage::OnEditDelete()
{
	// TODO:  在此添加命令处理程序代码
	unsigned __int64 selStart = 0;
	HexView_GetSelStart(hwndHV, &selStart);

	unsigned int len = GetDataLen();
	BYTE *saveData = new BYTE[len];
	GetData(saveData, len);
	unsigned __int64 selSize = 0;
	HexView_GetSelSize(hwndHV, &selSize);
	if (selSize != len)
	{
		std::string str;
		str.append((char*)saveData, len);
		str.erase(selStart, selSize);
		SetData((BYTE*)str.c_str(), str.size());
		HexView_SetSelStart(hwndHV, selStart);
		HexView_SetSelEnd(hwndHV, selStart);
	}
	else
		HexView_Clear(hwndHV);
}

void CHexEditViewPage::OnEditClear()
{
	// TODO:  在此添加命令处理程序代码
	HexView_Clear(hwndHV);
}

unsigned __int64 CHexEditViewPage::GetCurSelLen()
{
	unsigned __int64 size = 0;
	HexView_GetSelSize(hwndHV, &size);
	return size;
}

void CHexEditViewPage::GetCurSelData(BYTE *buffer,unsigned __int64 len)
{
	unsigned __int64 oldPos = 0;
	HexView_GetCurPos(hwndHV, &oldPos);
	unsigned __int64 selStart = 0;
	HexView_GetSelStart(hwndHV, &selStart);
	HexView_SetCurPos(hwndHV, selStart);
	HexView_GetDataCur(hwndHV, buffer, len);
	HexView_SetCurPos(hwndHV, oldPos);
}

unsigned __int64 CHexEditViewPage::GetDataLen()
{
	unsigned __int64 len = 0;
	HexView_GetFileSize(hwndHV, &len);
	return len;
}

void CHexEditViewPage::GetData(BYTE *buffer, unsigned __int64 len)
{
	unsigned __int64 selStart = 0;
	HexView_GetSelStart(hwndHV, &selStart);
	unsigned __int64 selEnd = 0;
	HexView_GetSelEnd(hwndHV, &selEnd);
	HexView_SetCurPos(hwndHV, 0);
	HexView_GetDataCur(hwndHV, buffer, len);
	HexView_SetSelStart(hwndHV, selStart);
	HexView_SetSelEnd(hwndHV, selEnd);
}

void CHexEditViewPage::SetData(BYTE *buffer, unsigned __int64 len)
{
	HexView_Clear(hwndHV);
	HexView_SetDataCur(hwndHV, buffer, len);
	HexView_SetCurPos(hwndHV, 0);
	HexView_SetRedraw(hwndHV, true);
}

void CHexEditViewPage::SetDataPtr(DWORD_PTR ptr)
{
	this->ptr = ptr;
}

DWORD_PTR CHexEditViewPage::GetDataPtr()
{
	return ptr;
}

//把粘贴板数据按照固定格式读出
unsigned int CHexEditViewPage::GetPasteData(unsigned char *buffer, unsigned int len)
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

// Inserts data from paste or drag and drop operation
BOOL CHexEditViewPage::DoPasteData(COleDataObject* pDataObject, unsigned int type)
{
	HGLOBAL hMem = NULL;
	BYTE* lpByte;
	int nLength = 0;
	BOOL bRetVal = FALSE;

	try
	{
		if (type == CT_Binary)
		{
			if (pDataObject->IsDataAvailable((CLIPFORMAT)CF_TEXT))
			{
				// Get length of CF_TEXT data
				hMem = pDataObject->GetGlobalData((CLIPFORMAT)CF_TEXT);
				if (hMem == NULL)
					AfxThrowMemoryException();
				lpByte = (BYTE*)::GlobalLock(hMem);
				ASSERT(lpByte != NULL);
				nLength = *((int*)lpByte);
				ASSERT(nLength != 0);
				// Requires CF_TEXT for data
				ASSERT(pDataObject->IsDataAvailable(CF_TEXT));
				::GlobalUnlock(hMem);
				::GlobalFree(hMem);
			}
		}

		if (pDataObject->IsDataAvailable(CF_TEXT))
		{
			hMem = pDataObject->GetGlobalData(CF_TEXT);
			if (hMem == NULL)
				AfxThrowMemoryException();
			lpByte = (BYTE*)::GlobalLock(hMem);
			nLength = GlobalSize(hMem);
			ASSERT(lpByte != NULL);
			// If no native length indicator, read as null-terminated string
			if (nLength == 0)
				nLength = strlen((char*)lpByte);
			if (nLength > 0)
			{
				switch (type)
				{
				case CT_Binary:
				{
					// Paste data
					HexView_SetDataCur(hwndHV, lpByte, nLength);
					HexView_SetRedraw(hwndHV, true);
				}break;
				case CT_BinaryString:
				{
					// Paste data
					nLength = GetPasteData(lpByte, nLength);
					HexView_SetDataCur(hwndHV, lpByte, nLength);
					HexView_SetRedraw(hwndHV, true);
				}break;
				case CT_HEXString:
				{
					// Paste data
					BYTE *buffer = (BYTE*)malloc(nLength / 2 + 1);
					memset(buffer, 0, nLength / 2 + 1);
					StrToHex(lpByte, nLength, buffer);
					nLength = nLength / 2;
					memcpy(lpByte, buffer, nLength);
					free(buffer);
					buffer = NULL;
					HexView_SetDataCur(hwndHV, lpByte, nLength);
					HexView_SetRedraw(hwndHV, true);
				}break;
				}

				bRetVal = TRUE;
			}
			// Release memory
			::GlobalUnlock(hMem);
			::GlobalFree(hMem);
		}
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	return bRetVal;
}

//读出数据，输出成数组格式放到粘贴板
unsigned int CHexEditViewPage::SetPasteView(unsigned char *buffer, unsigned char *srcBuffer, unsigned int nLen)
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
unsigned int CHexEditViewPage::SetPasteData(unsigned char *buffer, unsigned char *srcBuffer, unsigned int len)
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

void CHexEditViewPage::HexToStr(BYTE *pbSrc, int nLen, BYTE *dstbuffer)
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

void CHexEditViewPage::StrToHex(BYTE *pbSrc, int nLen, BYTE *dstbuffer)
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

COleDataSource* CHexEditViewPage::CreateDataSource(unsigned int type)
{
	//先取出数据
	unsigned __int64 nSrcLength = GetCurSelLen();
	BYTE* lpSrcByte = new BYTE[nSrcLength];
	GetCurSelData(lpSrcByte, nSrcLength);

	COleDataSource* pDataSource = NULL;
	HGLOBAL hMem;
	try {
		switch (type)
		{
		case CT_View:
		{
			pDataSource = new COleDataSource;
			// Copy data as CF_TEXT
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
			hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLength + 1);
			if (hMem == NULL)
				AfxThrowMemoryException();
			unsigned char *lpByte = (BYTE*)::GlobalLock(hMem);
			ASSERT(lpByte != NULL);
			nLength = SetPasteView(lpByte, lpSrcByte, nSelLength);
			delete []lpSrcByte;
			lpByte[nLength] = 0;
			::GlobalUnlock(hMem);
			pDataSource->CacheGlobalData(CF_TEXT, hMem);
		}break;
		case CT_BinaryString:
		{
			pDataSource = new COleDataSource;
			// Copy data as CF_TEXT
			unsigned int nSelLength = nSrcLength;
			unsigned int nLength = nSelLength * 5;
			hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLength + 1);
			if (hMem == NULL)
				AfxThrowMemoryException();
			unsigned char *lpByte = (BYTE*)::GlobalLock(hMem);
			ASSERT(lpByte != NULL);
			SetPasteData(lpByte, lpSrcByte, nSelLength);
			delete[]lpSrcByte;
			lpByte[nLength] = 0;
			::GlobalUnlock(hMem);
			pDataSource->CacheGlobalData(CF_TEXT, hMem);

		}break;
		case CT_HEXString:
		{
			pDataSource = new COleDataSource;
			// Copy data as CF_TEXT
			unsigned int nSelLength = nSrcLength;
			unsigned int nLength = nSelLength * 2;
			hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLength + 1);
			if (hMem == NULL)
				AfxThrowMemoryException();
			unsigned char *lpByte = (BYTE*)::GlobalLock(hMem);
			ASSERT(lpByte != NULL);
			HexToStr(lpSrcByte, nSelLength, lpByte);
			delete[]lpSrcByte;
			lpByte[nLength] = 0;
			::GlobalUnlock(hMem);
			pDataSource->CacheGlobalData(CF_TEXT, hMem);
		}break;
		case CT_Binary:
		default:
		{
			pDataSource = new COleDataSource;
			// Copy data as CF_TEXT
			unsigned int nSelLength = nSrcLength;
			hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nSelLength + 1);
			if (hMem == NULL)
				AfxThrowMemoryException();
			unsigned char * lpByte = (BYTE*)::GlobalLock(hMem);
			ASSERT(lpByte != NULL);
			memcpy(lpByte, lpSrcByte, nSelLength);
			delete []lpSrcByte;
			lpByte[nSelLength] = 0;
			::GlobalUnlock(hMem);
			pDataSource->CacheGlobalData(CF_TEXT, hMem);

			// Since binary data may contain '\0's, also create native format
			// that contains only the length of the data
			hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(int));
			if (hMem == NULL)
				AfxThrowMemoryException();
			lpByte = (BYTE*)::GlobalLock(hMem);
			ASSERT(lpByte != NULL);
			*((int*)lpByte) = nSelLength;
			::GlobalUnlock(hMem);
			pDataSource->CacheGlobalData((CLIPFORMAT)CF_TEXT, hMem);
		}break;
		}

	}
	catch (CException* e)
	{
		delete pDataSource;
		pDataSource = NULL;
		e->ReportError();
		e->Delete();
	}
	return pDataSource;
}

void CHexEditViewPage::OnEditDetail()
{
	// TODO:  在此添加命令处理程序代码
	if (NULL == m_pTipDlg)
	{
		// 创建非模态对话框实例   
		m_pTipDlg = new CHexEditViewPage();
		m_pTipDlg->Create(IDD_DIALOG1, this);
	}
	unsigned __int64 len = GetDataLen();
	if (len)
	{
		BYTE *buffer = new BYTE[len];
		GetData(buffer, len);
		m_pTipDlg->SetData(buffer, len);
	}
	m_pTipDlg->ShowWindow(SW_SHOW);
}
