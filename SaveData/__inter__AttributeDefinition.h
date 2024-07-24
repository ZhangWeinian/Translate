#pragma once

#include "pch.h"

#include <limits>
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
	#define SAVEDATA_API extern __declspec(dllexport)
#else
	#define SAVEDATA_API extern __declspec(dllimport)
#endif	// SAVEDATA_EXPORTS


using info = _STD pair<_STD string, _STD string>;
