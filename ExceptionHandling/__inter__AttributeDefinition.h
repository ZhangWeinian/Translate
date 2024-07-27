#pragma once

#include "pch.h"

#include <version>



#ifndef _STD
	#define _STD ::std::
#endif	// !_STD

#ifndef _CSTD
	#define _CSTD ::
#endif	// !_CSTD

#ifndef _SPDLOG
	#define _SPDLOG ::spdlog::
#endif	// !_SPDLOG



#ifdef EXCEPTIONHANDLING_EXPORTS
	#define EXCEPTIONHANDLING_API __declspec(dllexport)
#else
	#define EXCEPTIONHANDLING_API __declspec(dllimport)
#endif	// EXCEPTIONHANDLING_EXPORTS
