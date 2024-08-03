#pragma once

#include "pch.h"

/*
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
*/

#if defined(_TRANSLATEAPI_ITEM) || defined(_TRANSLATEAPISTATIC_ITEM)

	#include <curl/curl.h>
	#include <string_view>
	#include <memory>
	#include <string>
	#include <version>

	#include "../SaveData/SaveData.h"
	#include "__inter__AttributeDefinition.h"

	#ifndef _SAVEDATA
		#define _SAVEDATA ::
	#endif	// !_SAVEDATA

/// <summary>
/// 使用百度翻译 API 进行翻译，需要提供 AppID 和 AppKey
/// </summary>
class __inter__BDTranslate final
{
public:

	/// <summary>
	/// 接受 AppID 和 AppKey 的构造函数
	/// </summary>
	/// <param name="appid">百度翻译的 AppID ，默认为空，尝试读取本地数据</param>
	/// <param name="appkey">对应的 key ，默认为空，尝试读取本地数据</param>
	/// <returns>无返回值</returns>
	explicit __inter__BDTranslate(const _STD string& appid, const _STD string& appkey) noexcept;

	/// <summary>
	/// 析构函数，释放资源
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>无返回值</returns>
	~__inter__BDTranslate(void) noexcept;

	/// <summary>
	/// 设置百度翻译的 AppID 和 AppKey
	/// </summary>
	/// <param name="appid">你的百度翻译 AppID</param>
	/// <param name="appkey">与 AppID 对应的 key</param>
	/// <returns>返回一个 bool 值，表示身份验证是否成功</returns>
	_NODISCARD bool InterBaiduTranslateSetAppID(const _STD string& appid, const _STD string& appkey) noexcept;

	/// <summary>
	/// 翻译文本
	/// </summary>
	/// <param name="source">原文</param>
	/// <param name="from">原文语言类型</param>
	/// <param name="to">目标语言类型</param>
	/// <returns>翻译后的译文 或 错误信息</returns>
	_STD string InterBaiduTranslate(_STD string_view source, _STD string_view from, _STD string_view to) noexcept;

	/// <summary>
	/// 判断当前对象的运行状态是否正确
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回状态</returns>
	bool isOK(void) const noexcept
	{
		return m_isOK;
	}

	/// <summary>
	/// 获取当前对象的运行状态的详细信息
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回状态信息</returns>
	_STD string whatHappened(void) const noexcept
	{
		return m_message;
	}

private:

	/// <summary>
	/// 用于记录当前对象的运行状态
	/// </summary>
	bool m_isOK { true };

	/// <summary>
	/// 用于记录当前对象的运行状态的详细信息
	/// </summary>
	_STD string m_message {};

	/// <summary>
	/// 用于记录翻译结果
	/// </summary>
	_STD string m_ans {};

	/// <summary>
	/// 记录翻译信息，包括 AppID、AppKey、翻译源文本、翻译原语言、翻译目标语言
	/// </summary>
	TranslateInfoType m_TranslateInfo {};

	/// <summary>
	/// 初始化一个 curl 句柄，用于访问百度翻译 API ，避免重复初始化
	/// </summary>
	CURL* m_pCurl { nullptr };

	/// <summary>
	/// 用委托的方式管理 SaveData 对象实例，用于保存和读取 AppID 和 AppKey ，以及处理给定字符串的 md5 值
	/// </summary>
	_STD unique_ptr<_SAVEDATA SaveData> m_pSaveData { nullptr };

	/// <summary>
	/// 此函数仅供内部使用。用于接收数据的回调函数，此函数必须是静态的
	/// </summary>
	/// <param name="data">接收到的数据</param>
	/// <param name="size">数据大小</param>
	/// <param name="nmemb">数据数量</param>
	/// <param name="userdata">用户数据</param>
	/// <returns>返回接收到的数据大小</returns>
	static _STD size_t InterWriteCallback(const char*  data,
										  _STD size_t  size,
										  _STD size_t  nmemb,
										  _STD string* userdata) noexcept;

	/// <summary>
	/// 此函数仅供内部使用。对原文进行编码，以确保 URL 正确
	/// </summary>
	/// <param name="source">要被编码的字符串</param>
	/// <returns>编码后的字符串</returns>
	_STD string InterSourceEncode(const _STD string& source) const noexcept;

	/// <summary>
	/// 此函数仅供内部使用。用于获取 url
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回一个浏览器可接受的 url </returns>
	_STD string InterGetURL(void) noexcept;

	/// <summary>
	/// 此函数仅供内部使用。访问 url 成功但是返回一个错误信息，用于获取错误信息
	/// </summary>
	/// <param name="errorCode">百度翻译 API 预定的错误码</param>
	/// <returns>对应信息</returns>
	_STD string InterGetErrorInfo(const _STD string& errorCode) const noexcept;
};

#endif	// defined(_TRANSLATEAPI_ITEM) || defined(_TRANSLATEAPISTATIC_ITEM)
