#pragma once

#include "../SaveData/SaveData.h"
#include "BaiDuTranslate.h"
#include "pch.h"

#include <exception>
#include <format>
#include <iostream>

#include <curl/curl.h>
#include <json/json.h>

namespace Translate::inline ERR_DEF
{
	class NetworkError final: public _STD exception
	{
	public:
		explicit NetworkError(const char* msg): _STD exception(msg) {}
	};

	class JSONAnalysisError final: public _STD exception
	{
	public:
		explicit JSONAnalysisError(const char* msg): _STD exception(msg) {}
	};

	using OtherError = _STD exception;

	void					HandleException(const _STD exception& e) noexcept
	{
		_STD cerr << _STD format("{}", e.what()) << _STD endl;
	}
}  // namespace Translate::inline ERR_DEF

namespace Translate
{
	_NODISCARD bool SetAppID(const _STD string& appid, const _STD string& appkey) noexcept(false)
	{
		_STD string str = GetIdentifyingCode(appid + appkey);

		if (str.empty())
		{
			throw ERR_DEF::NetworkError("Network error");
		}

		if (str == "error")
		{
			throw ERR_DEF::JSONAnalysisError("JSON analysis error");
		}

		SaveData(str, appid, appkey);

		return GetData(str) == info { appid, appkey };
	}

	_STD string BaiDuTranslate(const _STD string& source, const _STD string& from, const _STD string& to)
		noexcept(false)
	{
		return "";
	}
}  // namespace Translate

/// <summary>
/// 设置百度翻译的 AppID 和 AppKey
/// </summary>
/// <param name="appid">你的百度翻译 AppID</param>
/// <param name="appkey">与 AppID 对应的 key</param>
/// <returns>返回一个 bool 值，表示身份验证是否成功</returns>
_NODISCARD bool SetBaiDuTranslateAppID(const _STD string& appid, const _STD string& appkey) noexcept
{
	try
	{
		_STD string str = GetIdentifyingCode(appid + appkey);

		if (str.empty())
		{
			throw ::Translate::NetworkError("Network error");
		}

		if (str == "error")
		{
			throw ::Translate::JSONAnalysisError("JSON analysis error");
		}

		return ::Translate::SetAppID(appid, appkey);
	}
	catch (const _STD exception& e)
	{
		::Translate::HandleException(e);

		return false;
	}
}

/// <summary>
/// 调用百度翻译 API 进行翻译
/// </summary>
/// <param name="source">要翻译的字符串原文</param>
/// <param name="from">原文语言类型</param>
/// <param name="to">译文语言类型</param>
/// <returns>返回译文或错误信息</returns>
_STD string BaiduTranslate(const _STD string& source, const _STD string& from, const _STD string& to) noexcept
{
	try
	{
		return ::Translate::BaiDuTranslate(source, from, to);
	}
	catch (const _STD exception& e)
	{
		::Translate::HandleException(e);

		return e.what();
	}
}
