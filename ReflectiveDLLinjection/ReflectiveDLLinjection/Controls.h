#pragma once
#include "BaseWindow.h"

class Static : public BaseWindow
{
public:
	Static(std::wstring text, int x, int y, int w, int h, BaseWindow* parent, INT id = NULL)
		: BaseWindow(NULL, L"Static", text, x, y, w, h, id, parent, WS_CHILD | WS_VISIBLE | SS_CENTER)
	{
		this->create();
	}
};

class EditNum : public BaseWindow
{
public:
	EditNum(std::wstring text, int x, int y, int w, int h, BaseWindow* parent, INT id = NULL)
		: BaseWindow(NULL, L"Edit", text, x, y, w, h, id, parent, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER)
	{
		this->create();
	}
};
class EditPath : public BaseWindow
{
public:
	EditPath(std::wstring text, int x, int y, int w, int h, BaseWindow* parent, INT id = NULL)
		: BaseWindow(NULL, L"Edit", text, x, y, w, h, id, parent, WS_CHILD | WS_VISIBLE | ES_CENTER | WS_BORDER | ES_AUTOHSCROLL)
	{
		this->create();
	}
};
class Button : public BaseWindow
{
	bool isEnable = true;
public:
	Button(std::wstring text, int x, int y, int w, int h, BaseWindow* parent, INT id)
		: BaseWindow(NULL, L"Button", text, x, y, w, h, id, parent, WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON)
	{
		this->create();
	}

	void Enable() {
		EnableWindow(this->hWnd, isEnable = !isEnable);
	}
};

