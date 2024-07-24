#pragma once

#include "pch.h"

#include <format>
#include <limits>
#include <utility>
#include <version>



#ifndef _STD
	#define _STD ::std::
#endif	// !_STD

#ifndef _CSTD
	#define _CSTD ::
#endif	// !_CSTD

#ifndef _NODISCARD
	#define _NODISCARD [[nodiscard]]
#endif	// !_NODISCARD



#ifdef BAIDUTRANSLATE_EXPORTS
	#define BAIDUTRANSLATE_API extern __declspec(dllexport)
#else
	#define BAIDUTRANSLATE_API extern __declspec(dllimport)
#endif	// BAIDUTRANSLATE_EXPORTS
