// Target.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <string> 

void ostr(const wchar_t *strOutputString, ...)  
{  
	va_list vlArgs = NULL;  
	va_start(vlArgs, strOutputString);  
	size_t nLen = _vscwprintf(strOutputString, vlArgs) + 1;  
	wchar_t *strBuffer = new wchar_t[nLen];  
	_vsnwprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);  
	va_end(vlArgs);  
	OutputDebugStringW(strBuffer);  
	delete [] strBuffer;  
}  

void ostr(const char *strOutputString, ...)  
{  
	va_list vlArgs = NULL;  
	va_start(vlArgs, strOutputString);  
	size_t nLen = _vscprintf(strOutputString, vlArgs) + 1;  
	char *strBuffer = new char[nLen];  
	_vsnprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);  
	va_end(vlArgs);  
	OutputDebugStringA(strBuffer);  
	delete [] strBuffer;  
}  

int _tmain(int argc, _TCHAR* argv[])
{
	std::string value;
	while (true)
	{
		HANDLE currentThread = GetCurrentThread();
		std::cout << "Target process id: ";
		std::cout << GetProcessIdOfThread(currentThread);
		std::cout << "\n";
		CloseHandle(currentThread);

		std::cout << "Target: press <enter> to Beep (Ctrl-C to exit): ";
		std::getline(std::cin, value);
		std::cout << "Target: calling Beep...\n";
		Beep(500, 500);
		std::cout << "Target: Beep complete.\n";
		ostr("hahahahhahahahahah\n");
		std::cout << "hahahha" << std::endl;
	}
	return 0;
}

