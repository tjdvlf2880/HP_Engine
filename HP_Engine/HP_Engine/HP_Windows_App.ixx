module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
export module HP_Windows_App;

import std;

export namespace HP
{
	LRESULT MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	class Windows_App
	{
		LPCTSTR name;
		HINSTANCE hInstance;
		std::unordered_map<LPCTSTR, HWND> window; //공유 자원.
		std::mutex mtx;
		void Register(LPCTSTR classname);
		void UnRegister(LPCTSTR name);
		HWND Create(LPCTSTR name);
		void CheckErr();
	public:
		~Windows_App();
		Windows_App(HINSTANCE hInstance);
		void Run(LPCTSTR name, void(work)());
	};
}


