module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
module HP_Windows_App;

HWND HP::Windows_App::Create(LPCTSTR name)
{
	if (window.find(name) == window.end())
	{
		HWND hwnd = CreateWindowEx(NULL, this->name, name, WS_SIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, this->hInstance, NULL);
		window[name] = hwnd;
		ShowWindow(hwnd, SW_SHOWNORMAL);
		UpdateWindow(hwnd);
	}
	return window[name];
}

HP::Windows_App::~Windows_App()
{
	for (std::pair<LPCTSTR, HWND> elem : window)
	{
		DestroyWindow(elem.second);
	}
	window.clear();
	UnRegister(name);
}

void HP::Windows_App::Register(LPCTSTR classname)
{
	WNDCLASSEX prop;
	prop.cbSize = sizeof(WNDCLASSEX);
	prop.style = CS_HREDRAW | CS_VREDRAW;
	prop.lpfnWndProc = MessageProc,
	prop.cbClsExtra = 0,
	prop.cbWndExtra = 0,
	prop.hInstance = this->hInstance;
	prop.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	prop.hCursor = LoadCursor(NULL, IDC_ARROW);
	prop.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	prop.lpszMenuName = NULL;
	prop.lpszClassName = classname;
	prop.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&prop))
	{
		CheckErr();
	}
}

void HP::Windows_App::UnRegister(LPCTSTR name)
{
	if (!UnregisterClass(name, hInstance))
	{
		CheckErr();
	}
}

void HP::Windows_App::CheckErr()
{
	DWORD error = GetLastError();
	LPTSTR errorMsg = nullptr;
	// HRESULT에서 에러 메시지를 가져옴
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&errorMsg, 0, NULL);
	MessageBox(NULL, errorMsg, TEXT("오류"), 0);
	LocalFree(errorMsg);
}

HP::Windows_App::Windows_App(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
	name = _T("HP_APP");
	Register(name);
}

void HP::Windows_App::Run(LPCTSTR name,void(work)())
{
	HWND hwnd = NULL;
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (window.find(name) == window.end())
		{
			hwnd = Create(name);
		}
		else
		{
			hwnd = window[name];
		}
	}
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);
	MSG msg;
	bool run = true;
	while (run)
	{
		if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (!IsWindow(hwnd))
			{
				run = false;
			}
		}
		else
		{
			work();
		}
	}
	DestroyWindow(hwnd);
	{
		std::lock_guard<std::mutex> lock(mtx);
		window.erase(name);
	}
}

LRESULT HP::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}
