#pragma once

#include <iostream>
#include <version>

#if defined(_HAS_CXX20)
	#include "./BaiDuTranslate/BaiDuTranslate.h"
#endif	// defined(_HAS_CXX20)

using namespace std;

int main(int argc, char* argv[])
{
	BaiduTranslate baiduTranslate("20230527001691590", "dNBnV59wxA6ZhONLUmtF");

	cout << baiduTranslate.Translate("Hello, World!", "en", "zh") << endl;

	return 0;
}
