#pragma once

#include "pch.h"

#include "__inter__AttributeDefinition.h"
#include "ExceptionHandling.h"

#include <type_traits>
#include <exception>
#include <format>
#include <iterator>
#include <string>
#include <version>

_STD string HandleException(const _STD exception& e) noexcept
{
	_STD string str {};

	_STD		format_to(_STD back_inserter(str), "Exception: {}\n", e.what());

	return _STD move(str);
}
