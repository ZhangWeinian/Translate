#pragma once

#include "pch.h"

/*
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
*/

#if defined(_TRANSLATEAPISTATIC_ITEM)

	#include <string>
	#include <memory>
	#include <version>

	#include "__inter__BDTranslate.h"

	#ifndef _INTERBDTRANSLATE
		#define _INTERBDTRANSLATE ::
	#endif	// !_INTERBDTRANSLATE

#endif		// defined(_TRANSLATEAPISTATIC_ITEM)

class BDTranslateLib final
{
public:

	/// <summary>
	/// 调用此函数初始化使用百度翻译 API 的翻译器
	/// </summary>
	/// <param name="appid">百度翻译 AppID</param>
	/// <param name="appkey">对应的 key</param>
	/// <returns>无返回值</returns>
	static void BDTranslate(const char* appid, const char* appkey) noexcept;

	/// <summary>
	/// 调用此函数设置百度翻译 API 的 AppID 和 AppKey
	/// </summary>
	/// <param name="appid">百度翻译 AppID</param>
	/// <param name="appkey">对应的 key</param>
	/// <returns>返回值确认是否设置成功</returns>
	static bool SetAppID(const char* appid, const char* appkey) noexcept;

	/// <summary>
	/// 此函数调用百度翻译 API 进行翻译
	/// </summary>
	/// <param name="source">原文</param>
	/// <param name="from">原文语言类型</param>
	/// <param name="to">目标语言类型</param>
	/// <returns>翻译后的译文 或 错误信息</returns>
	static const char* Translate(const char* source, const char* from, const char* to) noexcept;

	/// <summary>
	/// 检查 __inter__BDTranslate 对象的运行状态是否正确
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回值表示当前状态是否正确</returns>
	static bool isOK(void) noexcept;

	/// <summary>
	/// 获取 __inter__BDTranslate 对象的运行状态的详细信息
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回运行状态的详细信息，空值表示无异常</returns>
	static const char* whatHappened(void) noexcept;

#if defined(_TRANSLATEAPISTATIC_ITEM)

private:

	/// <summary>
	/// 用委托的方式管理 BaiduTranslateAPI 对象
	/// </summary>
	static _STD unique_ptr<_INTERBDTRANSLATE __inter__BDTranslate> m_pBaiduTranslateAPI;

	/// <summary>
	/// 用于记录当前对象的运行状态是否正确
	/// </summary>
	static bool m_isOK;

	/// <summary>
	/// 用于记录当前对象的运行状态的详细信息
	/// </summary>
	static _STD string m_message;

	/// <summary>
	/// 用于记录翻译结果，避免多次执行完全相同的翻译请求
	/// </summary>
	static _STD string m_ans;

#endif	// defined(_TRANSLATEAPISTATIC_ITEM)
};
