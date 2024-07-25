#pragma once

#include "pch.h"

#include <string>
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



#ifdef SAVEDATA_EXPORTS
	#define SAVEDATA_API __declspec(dllexport)
#else
	#define SAVEDATA_API __declspec(dllimport)
#endif	// SAVEDATA_EXPORTS


using AppIDAndKey = _STD pair<_STD string, _STD string>;
