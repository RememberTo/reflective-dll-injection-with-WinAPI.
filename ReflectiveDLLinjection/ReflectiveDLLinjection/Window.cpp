#include "Window.h"
void Window::MessageMap::append(UINT msg, Window* wnd, EventHandle eHandle)
{
	Handler* h = new Handler();
	h->eHandle = eHandle;
	h->msg = msg;
	h->next = nullptr;
	h->wnd = wnd;
	tail = head == nullptr ? head = h : tail->next = h;
}

LRESULT Window::MessageMap::exec(UINT msg, LPARAM lParam, WPARAM wParam)
{
	for (Handler* h = head; h != nullptr; h = h->next) {
		if (h->msg == msg) return (h->wnd->*(h->eHandle))(lParam, wParam);
	}
	return -1;
}

Window::MessageMap::~MessageMap()
{
	for (Handler* h = head; h != nullptr;) {
		Handler* tmp = h;
		h = h->next;
		delete tmp;
	}
}

bool Window::createWndClass(HINSTANCE hInstance) {
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpszClassName = this->className.c_str();
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassExW(&wndClass);
}

Window::Window(HINSTANCE		hInstance,
	std::wstring				className,
	std::wstring				name,
	int							x,
	int							y,
	int							w,
	int							h,
	BaseWindow* parent,
	DWORD						style,
	DWORD						styleEx) :BaseWindow(hInstance, className, name, x, y, w, h, 0, parent, style, styleEx, this)
{
	createWndClass(hInstance);
}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* wnd = nullptr;
	if (msg == WM_NCCREATE) {
		wnd = (Window*)(LPCREATESTRUCTW(lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(wnd));
		wnd->hWnd = hWnd;
	}
	wnd = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (wnd) {
		LRESULT lResult;
		if (-1 != (lResult = wnd->messageMap.exec(msg, lParam, wParam)))
			return lResult;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::~Window() {}