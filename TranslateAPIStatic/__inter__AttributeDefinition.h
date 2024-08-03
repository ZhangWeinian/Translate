#pragma once

#include <string>
#include <version>



#ifndef _STD
	#define _STD ::std::
#endif	// !_STD

#ifndef _CSTD
	#define _CSTD ::
#endif	// !_CSTD

#ifndef _CHRONO
	#define _CHRONO ::std::chrono::
#endif	// !_CHRONO

#ifndef _NODISCARD
	#define _NODISCARD [[nodiscard]]
#endif	// !_NODISCARD


using TranslateInfoType = struct
{
	_STD string source;
	_STD string from;
	_STD string to;
	_STD string appid;
	_STD string appkey;
};
