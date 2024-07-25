#pragma once

#include "pch.h"

#include <string>
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
	#define BAIDUTRANSLATE_API __declspec(dllexport)
#else
	#define BAIDUTRANSLATE_API __declspec(dllimport)
#endif	// BAIDUTRANSLATE_EXPORTS



using TranslateInfoType = struct
{
	_STD string source;
	_STD string from;
	_STD string to;
	_STD string appid;
	_STD string appkey;
};
