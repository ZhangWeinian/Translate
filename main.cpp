#pragma once

#include <version>

#if defined(_HAS_CXX20)
	#include "./ConnectAPIAndGui/ConnectAPIAndGui.h"
#endif	// defined(_HAS_CXX20)

#include <iostream>
#include <string>

#ifndef _CONNECTAPIANDGUI
	#define _CONNECTAPIANDGUI ::
#endif	// !_CONNECTAPIANDGUI

int main(int argc, char* argv[])
{
	_STD string appid { "123" };

	_STD string appkey { "abc" };

	auto p = _CONNECTAPIANDGUI BeginBaiduTranslate(appid.c_str(), appkey.c_str());

	_CONNECTAPIANDGUI		   SetAppID(p, appid.c_str(), appkey.c_str());

	_STD string				   src { "hello" };

	_STD string				   from { "en" };

	_STD string				   to { "zh" };

	_STD cout << _CONNECTAPIANDGUI Translate(p, src.c_str(), from.c_str(), to.c_str()) << _STD endl;

	_CONNECTAPIANDGUI																		   EndBaiduTranslate(p);

	return 0;
}
