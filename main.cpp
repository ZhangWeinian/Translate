#pragma once

#include <version>


#if defined(_HAS_CXX20)
	#include "./TranslateAPI/TranslateAPI.h"
#endif	// defined(_HAS_CXX20)


#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	_STD string	   appid {};
	_STD		   cout << "请输入 appid: 123";
	_STD		   cin >> appid;

	_STD string	   appkey {};
	_STD		   cout << "请输入 appkey: 311";
	_STD		   cin >> appkey;

	BaiduTranslate baiduTranslate(appid, appkey);

	_STD string	   source {};
	_STD string	   from { "auto" };
	_STD string	   to {};

	while (true)
	{
		_STD cout << "请输入要翻译的内容: ";
		_STD cin >> source;

		_STD cout << "请输入源语言: ";
		_STD cin >> from;

		_STD cout << "请输入目标语言: ";
		_STD cin >> to;

		_STD cout << "翻译结果: " << baiduTranslate.Translate(source, from, to) << _STD endl;

		_STD cout << _STD endl << _STD endl;
	}

	return 0;
}
