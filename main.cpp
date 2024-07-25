#pragma once

#include "./BaiDuTranslate/BaiDuTranslate.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	BaiduTranslate baiduTranslate("20230527001691590", "dNBnV59wxA6ZhONLUmtF");

	cout << baiduTranslate.Translate("Hello, World!", "en", "zh") << endl;

	return 0;
}
