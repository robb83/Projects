#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	_TCHAR buff[256];
	DWORD readed = 0;

	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	
	if (hInput == NULL)
	{
		return 0;
	}

	while (ReadConsole(hInput, buff, sizeof(buff), &readed, NULL))
	{
		if (_tcsncmp(buff, _T("exit"), 4) == 0)
		{
			break;
		}
	}

	CloseHandle(hInput);

	return 0;
}
