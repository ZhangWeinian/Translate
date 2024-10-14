#pragma once

#include "BaiduAPI.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	using namespace std;

	// 20230527001691590
	// dNBnV59wxA6ZhONLUmtF

	BaiduTranslate::Constructor();

	BaiduTranslate::SetAppIDAndKey("20230527001691590", "dNBnV59wxA6ZhONLUmtF");

	cout << BaiduTranslate::Translate("hello", "en", "zh") << endl;

	BaiduTranslate::Destructor();

	return 0;
}
