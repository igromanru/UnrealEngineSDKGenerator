#include "ExceptionHandler.hpp"

namespace ExceptionHandler
{
	void Init()
	{
		ExceptionHandler = AddVectoredExceptionHandler(1, VectoredHandler);
	}

	void Destroy()
	{
		if (ExceptionHandler)
		{
			RemoveVectoredExceptionHandler(ExceptionHandler);
		}
	}

	long VectoredHandler(_EXCEPTION_POINTERS* exceptionInfo)
	{
		if(IsKnownException(exceptionInfo->ExceptionRecord->ExceptionCode))
		{
			const auto moduleAddress = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
			const auto exceptionAddress = reinterpret_cast<uintptr_t>(exceptionInfo->ExceptionRecord->ExceptionAddress);
			Logger::Log("Exception code: %d\nException address: 0x%P\nAddress in the DLL: 0x%P", 
				ExceptionCodeToString(exceptionInfo->ExceptionRecord->ExceptionCode),
				reinterpret_cast<void*>(exceptionAddress),
				reinterpret_cast<void*>(exceptionAddress - moduleAddress));
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	std::string ExceptionCodeToString(const DWORD& exceptionCode)
	{
		switch (exceptionCode) 
		{
			case EXCEPTION_ACCESS_VIOLATION:
				return "EXCEPTION_ACCESS_VIOLATION";
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
			case EXCEPTION_BREAKPOINT:
				return "EXCEPTION_BREAKPOINT";
			case EXCEPTION_DATATYPE_MISALIGNMENT:
				return "EXCEPTION_DATATYPE_MISALIGNMENT";
			case EXCEPTION_FLT_DENORMAL_OPERAND:
				return "EXCEPTION_FLT_DENORMAL_OPERAND";
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
			case EXCEPTION_FLT_INEXACT_RESULT:
				return "EXCEPTION_FLT_INEXACT_RESULT";
			case EXCEPTION_FLT_INVALID_OPERATION:
				return "EXCEPTION_FLT_INVALID_OPERATION";
			case EXCEPTION_FLT_OVERFLOW:
				return "EXCEPTION_FLT_OVERFLOW";
			case EXCEPTION_FLT_STACK_CHECK:
				return "EXCEPTION_FLT_STACK_CHECK";
			case EXCEPTION_FLT_UNDERFLOW:
				return "EXCEPTION_FLT_UNDERFLOW";
			case EXCEPTION_ILLEGAL_INSTRUCTION:
				return "EXCEPTION_ILLEGAL_INSTRUCTION";
			case EXCEPTION_IN_PAGE_ERROR:
				return "EXCEPTION_IN_PAGE_ERROR";
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				return "EXCEPTION_INT_DIVIDE_BY_ZERO";
			case EXCEPTION_INT_OVERFLOW:
				return "EXCEPTION_INT_OVERFLOW";
			case EXCEPTION_INVALID_DISPOSITION:
				return "EXCEPTION_INVALID_DISPOSITION";
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
			case EXCEPTION_PRIV_INSTRUCTION:
				return "EXCEPTION_PRIV_INSTRUCTION";
			case EXCEPTION_SINGLE_STEP:
				return "EXCEPTION_SINGLE_STEP";
			case EXCEPTION_STACK_OVERFLOW:
				return "EXCEPTION_STACK_OVERFLOW";
			default: 
				return "UNKNOWN EXCEPTION";
		}
	}


	bool IsKnownException(const DWORD& exceptionCode)
	{
		switch (exceptionCode) 
		{
			case EXCEPTION_ACCESS_VIOLATION:
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			case EXCEPTION_BREAKPOINT:
			case EXCEPTION_DATATYPE_MISALIGNMENT:
			case EXCEPTION_FLT_DENORMAL_OPERAND:
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			case EXCEPTION_FLT_INEXACT_RESULT:
			case EXCEPTION_FLT_INVALID_OPERATION:
			case EXCEPTION_FLT_OVERFLOW:
			case EXCEPTION_FLT_STACK_CHECK:
			case EXCEPTION_FLT_UNDERFLOW:
			case EXCEPTION_ILLEGAL_INSTRUCTION:
			case EXCEPTION_IN_PAGE_ERROR:
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
			case EXCEPTION_INT_OVERFLOW:
			case EXCEPTION_INVALID_DISPOSITION:
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			case EXCEPTION_PRIV_INSTRUCTION:
			case EXCEPTION_SINGLE_STEP:
			case EXCEPTION_STACK_OVERFLOW:
				return true;
			default: 
				return false;
		}
	}
}
