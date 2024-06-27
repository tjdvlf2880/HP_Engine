module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <crtdbg.h>
#pragma comment(lib, "HP_Console.lib")
export module HP_Core;
import HP_Windows_App;
import HP_Console;
import HP_Input;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
	HP::Windows_App instance(hInstance);
	HANDLE semaphore = CreateSemaphoreEx(NULL, 0, 2, NULL, NULL, SEMAPHORE_ALL_ACCESS);
	HMODULE hModule = LoadLibraryEx(_T("lib/HP_Input.dll"),NULL, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);
	if (hModule != NULL) 
	{
		typedef void (*test)();
		int* pX = (int*)GetProcAddress(hModule, "x");
		test f = (test)GetProcAddress(hModule, "myFunction");
		f();
		FreeLibrary(hModule);
	}

 	HP::Console::Write(_T("start!\n"));
	/*
		멀티 스레드로 윈도우를 생성하고 작업을 지정
	*/
	LPCTSTR name = _T("HP_App");
	std::thread([&]()
		{
			instance.Run(name, []() {HP::Console::Write(_T("sub\n")); });
			ReleaseSemaphore(semaphore, 1, 0);
		}).detach();

	instance.Run(_T("HP_App2"), []() {HP::Console::Write(_T("Main\n")); });
	ReleaseSemaphore(semaphore, 1, 0);
	for (int i = 0; i < 2; i++)
	{
		WaitForSingleObject(semaphore, INFINITE);
	}
	CloseHandle(semaphore);
	return 0;
}