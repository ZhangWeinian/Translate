#pragma once

#include "BaiduAPI.h"

#include <string>
#include <version>

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		return 0;
	}
	else
	{
		_STD string str {};

		for (size_t i = 1; i < argc; ++i)
		{
			str += argv[i];
			str += '\n';
		}

		BaiduTranslate::Constructor();

		BaiduTranslate::SetAppIDAndKey("20210720000876307", "X3Q1Q1Q1Q1Q1Q1Q1");

		_STD string result = BaiduTranslate::Translate(str, "auto", "zh");

		BaiduTranslate::Destructor();
	}

	return 0;
}
