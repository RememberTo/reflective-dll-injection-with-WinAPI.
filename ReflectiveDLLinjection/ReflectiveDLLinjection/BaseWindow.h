#pragma once
#include <windows.h>
#include <string>

class BaseWindow
{
protected:
	HWND hWnd;
	std::wstring className;
	std::wstring name;
	HINSTANCE hInstance;
	int x;
	int y;
	int w;
	int h;
	INT id;
	BaseWindow* parent;
	DWORD style;
	DWORD styleEx;
	LPVOID param;
public:
	BaseWindow(HINSTANCE		hInstance,
		std::wstring		className,
		std::wstring		name,
		int				x,
		int			    y,
		int				w,
		int				h,
		INT				id = 0,
		BaseWindow* parent = nullptr,
		DWORD			style = WS_OVERLAPPEDWINDOW,
		DWORD			styleEx = NULL,
		LPVOID			param = NULL);
	void create();
	RECT getRect();
	RECT getClientRect();
	void setText(std::wstring text);
	wchar_t* getText();
	void setBgColor(COLORREF color);
	void setPos(int x, int y);
	void setSize(int w, int h);
	void show(bool flag = true);
	virtual ~BaseWindow();
};
