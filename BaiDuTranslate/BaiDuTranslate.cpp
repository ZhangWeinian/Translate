#pragma once

#include "pch.h"

#include "../ExceptionHandling/ExceptionHandling.h"
#include "../SaveData/SaveData.h"
#include "BaiDuTranslate.h"

#include <source_location>
#include <type_traits>
#include <cctype>
#include <chrono>
#include <exception>
#include <format>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <version>

#include <curl/curl.h>
#include <curl/easy.h>
#include <json/reader.h>
#include <json/value.h>



#ifndef _SAVEDATA
	#define _SAVEDATA ::
#endif	// !_SAVEDATA

#ifndef EXCEPTIONHADLING
	#define EXCEPTIONHADLING ::
#endif	// !EXCEPTIONHADLING



namespace STATIC_INFO
{
	struct ErrorCodeInfo
	{
		_STD string meaning {};
		_STD string solution {};
	};

	// clang-format off
	static const _STD map<_STD string, ErrorCodeInfo, _STD less<>> errorCodeMap {
		{ "52000", { "成功", "成功" } },
		{ "52001", { "请求超时", "检查请求原文是否超长，以及原文或译文参数是否在支持的语种列表里" } },
		{ "52002", { "系统错误", "请重试" } },
		{ "52003", { "未授权用户", "请检查 AppID 是否正确或者服务是否开通" } },
		{ "54000", { "必填参数为空", "请检查是否少传参数" } },
		{ "54001", { "签名错误", "请检查您的签名生成方法" }	},
		{ "54003", { "访问频率受限", "请降低您的调用频率，或在控制台进行身份认证后切换为高级版/尊享版" } },
		{ "54004", { "账户余额不足", "请前往管理控制台为账户充值" } },
		{ "54005", { "长原文请求频繁", "请降低长原文的发送频率，3s 后再试" } },
		{ "58000", { "客户端 IP 非法", "检查个人资料里填写的IP地址是否正确，可前往开发者信息-基本信息修改" } },
		{ "58001", { "译文语言方向不支持", "检查译文语言是否在语言列表里" } },
		{ "58002", { "服务当前已关闭", "请前往管理控制台开启服务" } },
		{ "58003", { "此IP已被封禁","同一 IP 当日使用多个 AppID 发送翻译请求，"
					 "则该 IP 将被封禁当日请求权限，次日解封。请勿将 AppID 和密钥填写到第三方软件中" } },
		{ "90107", { "认证未通过或未生效", "请前往 我的认证 查看认证进度" } },
		{ "20003", { "请求内容存在安全风险", "请检查请求内容" } }
	};	// clang-format on
}  // namespace STATIC_INFO

BaiduTranslate::BaiduTranslate(const _STD string& appid, const _STD string& appkey) noexcept:
	TranslateInfo({ .appid = appid, .appkey = appkey })
{
	try
	{
		if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
		{
			const auto&			   source_location { _STD source_location::current() };

			throw EXCEPTIONHADLING NetworkError("curl_global_init(CURL_GLOBAL_ALL) 返回错误值",
												source_location.file_name(),
												source_location.function_name(),
												source_location.line());
		}

		curl = curl_easy_init();

		if (curl == nullptr)
		{
			curl_global_cleanup();

			const auto&			   source_location { _STD source_location::current() };

			throw EXCEPTIONHADLING NetworkError("curl_easy_init()] 返回空",
												source_location.file_name(),
												source_location.function_name(),
												source_location.line());
		}

		curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_LAST);
	}
	catch (const _STD exception& e)
	{
		EXCEPTIONHADLING HandleException(e);
	}

	pSaveData = _STD make_unique<SaveData>();

	if (appid.empty() || appkey.empty())
	{
		return;
	}

	SetAppID(appid, appkey);
}

BaiduTranslate::BaiduTranslate(void) noexcept
{
	const auto& [appid, appkey] { pSaveData->GetDataFromLocal() };

	if (appid.empty() || appkey.empty())
	{
		return;
	}

	TranslateInfo.appid	 = appid;
	TranslateInfo.appkey = appkey;
}

