#include "stdafx.h"

// reference
// https://msdn.microsoft.com/en-us/library/ms235636.aspx

namespace Injection
{
	void CreateWindowsProcess(_TCHAR* executableFilePath) {
		STARTUPINFOW startupInfo;
		ZeroMemory(&startupInfo, sizeof(STARTUPINFOW));
		startupInfo.cb = sizeof STARTUPINFOW;

		PROCESS_INFORMATION processInfo;
		ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

		CreateProcess(executableFilePath, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo);
	}

	void Dummy::StartCalc()
	{
		CreateWindowsProcess(_T("c:\\Windows\\System32\\calc.exe"));
	}
}