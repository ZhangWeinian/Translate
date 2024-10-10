#pragma once

#include "baidu.h"

#include <iostream>

int main(int argc, char* argv[])
{
	using namespace std;

	::BaiDuTranslateDLL::BaiDuTranslate baidu("20230527001691590", "dNBnV59wxA6ZhONLUmtF");

	cout << baidu.Translate("hello", "en", "zh") << endl;

	return 0;
}
