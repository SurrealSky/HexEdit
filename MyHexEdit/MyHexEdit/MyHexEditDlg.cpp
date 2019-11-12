
// MyHexEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyHexEdit.h"
#include "MyHexEditDlg.h"
#include "afxdialogex.h"
#include <ShellScalingApi.h>


#ifdef _DEBUG
#pragma comment(lib,"../Debug/HexView.lib")
#else
#pragma comment(lib,"../Release/HexView.lib")
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyHexEditDlg �Ի���



CMyHexEditDlg::CMyHexEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyHexEditDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isInit = false;
}

void CMyHexEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyHexEditDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyHexEditDlg::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMyHexEditDlg ��Ϣ�������

BOOL CMyHexEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(_T("oleʧ��"));
		return FALSE;
	}

	BYTE buffer[] = "hello worldhello worldhello";

	mHexControl.CreateHexView(AfxGetInstanceHandle(), m_hWnd);
	mHexControl.SetPosition(0, 90, 500, 50);
	mHexControl.SetData(buffer, sizeof(buffer));
	mHexControl2.CreateHexView(AfxGetInstanceHandle(), m_hWnd);
	mHexControl2.SetPosition(0, 215, 650, 50);
	mHexControl2.SetData(buffer, sizeof(buffer));
	mHexControl2.SetBkFillColor(0, 2, 0, 255, 0);
	mHexControl2.ClearFillColor();
	mHexControl2.SetBkFillColor(3, 2, 128, 0, 128);
	mHexControl2.SetEditMode(READONLY);
	
	mHexControl2.SetFont("����",10,false);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyHexEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyHexEditDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyHexEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyHexEditDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMyHexEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);	
}


void CMyHexEditDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}
