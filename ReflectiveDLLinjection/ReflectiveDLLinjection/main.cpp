#include "Window.h"
#include "Controls.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "LoadLibraryR.h"

#pragma comment(lib,"Advapi32.lib")

#pragma warning(disable: 4700)
#pragma warning(disable: 4996)
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") //Стилизация под Windows 10


#define ID_BUTTON 1

class App {
public:
	void Run() {
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
};

class MyWindow :public Window {
	Static* StaticPID;
	Static* StaticPath;
	EditNum* Epid;
	EditPath* Epath;
	Button* RunButton;
	RECT lrect;
public:
	MyWindow(HINSTANCE hInstance) : Window(hInstance, L"myWndClass", L"Injection", 
		700, 300, 400, 320,
		NULL, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUP) {
		this->addEventListener<MyWindow>(WM_CREATE, &MyWindow::onCreate, this);
		this->addEventListener<MyWindow>(WM_DESTROY, &MyWindow::onDestroy, this);
		this->addEventListener<MyWindow>(WM_CTLCOLORSTATIC, &MyWindow::onBgColorStatic, this);
		this->addEventListener<MyWindow>(WM_CTLCOLOREDIT, &MyWindow::onBgColorEdit, this);
		this->addEventListener<MyWindow>(WM_COMMAND, &MyWindow::onClickBtn, this);
		/////////////////////////////////////////////////////////////////
		this->create();

		this->setBgColor(RGB(0, 63, 77));
		this->lrect = this->getClientRect();

		this->StaticPID = new Static(L"Enter PID", lrect.left + 30, 70, 100, 25, this, NULL);
		this->StaticPath = new Static(L"Enter Path", 200, 70, lrect.right - 255, 25, this, NULL);
		this->Epid = new EditNum(L"", lrect.left + 30, 90, 100, 25, this, NULL);
		this->Epath = new EditPath(L"", lrect.right - 220, 90, 200, 25, this, NULL);
		this->RunButton = new Button(L"Run", (lrect.right / 2) - 75, lrect.bottom - 100, 150, 35, this, ID_BUTTON);

		this->show();
	}
	LRESULT onCreate(LPARAM lParam, WPARAM wParam) {
		return 0;
	}
	LRESULT onDestroy(LPARAM lParam, WPARAM wParam) {
		PostQuitMessage(0);
		return 0;
	}

	LRESULT onBgColorStatic(LPARAM lParam, WPARAM wParam) {
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, 0);
		SetTextColor(hdc, RGB(177, 244, 111));
		return (int)GetStockObject(NULL_BRUSH);
	}
	LRESULT onBgColorEdit(LPARAM lParam, WPARAM wParam) {
		HDC hdc = (HDC)wParam;
		SetBkColor(hdc, RGB(215, 215, 215));
		SetTextColor(hdc, RGB(0, 12, 128));
		return (DWORD)GetSysColorBrush(COLOR_WINDOW);
	}


	LRESULT onClickBtn(LPARAM lParam, WPARAM wParam) {
		wchar_t* pid;
		char* bufPId;
		DWORD PID;
		wchar_t* path;
		char* PATH;
		if (wParam == ID_BUTTON) {
			pid = Epid->getText();
			path = Epath->getText();
			size_t size = wcslen(pid) + 1;
			size_t sizePa = wcslen(path) + 1;
			if (sizePa == 1)
			{
				MessageBox(hWnd, L"Please fill in the fields", NULL, MB_OK);
				return 0;
			}
			bufPId = new char[size];
			PATH = new char[sizePa];
			wcstombs(bufPId, pid, size);
			wcstombs(PATH, path, size);
			PID = atoi(bufPId);

			HANDLE hFile = NULL;
			HANDLE hModule = NULL;
			HANDLE hProcess = NULL;
			HANDLE hToken = NULL;
			LPVOID lpBuffer = NULL;
			DWORD dwLength = 0;
			DWORD dwBytesRead = 0;
			DWORD dwProcessId = 0;
			TOKEN_PRIVILEGES priv = { 0 };


			do
			{
					//dwProcessId = PID;

				const char* cpDllFile = "../Debug/Dll1.dll";

				hFile = CreateFileA(cpDllFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE)
				{
					MessageBox(hWnd, L"Failed to open the DLL file", NULL, MB_OK);
					break;
				}
				dwLength = GetFileSize(hFile, NULL);
				if (dwLength == INVALID_FILE_SIZE || dwLength == 0)
				{
					MessageBox(hWnd, L"Failed to get the DLL file size", NULL, MB_OK);
					break;
				}
				lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwLength);
				if (!lpBuffer)
				{
					MessageBox(hWnd, L"Failed to get the DLL file size", NULL, MB_OK);
					break;
				}
				if (ReadFile(hFile, lpBuffer, dwLength, &dwBytesRead, NULL) == FALSE)
				{
					MessageBox(hWnd, L"Failed to alloc a buffer!", NULL, MB_OK);
					break;
				}
				if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
				{
					priv.PrivilegeCount = 1;
					priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

					if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
						AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

					CloseHandle(hToken);
				}

				hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);
				if (!hProcess) {
					MessageBox(hWnd, L"Failed to open the target process", NULL, MB_OK);
					break;
				}
				hModule = LoadRemoteLibraryR(hProcess, lpBuffer, dwLength, NULL);
				if (!hModule) {
					MessageBox(hWnd, L"Failed to inject the DLL", NULL, MB_OK);
					break;
				}

				MessageBox(hWnd, L"Injected was successful", L"Info", MB_OK | MB_ICONASTERISK);
				CreateFileA("D:\\file.txt", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				//WaitForSingleObject(hModule, -1);

			} while (0);

			if (lpBuffer)
				HeapFree(GetProcessHeap(), 0, lpBuffer);

			if (hProcess)
				CloseHandle(hProcess);
			return 0;

		}

		return 0;
	}
};


int CALLBACK WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int ncmdShow) {
	MyWindow window(hInstance);
	App app;
	app.Run();
	return 0;
};