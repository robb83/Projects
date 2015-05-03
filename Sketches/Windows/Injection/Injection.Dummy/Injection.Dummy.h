#pragma once

#ifdef INJECTIONDUMMY_EXPORTS
#define INJECTIONDUMMY_API __declspec(dllexport)
#else
#define INJECTIONDUMMY_API __declspec(dllimport)
#endif

namespace Injection
{
	class Dummy
	{
	public:
		static INJECTIONDUMMY_API void StartCalc();
	};
}