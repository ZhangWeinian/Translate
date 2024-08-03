#pragma once

#include "pch.h"

/*
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
*/

#if defined(_TRANSLATEAPI_ITEM)

	#include <memory>
	#include <string>
	#include <version>

	#include "__inter__AttributeDefinition.h"
	#include "__inter__BDTranslate.h"

	#ifndef _INTERBDTRANSLATE
		#define _INTERBDTRANSLATE ::
	#endif	// !_INTERBDTRANSLATE

#endif		// defined(_TRANSLATEAPI_ITEM)

class BDTranslateDll final
{
public:

	/// <summary>
	/// 调用此函数初始化使用百度翻译 API 的翻译器
	/// </summary>
	/// <param name="appid">百度翻译 AppID</param>
	/// <param name="appkey">对应的 key</param>
	/// <returns>无返回值</returns>
	TRANSLATEAPI_API BDTranslateDll(const _STD string& appid, const _STD string& appkey) noexcept;

	/// <summary>
	/// 析构函数。由于使用了智能指针，因此无需手动释放资源
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>无返回值</returns>
	TRANSLATEAPI_API ~BDTranslateDll(void) noexcept = default;

	/// <summary>
	/// 调用此函数设置百度翻译 API 的 AppID 和 AppKey
	/// </summary>
	/// <param name="appid">百度翻译 AppID</param>
	/// <param name="appkey">对应的 key</param>
	/// <returns>返回值确认是否设置成功</returns>
	TRANSLATEAPI_API _NODISCARD bool SetAppID(const _STD string& appid, const _STD string& appkey) noexcept;

	/// <summary>
	/// 此函数调用百度翻译 API 进行翻译
	/// </summary>
	/// <param name="source">原文</param>
	/// <param name="from">原文语言类型</param>
	/// <param name="to">目标语言类型</param>
	/// <returns>翻译后的译文 或 错误信息</returns>
	TRANSLATEAPI_API _NODISCARD const char*
		Translate(const _STD string& source, const _STD string& from, const _STD string& to) noexcept;

	/// <summary>
	/// 检查 __inter__BDTranslate 对象的运行状态是否正确
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回值表示当前状态是否正确</returns>
	TRANSLATEAPI_API bool isOK(void) noexcept;

	/// <summary>
	/// 获取 __inter__BDTranslate 对象的运行状态的详细信息
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回运行状态的详细信息，空值表示无异常</returns>
	TRANSLATEAPI_API const char* whatHappened(void) noexcept;

#if defined(_TRANSLATEAPI_ITEM)

private:

	/// <summary>
	/// 用委托的方式管理 BaiduTranslateAPI 对象
	/// </summary>
	_STD unique_ptr<_INTERBDTRANSLATE __inter__BDTranslate> m_pBaiduTranslateAPI { nullptr };

	/// <summary>
	/// 用于记录当前对象的运行状态是否正确
	/// </summary>
	bool m_isOK { true };

	/// <summary>
	/// 用于记录当前对象的运行状态的详细信息
	/// </summary>
	_STD string m_message { "" };

	/// <summary>
	/// 用于记录翻译结果
	/// </summary>
	_STD string m_ans { "" };
#endif	// defined(_TRANSLATEAPI_ITEM)
};
