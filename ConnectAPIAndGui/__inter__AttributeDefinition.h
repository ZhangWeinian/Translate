#pragma once

#include <version>



#ifdef CONNECTAPIANDGUI_EXPORTS
	#define CONNECTAPIANDGUI_API __declspec(dllexport)
#else
	#define CONNECTAPIANDGUI_API __declspec(dllimport)
#endif	// CONNECTAPIANDGUI_EXPORTS

#ifndef _STD
	#define _STD ::std::
#endif	// !_STD
