#pragma once
#include<Windows.h>



class HexControl
{
public:
	HexControl();
	~HexControl();
public:
	void CreateHexView(HINSTANCE, HWND);
	void SetPosition(int sysDPI,int x, int y, int width, int height);
	void SetData(BYTE *data, unsigned __int64 len);
	unsigned __int64 GetDataLen();
	void GetData(BYTE *data, unsigned __int64 len);
	void SetDataPtr(DWORD_PTR ptr);
	DWORD_PTR GetDataPtr();
private:
	HWND m_hWndParent;
	HWND m_hWndHexView;
	DWORD_PTR ptr;
};

