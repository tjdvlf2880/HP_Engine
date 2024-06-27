module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
module HP_Console;
import std;

namespace HP
{
	HANDLE Console::hConsole = nullptr;
	std::mutex Console::mtx;

	Console::Console()
	{
		hConsole = nullptr;
	}

	Console::~Console()
	{
		hConsole = nullptr;
		FreeConsole();
	}

	void Console::Write(LPCTSTR message)
	{
#ifdef _DEBUG
		DWORD bytesWritten = 0;
		{
			std::lock_guard<std::mutex> lock(Console::mtx);
			WriteConsole(GetConsole(), message, _tcslen(message), &bytesWritten, NULL);
		}
#endif
	}
	HANDLE Console::GetConsole()
	{
		if (hConsole == nullptr)
		{
			AllocConsole();
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			/*
				콘솔 창을 닫으면 프로세스가 강제 종료된다.
				이는 메모리 누수가 발생할 위험이 있음으로 닫기 버튼을 비활성화.
				그럼에도 Alt F4 는 어쩔 수 없다.
			*/
			HWND consoleWindow = GetConsoleWindow();
			HMENU hmenu = GetSystemMenu(consoleWindow, FALSE);
			DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}
		return hConsole;
	}
}
