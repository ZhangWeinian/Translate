#pragma once

#include "./BaiDuTranslate/BaiDuTranslate.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	BaiduTranslate baiduTranslate("20210819000900600", "X3Q1Q1J");

	cout << baiduTranslate.Translate("Hello, World!", "en", "zh") << endl;

	return 0;
}
