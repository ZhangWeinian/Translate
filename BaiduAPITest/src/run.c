#pragma once

#include "Baidu.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if (!BaiduTranslate_Init("", ""))
	{
		fputs("BaiduTranslate_Init failed.\n", stderr);
		return EXIT_FAILURE;
	}

	const char* query  = "Hello, World!";
	const char* from   = "en";
	const char* to	   = "zh";
	const char* appid  = "";
	const char* appkey = "";

	const char* result = BaiduTranslate_Translate(query, from, to, appid, appkey);

	if (result)
	{
		printf("BaiduTranslate_Translate: %s\n", result);
	}
	else
	{
		fputs("BaiduTranslate_Translate failed.\n", stderr);
	}

	return 0;
}
