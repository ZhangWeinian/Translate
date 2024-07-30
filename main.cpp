#pragma once

#include <version>


#if defined(_HAS_CXX20)
	#include "./TranslateAPI/TranslateAPI.h"
#endif	// defined(_HAS_CXX20)


#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	_STD string appid {};
	_STD		cout << "请输入 appid: ";
	_STD		cin >> appid;

	_STD string appkey {};
	_STD		cout << "请输入 appkey: ";
	_STD		cin >> appkey;

	if (BaiduTranslate baiduTranslate(appid, appkey); !baiduTranslate.isOK())
	{
		_STD cout << "初始化失败: " << baiduTranslate.whatHappened() << _STD endl;

		return -1;
	}
	else
	{
		_STD string source {};
		_STD string from { "auto" };
		_STD string to {};

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
	}

	return 0;
}
