// BeepHookDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include <easyhook.h>
#include <string>
#include <iostream>
#include <Windows.h>

#include <easyhook.h>
#pragma comment(lib,"easyhook32.lib")

//DWORD gFreqOffset = 0;
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
int WriteMemoryFile (LPCWSTR str) {

    HANDLE hFile_Map = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4 * 1024, TEXT("MMFSharedData"));  
  
    if (hFile_Map == NULL)  
    {  
        printf("Failed to create file mapping!\n");  
        getchar();  
  
        return;  
    }  
    else if (GetLastError() == ERROR_ALREADY_EXISTS)  
    {  
        printf("Mapping already exists - not created.\n");  
        getchar();  
    }  
  
    // File mapping created successfully.  
  
    // Map a view of the file into the address space.  
    PVOID pView = MapViewOfFile(hFile_Map, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);  
    if (NULL == pView)  
    {  
        printf("Failed to map view of file.\n");  
        getchar();  
  
        return;  
    }  
  
    while (true)  
    {  
        getchar();  
        system("cls");  
        printf("\n\t1.写入 mapping file 内容.");  
        printf("\n\t2.读取 mapping file 内容.");  
        printf("\n\t3.退出程序");  
        printf("\n\n请输入你的选择序号：");  
        scanf("%d", &choice);  
  
        if (choice == 3)  
            break;  
  
        switch (choice)  
        {  
        case 1:  
            printf("输入要写入的内容：");  
            scanf("%s", pView);  
            break;  
        case 2:  
            printf("显示已写入的内容：");  
            printf("%s\n", pView);  
            break;  
        default:  
            printf("你的选择序号有误。\n");  
            break;  
        }  
        getchar();  
    }  
      
    UnmapViewOfFile(pView);  
    CloseHandle(hFile_Map);      
}

 BOOL myExtTextOutW( __in HDC hdc, __in int x, __in int y, 
                    __in UINT options, __in_opt CONST RECT * lprect, 
                    __in_ecount_opt(c) LPCWSTR lpString, 
                    __in UINT c, __in_ecount_opt(c) CONST INT * lpDx)
 {
    
 }
#if 0
BOOL WINAPI myTextOutW(HDC dc, int x,int y, LPCWSTR str,int c)
{
	ostr("Hook begin!");
	ostr(str);
	ostr("Hook end!");
	return TextOutW (dc,x,y,str,c);
}

BOOL WINAPI myBeepHook(DWORD dwFreq, DWORD dwDuration)
{
	std::cout << "\n    BeepHook: ****All your beeps belong to us!\n\n";
	return Beep(dwFreq + gFreqOffset, dwDuration);
}
#endif

// EasyHook will be looking for this export to support DLL injection. If not found then 
// DLL injection will fail.
extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo);

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
	std::cout << "\n\nNativeInjectionEntryPointt(REMOTE_ENTRY_INFO* inRemoteInfo)\n\n" <<
		"IIIII           jjj               tt                dd !!! \n"
		" III  nn nnn          eee    cccc tt      eee       dd !!! \n"
		" III  nnn  nn   jjj ee   e cc     tttt  ee   e  dddddd !!! \n"
		" III  nn   nn   jjj eeeee  cc     tt    eeeee  dd   dd     \n"
		"IIIII nn   nn   jjj  eeeee  ccccc  tttt  eeeee  dddddd !!! \n"
		"              jjjj                                         \n\n";

	std::cout << "NativeInjectionEntryPoint: Injected by process Id: " << inRemoteInfo->HostPID << "\n";
	std::cout << "NativeInjectionEntryPoint: Passed in data size: " << inRemoteInfo->UserDataSize << "\n";
#if 0

	if (inRemoteInfo->UserDataSize == sizeof(DWORD))
	{
		gFreqOffset = *reinterpret_cast<DWORD *>(inRemoteInfo->UserData);
		std::cout << "NativeInjectionEntryPoint: Adjusting Beep frequency by: " << gFreqOffset << "\n";
	}
#endif

	// Perform hooking
	HOOK_TRACE_INFO hHook = { NULL }; // keep track of our hook

	std::cout << "\n";
	std::cout << "NativeInjectionEntryPoint: Win32 TextOut found at address: " 
		<< GetProcAddress(GetModuleHandle(TEXT("GDI32")), "TextOutW") << "\n";

	// Install the hook
	NTSTATUS result = LhInstallHook(
		GetProcAddress(GetModuleHandle(TEXT("GDI32")), "TextOutW"),
		myTextOutW,
		NULL,
		&hHook);
	if (FAILED(result))
	{
		std::wstring s(RtlGetLastErrorString());
		std::wcout << "NativeInjectionEntryPoint: Failed to install hook: " << s << "\n";
	}
	else 
	{
		std::cout << "NativeInjectionEntryPoint: Hook 'myTextOut installed successfully.\n";
	}

	// If the threadId in the ACL is set to 0,
	// then internally EasyHook uses GetCurrentThreadId()
	ULONG ACLEntries[1] = { 0 };

	// Disable the hook for the provided threadIds, enable for all others
	LhSetExclusiveACL(ACLEntries, 1, &hHook);

	return;
}

