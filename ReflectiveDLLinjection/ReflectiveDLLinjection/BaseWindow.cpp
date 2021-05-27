#include "BaseWindow.h"

BaseWindow::BaseWindow(HINSTANCE		hInstance,
	std::wstring		className,
	std::wstring		name,
	int				x,
	int				y,
	int				w,
	int				h,
	INT				id,
	BaseWindow* parent,
	DWORD			style,
	DWORD			styleEx,
	LPVOID		    param)
{
	this->name = name;
	this->className = className;
	this->hInstance = hInstance;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->id = id;
	this->parent = parent;
	this->style = style;
	this->styleEx = styleEx;
	this->param = param;
}

void BaseWindow::create()
{
	this->hWnd = CreateWindowExW(styleEx, className.c_str(), name.c_str(), style, x, y, w, h, parent == nullptr ? NULL : parent->hWnd, (HMENU)id, hInstance, param);

}

RECT BaseWindow::getRect()
{
	RECT r;
	GetWindowRect(this->hWnd, &r);
	return r;
}

RECT BaseWindow::getClientRect()
{
	RECT r;
	GetClientRect(this->hWnd, &r);
	return r;
}

void BaseWindow::setText(std::wstring text)
{
	SetWindowTextW(this->hWnd, text.c_str());
	this->name = text.c_str();
}

wchar_t* BaseWindow::getText()
{
	const size_t size = 100;
	wchar_t* buff = new wchar_t[30];
	GetWindowTextW(this->hWnd, buff, size);
	return buff;
}

void BaseWindow::setBgColor(COLORREF color)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)SetClassLongPtr(this->hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
	DeleteObject(hOldBrush);
	InvalidateRect(hWnd, NULL, 1);
}

void BaseWindow::setPos(int x, int y)
{
	this->x = x;
	this->y = y;
	SetWindowPos(this->hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void BaseWindow::setSize(int w, int h)
{
	this->w = w;
	this->h = h;
	SetWindowPos(this->hWnd, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
}

void BaseWindow::show(bool flag)
{
	ShowWindow(this->hWnd, flag ? SW_SHOWNORMAL : SW_MINIMIZE);
}

BaseWindow::~BaseWindow()
{
}