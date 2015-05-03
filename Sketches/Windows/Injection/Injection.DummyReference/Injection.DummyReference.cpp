#include <stdio.h>
#include <tchar.h>
#include <Injection.Dummy.h>

int _tmain(int argc, _TCHAR* argv[])
{
	Injection::Dummy::StartCalc();

	return 0;
}

