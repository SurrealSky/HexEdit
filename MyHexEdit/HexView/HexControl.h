#pragma once
#include<Windows.h>



class HexControl
{
public:
	HexControl(HWND hParent);
	~HexControl();
public:
	void CreateHexView(HINSTANCE);
	void SetPosition(int x, int y, int width, int height);
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

