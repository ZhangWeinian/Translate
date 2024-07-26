#pragma once

/*
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
*/

#include <version>

#if defined(_HAS_CXX20)

	#include "pch.h"

	#include "__inter__AttributeDefinition.h"

	#include <exception>
	#include <string>
	#include <format>
	#include <iterator>
	#include <stdexcept>
	#include <string_view>
	#include <cstdint>

/// <summary>
/// 封装后的基础运行时异常，不对外开放
/// </summary>
class __inter__RunTimeError: public _STD runtime_error
{
public:
	__inter__RunTimeError(const _STD string&		 errMsg,
						  const _STD string_view	 errFile = "",
						  const _STD string_view	 errFunc = "",
						  const _STD uint_least32_t& errLine = 0) noexcept:
		_STD runtime_error(errMsg)
	{
		what_str.clear();

		// clang-format off
		_STD format_to(_STD back_inserter(what_str),
					   "[信息]: {}\n\n[文件]: {}\n\n[函数]: \"{}\"\n\n[行号]: {}\n",
					   errMsg,
					   errFile,
					   errFunc,
					   errLine);	 // clang-format on
	}

	~__inter__RunTimeError(void) noexcept override = default;

	const char* what(void) const final
	{
		return what_str.c_str() != nullptr ? what_str.c_str() : "未知";
	}

private:
	_STD string what_str {};
};

/// <summary>
/// 与 curl 有关的异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns></returns>
class NetworkError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit NetworkError(const _STD string&		   errMsg,
												const _STD string_view	   errFile,
												const _STD string_view	   errFunc,
												const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError(errMsg, errFile, errFunc, errLine)
	{}

	~NetworkError(void) noexcept override = default;
};

/// <summary>
/// 解析 json 数据异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns></returns>
class JSONAnalysisError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit JSONAnalysisError(const _STD string&			errMsg,
													 const _STD string_view		errFile,
													 const _STD string_view		errFunc,
													 const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError(errMsg, errFile, errFunc, errLine)
	{}

	~JSONAnalysisError(void) noexcept override = default;
};

/// <summary>
/// 获取最终 url 异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns></returns>
class URLError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit URLError(const _STD string&		   errMsg,
											const _STD string_view	   errFile,
											const _STD string_view	   errFunc,
											const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError(errMsg, errFile, errFunc, errLine)
	{}

	~URLError(void) noexcept override = default;
};

/// <summary>
/// 文件操作异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns></returns>
class FileError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit FileError(const _STD string&			errMsg,
											 const _STD string_view		errFile,
											 const _STD string_view		errFunc,
											 const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError(errMsg, errFile, errFunc, errLine)
	{}

	~FileError(void) noexcept override = default;
};

/// <summary>
/// 计算 md5 异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns></returns>
class MD5Error final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit MD5Error(const _STD string&		   errMsg,
											const _STD string_view	   errFile,
											const _STD string_view	   errFunc,
											const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError(errMsg, errFile, errFunc, errLine)
	{}

	~MD5Error(void) noexcept override = default;
};

/// <summary>
/// 用户信息异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns></returns>
class AppIDError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit AppIDError(const _STD string&		 errMsg,
											  const _STD string_view	 errFile,
											  const _STD string_view	 errFunc,
											  const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError(errMsg, errFile, errFunc, errLine)
	{}

	~AppIDError(void) noexcept override = default;
};

/// <summary>
/// 其他异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns></returns>
class OtherError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit OtherError(const _STD string&		 errMsg,
											  const _STD string_view	 errFile,
											  const _STD string_view	 errFunc,
											  const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError(errMsg, errFile, errFunc, errLine)
	{}

	~OtherError(void) noexcept override = default;
};

/// <summary>
/// 异常处理函数
/// </summary>
/// <param name="e">标准库异常及其派生类</param>
/// <returns></returns>
EXCEPTIONHANDLING_API _STD string HandleException(const _STD exception& e) noexcept;

#endif	// _HAS_CXX20
