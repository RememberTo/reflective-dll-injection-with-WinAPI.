#pragma once
#include "BaseWindow.h"

class Window :
	public BaseWindow
{
	// Указатель на метод
	typedef LRESULT(Window::* EventHandle)(LPARAM, WPARAM);

	// Карта сообщений (Односвязный список)
	class MessageMap {
		struct Handler { UINT msg; Window* wnd; EventHandle eHandle; Handler* next; };
	private:
		Handler* head = nullptr, * tail = nullptr;
	public:
		void append(UINT msg, Window* wnd, EventHandle eHandle);
		LRESULT exec(UINT msg, LPARAM lParam, WPARAM wParam);
		~MessageMap();
	};



	MessageMap messageMap;

	bool createWndClass(HINSTANCE hInstance);
public:
	Window(HINSTANCE		hInstance,
		std::wstring		className,
		std::wstring		name,
		int					x,
		int					y,
		int					w,
		int					h,
		BaseWindow* parent = nullptr,
		DWORD				style = WS_OVERLAPPEDWINDOW,
		DWORD				styleEx = NULL);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	template<typename T> void addEventListener(UINT msg, LRESULT(T::* eHandle) (LPARAM, WPARAM), T* context);

	virtual ~Window();
};

template<typename T>
void Window::addEventListener(UINT msg, LRESULT(T::* eHandle)(LPARAM, WPARAM), T* context)
{
	messageMap.append(msg, context, (EventHandle)eHandle);
}
