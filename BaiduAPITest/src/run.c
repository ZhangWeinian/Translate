#pragma once

#include "Baidu.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	const char* query  = "Hello, World!";
	const char* from   = "en";
	const char* to	   = "zh";
	const char* appid  = "1234567890";
	const char* appkey = "abcdefg";

	const char* result = BaiduTranslate_Translate(query, from, to, appid, appkey);

	fputs(result, stdout);

	return 0;
}
