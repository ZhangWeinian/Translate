#pragma once

#include <Baidu.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	const TranslateObj* p_translate_obj = BaiduTranslate_Init("", "");

	if (p_translate_obj == NULL)
	{
		printf("Init failed\n");
		return -1;
	}

	return 0;
}
