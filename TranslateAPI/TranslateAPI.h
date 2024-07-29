#pragma once

/*
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
*/

#include <version>

#if defined(_HAS_CXX20)

	#include "pch.h"

	#include "../SaveData/SaveData.h"
	#include "__inter__AttributeDefinition.h"

	#include <memory>
	#include <string>
	#include <string_view>

	#include <curl/curl.h>

/// <summary>
/// 使用百度翻译 API 进行翻译，需要提供 AppID 和 AppKey
/// </summary>
class BaiduTranslate final
{
public:

	/// <summary>
	/// 接受 AppID 和 AppKey 的构造函数
	/// </summary>
	/// <param name="appid">百度翻译的 AppID</param>
	/// <param name="appkey">对应的 key</param>
	/// <returns>无返回值</returns>
	TRANSLATEAPI_API explicit BaiduTranslate(const _STD string& appid, const _STD string& appkey) noexcept;

	/// <summary>
	/// 尝试使用本地保存的 AppID 和 AppKey 进行构造，如果失败则在调用 Translate 返回错误信息
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>无返回值</returns>
	TRANSLATEAPI_API explicit BaiduTranslate(void) noexcept;

	/// <summary>
	/// 析构函数，释放资源
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>无返回值</returns>
	TRANSLATEAPI_API ~BaiduTranslate(void) noexcept;

	/// <summary>
	/// 设置百度翻译的 AppID 和 AppKey
	/// </summary>
	/// <param name="appid">你的百度翻译 AppID</param>
	/// <param name="appkey">与 AppID 对应的 key</param>
	/// <returns>返回一个 bool 值，表示身份验证是否成功</returns>
	TRANSLATEAPI_API _NODISCARD bool SetAppID(const _STD string& appid, const _STD string& appkey) noexcept;

	/// <summary>
	/// 翻译文本
	/// </summary>
	/// <param name="source">原文</param>
	/// <param name="from">原文语言类型</param>
	/// <param name="to">目标语言类型</param>
	/// <returns>翻译后的译文 或 错误信息</returns>
	TRANSLATEAPI_API _STD string Translate(_STD string_view source,
										   _STD string_view from,
										   _STD string_view to) noexcept;

private:

	/// <summary>
	/// 记录翻译信息，包括 AppID、AppKey、翻译源文本、翻译原语言、翻译目标语言
	/// </summary>
	TranslateInfoType TranslateInfo {};

	/// <summary>
	/// 初始化一个 curl 句柄，用于访问百度翻译 API ，避免重复初始化
	/// </summary>
	CURL* curl { nullptr };

	/// <summary>
	/// 一个 SaveData 的委托，用于保存和读取 AppID 和 AppKey ，以及处理给定字符串的 md5 值
	/// </summary>
	_STD unique_ptr<::SaveData> pSaveData { nullptr };

	/// <summary>
	/// 此函数仅供内部使用。用于接收数据的回调函数，此函数必须是静态的
	/// </summary>
	/// <param name="data"></param>
	/// <param name="size"></param>
	/// <param name="nmemb"></param>
	/// <param name="userdata"></param>
	/// <returns></returns>
	static _STD size_t InterWriteCallback(const char* data, _STD size_t size, _STD size_t nmemb, _STD string* userdata);

	/// <summary>
	/// 此函数仅供内部使用。对原文进行编码，以确保 URL 正确
	/// </summary>
	/// <param name="source">要被编码的字符串</param>
	/// <returns>编码后的字符串</returns>
	static _STD string InterSourceEncode(const _STD string& source);

	/// <summary>
	/// 此函数仅供内部使用。用于获取 url
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回一个浏览器可接受的 url </returns>
	_STD string InterGetURL(const _STD string& source, const _STD string& from, const _STD string& to) noexcept(false);

	/// <summary>
	/// 此函数仅供内部使用。访问 url 成功但是返回一个错误信息，用于获取错误信息
	/// </summary>
	/// <param name="errorCode">百度翻译 API 预定的错误码</param>
	/// <returns>对应信息</returns>
	static _STD string InterGetErrorInfo(const _STD string& errorCode) noexcept;

	/// <summary>
	/// 此函数仅供内部使用。用于翻译文本
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回文本信息，可能是翻译结果，也可能是错误信息</returns>
	_STD string InterTranslate(void) noexcept(false);
};

#endif	// defined(_HAS_CXX20)
