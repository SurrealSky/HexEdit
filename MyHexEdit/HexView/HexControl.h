#pragma once
#include<Windows.h>

#define CUR_DEVICES_DPI96	96
#define CUR_DEVICES_DPI120	120

class HexControl
{
public:
	HexControl();
	~HexControl();
public:
	void CreateHexView(HINSTANCE, HWND);
	void SetPosition(int x, int y, int width, int height, int sysDPI= CUR_DEVICES_DPI96);
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

