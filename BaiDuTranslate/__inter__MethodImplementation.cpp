#pragma once

#include "../SaveData/SaveData.h"
#include "BaiDuTranslate.h"
#include "pch.h"

#include <string_view>
#include <type_traits>
#include <chrono>
#include <exception>
#include <format>
#include <iostream>
#include <map>
#include <string>

#include <curl/curl.h>
#include <curl/easy.h>
#include <json/reader.h>
#include <json/value.h>



#ifndef _SAVEDATA
	#define _SAVEDATA ::
#endif	// !_SAVEDATA



namespace ERR_DEF
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

	class TranslateError final: public _STD exception
	{
	public:
		explicit TranslateError(const char* msg): _STD exception(msg) {}
	};

	using OtherError = _STD exception;
}  // namespace ERR_DEF

namespace STATIC_INFO
{
	struct ErrorCodeInfo
	{
		_STD string meaning;
		_STD string solution;
	};

	static const _STD map<_STD string, ErrorCodeInfo, _STD less<>> errorCodeMap = {
		{ "52000", { "成功", "" }																											  },
		{ "52001", { "请求超时", "检查请求原文是否超长，以及原文或译文参数是否在支持的语种列表里" }			},
		{ "52002", { "系统错误", "请重试" }																							   },
		{ "52003", { "未授权用户", "请检查 AppID 是否正确或者服务是否开通" }												 },
		{ "54000", { "必填参数为空", "请检查是否少传参数" }																	   },
		{ "54001", { "签名错误", "请检查您的签名生成方法" }																	   },
		{ "54003", { "访问频率受限", "请降低您的调用频率，或在控制台进行身份认证后切换为高级版/尊享版" }	 },
		{ "54004", { "账户余额不足", "请前往管理控制台为账户充值" }															},
		{ "54005", { "长原文请求频繁", "请降低长原文的发送频率，3s 后再试" }												 },
		{ "58000", { "客户端 IP 非法", "检查个人资料里填写的IP地址是否正确，可前往开发者信息-基本信息修改" } },
		{ "58001", { "译文语言方向不支持", "检查译文语言是否在语言列表里" }												},
		{ "58002", { "服务当前已关闭", "请前往管理控制台开启服务" }															},
		{ "58003",
		  { "此IP已被封禁",
			"同一 IP 当日使用多个 AppID 发送翻译请求，则该IP将被封禁当日请求权限，次日解封。请勿将 AppID "
			"和密钥填写到第三方软"
			"件中。" }																														},
		{ "90107", { "认证未通过或未生效", "请前往 我的认证 查看认证进度" }												},
		{ "20003", { "请求内容存在安全风险", "请检查请求内容" }																 }
	};
}  // namespace STATIC_INFO

BaiduTranslate::BaiduTranslate(const _STD string& appid, const _STD string& appkey) noexcept
{
	TranslateInfo.appid	 = appid;
	TranslateInfo.appkey = appkey;

	SetAppID(appid, appkey);
}

_NODISCARD bool BaiduTranslate::SetAppID(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (_SAVEDATA SaveData(appid, appkey))
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
		HandleException(e);

		return "";
	}
}

_STD size_t BaiduTranslate::WriteCallback(void* contents, _STD size_t size, _STD size_t nmemb, _STD string* userp) const
{
	_STD size_t totalSize = size * nmemb;

	userp->append(static_cast<char*>(contents), totalSize);

	return totalSize;
}

_STD string BaiduTranslate::GetURL(void)
{
	const auto [appid, appkey] = _SAVEDATA GetData("code");
	const auto salt			   = _STD chrono::system_clock::to_time_t(_STD chrono::system_clock::now());

	const auto sign			   = _SAVEDATA GetMD5(_STD format("{}{}{}{}", appid, TranslateInfo.source, salt, appkey));

	return _STD format("http://api.fanyi.baidu.com/api/trans/vip/translate?q={}&from={}&to={}&appid={}&salt={}&sign={}",
					   TranslateInfo.source,
					   TranslateInfo.from,
					   TranslateInfo.to,
					   appid,
					   salt,
					   sign);
}

_STD string BaiduTranslate::GetErrorInfo(const _STD string& errorCode) const noexcept
{
	if (auto it = ::STATIC_INFO::errorCodeMap.find(errorCode); it != ::STATIC_INFO::errorCodeMap.end())
	{
		return _STD format("错误代码：{}\n含义：{}\t建议解决方案：{}",
						   errorCode,
						   it->second.meaning,
						   it->second.solution);
	}
	else
	{
		return _STD format("未知错误代码:{}\n建议解决方案：{}", errorCode, "请联系技术支持");
	}
}

_STD string BaiduTranslate::InterTranslate(void)
{
	curl_global_init(CURL_GLOBAL_ALL);

	auto curl = curl_easy_init();

	if (curl == nullptr)
	{
		throw ::ERR_DEF::NetworkError("curl_easy_init error");
	}

	_STD string readBuffer = {};

	curl_easy_setopt(curl, CURLOPT_URL, GetURL().c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &BaiduTranslate::WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

	curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_LAST);

	if (auto res = curl_easy_perform(curl); res != CURLE_OK)
	{
		throw ::ERR_DEF::NetworkError(curl_easy_strerror(res));
	}

	Json::Value root = {};

	if (Json::Reader reader; !reader.parse(readBuffer, root))
	{
		throw ::ERR_DEF::JSONAnalysisError("JSON analysis error");
	}

	_STD string k1 = root["k1"].asString();
	_STD string k2 = root["k2"].asString();

	curl_easy_cleanup(curl);

	curl_global_cleanup();

	return k1 + k2;
}

void BaiduTranslate::HandleException(const _STD exception& e) const noexcept
{
	_STD cerr << _STD format("{}", e.what()) << _STD endl;
}
