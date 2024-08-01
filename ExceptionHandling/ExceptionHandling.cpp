#pragma once

#include "ExceptionHandling.h"
#include "__inter__AttributeDefinition.h"

#include <source_location>
#include <type_traits>
#include <exception>
#include <format>
#include <fstream>
#include <iosfwd>
#include <iterator>
#include <ostream>
#include <string>
#include <version>

#include <spdlog/spdlog.h>

inline _STD string HandleException(const _STD exception& e) noexcept
{
	_STD string str {};

	_STD		format_to(_STD back_inserter(str), "Exception: {}\n", e.what());

	return _STD move(str);
}

void __inter__RunTimeError::InterSetLog(void) const noexcept
{
	if (_STD ofstream file("./log/Error.log", _STD ios::app); !file.is_open())
	{
		const auto& source_location { _STD source_location::current() };

		_SPDLOG		error(
			"[信息]: 无法打记录日志的文件 \"./log/Error.log \"\n\n[发生在]: {}\n\n[函数]: \"{}\"\n\n[行号]: {}\n",
			source_location.function_name(),
			source_location.line(),
			source_location.file_name());
	}
	else
	{
		_SPDLOG				   error(logMsg);

		file << logMsg << _STD endl;
	}
}