BaiduTranslate::~BaiduTranslate(void) noexcept
{
	if (curl != nullptr)
	{
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
}

_NODISCARD bool BaiduTranslate::SetAppID(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (pSaveData->SaveDataToLocal(appid, appkey))
	{
		TranslateInfo.appid	 = appid;
		TranslateInfo.appkey = appkey;

		return true;
	}
	else
	{
		return false;
	}
}

_STD string BaiduTranslate::Translate(const _STD string& source,
									  const _STD string& from,
									  const _STD string& to) noexcept
{
	TranslateInfo.source = source;
	TranslateInfo.from	 = from;
	TranslateInfo.to	 = to;

	try
	{
		return InterTranslate();
	}
	catch (const _STD exception& e)
	{
		return EXCEPTIONHADLING HandleException(e);
	}
}

_STD size_t BaiduTranslate::
	InterWriteCallback(const char* data, _STD size_t size, _STD size_t nmemb, _STD string* userdata)
{
	const auto& totalSize { size * nmemb };

	userdata->append(data, totalSize);

	return totalSize;
}

_STD string BaiduTranslate::InterSourceEncode(const _STD string& source) const
{
	_STD string encodedStr {};

	for (const auto& ch: source)
	{
		if (isalnum(static_cast<unsigned char>(ch)) || ch == '-' || ch == '_' || ch == '.' || ch == '~')
		{
			_STD format_to(_STD back_inserter(encodedStr), "{}", ch);
		}
		else if (ch == ' ')
		{
			_STD format_to(_STD back_inserter(encodedStr), "{}", '+');
		}
		else
		{
			_STD format_to(_STD back_inserter(encodedStr), "%{:02X}", ch);
		}
	}

	return _STD move(encodedStr);
}

_STD string BaiduTranslate::InterGetURL(void) noexcept(false)
{
	const auto& [appid, appkey] { pSaveData->GetDataFromLocal() };
	const auto& salt { _CHRONO system_clock::to_time_t(_CHRONO system_clock::now()) };

	_STD string msg {};

	_STD		format_to(_STD back_inserter(msg), "{}{}{}{}", appid, TranslateInfo.source, salt, appkey);

	const auto& sign { pSaveData->GetMD5(msg) };

	if (sign.size() != 32)
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING MD5Error("获取字符串 md5 值错误",
										source_location.file_name(),
										source_location.function_name(),
										source_location.line());
	}

	_STD string url {};

	_STD		format_to(_STD back_inserter(url),
						  "http://api.fanyi.baidu.com/api/trans/vip/translate?q={}&from={}&to={}&appid={}&salt={}&sign={}",
						  InterSourceEncode(TranslateInfo.source),
						  TranslateInfo.from,
						  TranslateInfo.to,
						  appid,
						  salt,
						  sign);

	return _STD move(url);
}

_STD string BaiduTranslate::InterGetErrorInfo(const _STD string& errorCode) const noexcept
{
	_STD string errorInfo {};

	if (const auto& it { ::STATIC_INFO::errorCodeMap.find(errorCode) }; it != ::STATIC_INFO::errorCodeMap.end())
	{
		_STD format_to(_STD back_inserter(errorInfo),
					   "错误码：{}\t含义：{}\n建议解决方案：{}\n",
					   errorCode,
					   it->second.meaning,
					   it->second.solution);
	}
	else
	{
		_STD format_to(_STD back_inserter(errorInfo),
					   "错误代码:{}\t含义：{}\n建议解决方案：{}\n",
					   errorCode,
					   "未定义的错误代码！",
					   "请联系技术支持");
	}

	return errorInfo;
}

_STD string BaiduTranslate::InterTranslate(void) noexcept(false)
{
	if (TranslateInfo.appid.empty() || TranslateInfo.appkey.empty())
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING AppIDError("AppID 或 AppKey 为空",
										  source_location.file_name(),
										  source_location.function_name(),
										  source_location.line());
	}

	_STD string readBuffer {};
	_STD string url {};

	try
	{
		url = InterGetURL();
	}
	catch (const _STD exception&)
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING URLError("获取 url 错误",
										source_location.file_name(),
										source_location.function_name(),
										source_location.line());
	}

	curl_easy_setopt(curl, CURLOPT_URL, url.data());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &BaiduTranslate::InterWriteCallback);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

	if (const auto& res { curl_easy_perform(curl) }; res != CURLE_OK)
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING NetworkError(curl_easy_strerror(res),
											source_location.file_name(),
											source_location.function_name(),
											source_location.line());
	}

	Json::Value root {};

	if (Json::Reader reader; !reader.parse(readBuffer, root))
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING JSONAnalysisError("初始化 json 数据错误",
												 source_location.file_name(),
												 source_location.function_name(),
												 source_location.line());
	}

	if (const auto& error_code { root["error_code"].asString() }; (error_code != "") && (error_code != "52000"))
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING JSONAnalysisError(InterGetErrorInfo(error_code).c_str(),
												 source_location.file_name(),
												 source_location.function_name(),
												 source_location.line());
	}

	try
	{
		return root["trans_result"][0]["dst"].asString();
	}
	catch (const _STD exception&)
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING JSONAnalysisError("返回的 json 数据无法被正确解析",
												 source_location.file_name(),
												 source_location.function_name(),
												 source_location.line());
	}
}
