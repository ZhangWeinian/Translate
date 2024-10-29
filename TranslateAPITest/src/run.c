#pragma once

#include "Translate.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	TranslateFunction* p_translate_function = InitTranslateFunction("", "");

	const char*		   result				= Translate(p_translate_function, "hello", "en", "zh", "", "");

	printf_s("%s\n", result);

	return 0;
}
