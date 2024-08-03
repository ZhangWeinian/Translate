#pragma once

#include "pch.h"

// #include "../TranslateAPIStatic/TranslateAPIStatic.h"
#include "CLREncapsulation.h"

#ifndef _TRANSLATEAPISTATIC
	#define _TRANSLATEAPISTATIC ::
#endif	// !_TRANSLATEAPISTATIC


int CLREncapsulation::Class1::test_add(int a, int b)
{
	return a + b;
}
