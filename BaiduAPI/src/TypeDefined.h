#pragma once

#ifndef TYPEDEFINED

	#define TYPEDEFINED

	#include <string_view>
	#include <cstdint>
	#include <filesystem>
	#include <string>
	#include <version>



using _string	   = _STD	   string;
using _wstring	   = _STD	  wstring;
using _string_view = _STD string_view;

using _size_t	   = _STD	   size_t;
using _int		   = int_least32_t;
using _uint		   = uint_least32_t;

using _file_path   = _STD filesystem::path;

namespace BaiduTranslateDLL
{
	// 在此命名空间中，可以使用字符串字面量 "s" 来创建字符串
	using ::std::string_literals::operator""s;
} // namespace BaiduTranslateDLL



#endif // !TYPEDEFINED
