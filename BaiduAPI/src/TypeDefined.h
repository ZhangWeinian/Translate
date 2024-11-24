#pragma once

#ifndef TYPEDEFINED

	#define TYPEDEFINED

	#include <type_traits>
	#include <concepts>
	#include <cstdint>
	#include <string>
	#include <version>



using _string  = _STD  string;
using _wstring = _STD wstring;

using _size_t  = _STD  size_t;
using _int	   = int_least32_t;
using _uint	   = uint_least32_t;

namespace BaiduTranslateDLL
{
	// 在此命名空间中，可以使用字符串字面量 "s" 来创建字符串
	using ::std::string_literals::operator""s;

	// 创建一个约束，用于检查字符串类型是否为 _string 或可以通过 _string 的转换构造函数转换为 _string
	template <typename StringType>
	concept _string_type =
		_STD same_as<StringType, _string> || _STD constructible_from<_string, StringType>;
} // namespace BaiduTranslateDLL



#endif // !TYPEDEFINED
