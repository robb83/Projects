#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

// reference
// Gray Hat Python: Python Programming for Hackers and Reverse Engineers
// https://aljensencprogramming.wordpress.com/2015/04/21/review-of-console-io-with-win32-functions/

int readPID() 
{
	int result = 0;
	_TCHAR buff[255];
	DWORD readed = 0;

	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	if (hInput == NULL)
	{
		return 0;
	}

	if (ReadConsole(hInput, buff, sizeof(buff), &readed, NULL))
	{
		result = _tstoi(buff);
	}

	CloseHandle(hInput);

	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR dllPath[MAX_PATH];

	DWORD length = GetCurrentDirectory(MAX_PATH, dllPath);
	dllPath[length++] = '\\';
	dllPath[length++] = '\0';
	_tcscat_s(dllPath, MAX_PATH - length, _T("Injection.Dummy.dll"));

	printf("PID: ");
	int processID = readPID();

	if (processID == 0)
	{
		return 0;
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

	if (!hProcess)
	{
		return 0;
	}

	LPVOID memoryAddress = VirtualAllocEx(hProcess, 0, sizeof(dllPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (!memoryAddress)
	{
		return 0;
	}

	SIZE_T writen = 0;
	if (WriteProcessMemory(hProcess, memoryAddress, dllPath, sizeof(dllPath), &writen) == FALSE)
	{
		return 0;
	}

	HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
	FARPROC loadLibraryW = GetProcAddress(kernel32, "LoadLibraryW");

	DWORD threadID = 0;
	HANDLE hTread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryW, memoryAddress, 0, &threadID);
	if (!hTread)
	{
		// check same architecture type
		DWORD errorCode = GetLastError();
		return 0;
	}

	return 0;
}