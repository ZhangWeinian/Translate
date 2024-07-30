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

	/// <summary>
	/// 接受错误信息、错误文件（可省略）、错误函数（可省略）、错误行号（可省略）的构造函数
	/// </summary>
	/// <param name="errMsg">错误信息</param>
	/// <param name="errFile">发生错误的文件</param>
	/// <param name="errFunc">发生错误的函数</param>
	/// <param name="errLine">发生错误的行号</param>
	/// <returns>无返回值</returns>
	__inter__RunTimeError(const _STD string&		 errMsg,
						  const _STD string_view	 errFile = "",
						  const _STD string_view	 errFunc = "",
						  const _STD uint_least32_t& errLine = 0) noexcept:
		_STD runtime_error(errMsg)
	{
		// clang-format off
		_STD format_to(_STD back_inserter(logMsg),
					   "[信息]: {}\n\n[发生在]: {}\n\n[抛出异常的函数]: \"{}\"\n\n[所在行号]: {}\n",
					   errMsg,
					   errFile,
					   errFunc,
					   errLine);	 // clang-format on

		_STD format_to(_STD back_inserter(showMsg), "{}", errMsg);
	}

	/// <summary>
	/// 析构函数
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>无返回值</returns>
	~__inter__RunTimeError(void) noexcept override = default;

	/// <summary>
	/// 重写 what() 函数
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回错误信息</returns>
	const char* what(void) const final
	{
		if (!logMsg.empty())
		{
			InterSetLog();
		}

		return showMsg.c_str() != nullptr ? showMsg.c_str() : "错误信息未知";
	}

private:

	/// <summary>
	/// 记录简略的错误信息，向用户展示
	/// </summary>
	_STD string showMsg {};

	/// <summary>
	/// 记录详细的错误信息，写入日志文件
	/// </summary>
	_STD string logMsg {};

	/// <summary>
	/// 把有关的信息写入日志文件
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>无返回值</returns>
	EXCEPTIONHANDLING_API void InterSetLog(void) const noexcept;
};

/// <summary>
/// 与 curl 有关的异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns>本身无返回值，但是可以用 HandleException() 函数拿到错误信息</returns>
class NetworkError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit NetworkError(const _STD string&		   errMsg,
												const _STD string_view	   errFile,
												const _STD string_view	   errFunc,
												const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError("curl 错误：" + errMsg, errFile, errFunc, errLine)
	{}

	EXCEPTIONHANDLING_API ~NetworkError(void) noexcept override = default;
};

/// <summary>
/// 解析 json 数据异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns>本身无返回值，但是可以用 HandleException() 函数拿到错误信息</returns>
class JSONAnalysisError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit JSONAnalysisError(const _STD string&			errMsg,
													 const _STD string_view		errFile,
													 const _STD string_view		errFunc,
													 const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError("json 错误：" + errMsg, errFile, errFunc, errLine)
	{}

	EXCEPTIONHANDLING_API ~JSONAnalysisError(void) noexcept override = default;
};

/// <summary>
/// 获取最终 url 异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns>本身无返回值，但是可以用 HandleException() 函数拿到错误信息</returns>
class URLError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit URLError(const _STD string&		   errMsg,
											const _STD string_view	   errFile,
											const _STD string_view	   errFunc,
											const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError("url 错误：" + errMsg, errFile, errFunc, errLine)
	{}

	EXCEPTIONHANDLING_API ~URLError(void) noexcept override = default;
};

/// <summary>
/// 文件操作异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns>本身无返回值，但是可以用 HandleException() 函数拿到错误信息</returns>
class FileError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit FileError(const _STD string&			errMsg,
											 const _STD string_view		errFile,
											 const _STD string_view		errFunc,
											 const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError("文件流错误：" + errMsg, errFile, errFunc, errLine)
	{}

	EXCEPTIONHANDLING_API ~FileError(void) noexcept override = default;
};

/// <summary>
/// 计算 md5 异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns>本身无返回值，但是可以用 HandleException() 函数拿到错误信息</returns>
class MD5Error final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit MD5Error(const _STD string&		   errMsg,
											const _STD string_view	   errFile,
											const _STD string_view	   errFunc,
											const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError("md5 错误：" + errMsg, errFile, errFunc, errLine)
	{}

	EXCEPTIONHANDLING_API ~MD5Error(void) noexcept override = default;
};

/// <summary>
/// 用户信息异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns>本身无返回值，但是可以用 HandleException() 函数拿到错误信息</returns>
class AppIDError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit AppIDError(const _STD string&		 errMsg,
											  const _STD string_view	 errFile,
											  const _STD string_view	 errFunc,
											  const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError("用户信息错误：" + errMsg, errFile, errFunc, errLine)
	{}

	EXCEPTIONHANDLING_API ~AppIDError(void) noexcept override = default;
};

/// <summary>
/// 其他异常
/// </summary>
/// <param name="errMsg">基本错误信息</param>
/// <param name="errFile">抛出异常的文件路径</param>
/// <param name="errFunc">抛出异常的函数名</param>
/// <param name="errLine">抛出异常的所在行</param>
/// <returns>本身无返回值，但是可以用 HandleException() 函数拿到错误信息</returns>
class OtherError final: public __inter__RunTimeError
{
public:
	EXCEPTIONHANDLING_API explicit OtherError(const _STD string&		 errMsg,
											  const _STD string_view	 errFile,
											  const _STD string_view	 errFunc,
											  const _STD uint_least32_t& errLine) noexcept:
		__inter__RunTimeError("其他错误：" + errMsg, errFile, errFunc, errLine)
	{}

	EXCEPTIONHANDLING_API ~OtherError(void) noexcept override = default;
};

/// <summary>
/// 异常处理函数
/// </summary>
/// <param name="e">标准库异常及其派生类</param>
/// <returns>返回简略的错误信息，同时把详细的错误信息记录到日志</returns>
EXCEPTIONHANDLING_API _STD string HandleException(const _STD exception& e) noexcept;

/// <summary>
/// 记录运行状态
/// </summary>
class RuntimeStatus
{
public:
	/// <summary>
	/// 运行状态是否正常
	/// </summary>
	bool isOK { true };

	/// <summary>
	/// 运行状态的详细信息
	/// </summary>
	_STD string message { "一切正常" };

	/// <summary>
	/// 重置运行状态
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>无返回值</returns>
	void Resetting(void) noexcept
	{
		isOK	= true;
		message = "一切正常";
	}
};

#endif	// _HAS_CXX20
