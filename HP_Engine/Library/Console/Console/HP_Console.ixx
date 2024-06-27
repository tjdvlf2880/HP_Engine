module;
#include <Windows.h>
#include <tchar.h>
#include <iostream>
export module HP_Console;

import std;

export namespace HP
{
	class Console
	{
	private:
		Console();
		static HANDLE hConsole;
		static HANDLE GetConsole();
		static std::mutex mtx;

	public:
		~Console();
		static void Write(LPCTSTR message);
	};
}