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
	}

	return 0;
}
