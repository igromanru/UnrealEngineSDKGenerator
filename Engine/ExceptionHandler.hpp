#pragma once

#include <windows.h>

#include "Logger.hpp"

namespace ExceptionHandler
{
	static void* ExceptionHandler;

	void Init();
	void Destroy();
	long VectoredHandler(struct _EXCEPTION_POINTERS *exceptionInfo);
	std::string ExceptionCodeToString(const DWORD& exceptionCode);
	bool IsKnownException(const DWORD& exceptionCode);
}