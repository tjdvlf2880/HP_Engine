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
				�ܼ� â�� ������ ���μ����� ���� ����ȴ�.
				�̴� �޸� ������ �߻��� ������ �������� �ݱ� ��ư�� ��Ȱ��ȭ.
				�׷����� Alt F4 �� ��¿ �� ����.
			*/
			HWND consoleWindow = GetConsoleWindow();
			HMENU hmenu = GetSystemMenu(consoleWindow, FALSE);
			DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}
		return hConsole;
	}
}
