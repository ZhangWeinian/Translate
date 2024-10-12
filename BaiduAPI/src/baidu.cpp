#pragma once

#include "pch.h"

#include "../include/baidu.h"

#include <array>
#include <format>
#include <iterator>
#include <random>
#include <string>
#include <version>

#include <curl/curl.h>
#include <curl/easy.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <json/reader.h>
#include <json/value.h>

BaiduTranslateDLL::BaiduTranslate::BaiduTranslate(const _STD string& appid, const _STD string& appkey) noexcept:
	p_app_id(appid),
	p_app_key(appkey)
{
	p_ctx = EVP_MD_CTX_new();

	EVP_DigestInit_ex(p_ctx, EVP_md5(), nullptr);

	p_curl = curl_easy_init();
}

BaiduTranslateDLL::BaiduTranslate::~BaiduTranslate(void) noexcept
{
	if (p_ctx != nullptr)
	{
		EVP_MD_CTX_free(p_ctx);
	}
	p_ctx = nullptr;

	if (p_curl != nullptr)
	{
		curl_easy_cleanup(p_curl);
	}
	p_curl = nullptr;
}

_STD string BaiduTranslateDLL::BaiduTranslate::Translate(const _STD string& query,
														 const _STD string& from,
														 const _STD string& to,
														 const _STD string& appid,
														 const _STD string& appkey) noexcept
{
	_STD string result {};

	if (p_curl == nullptr)
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "错误信息": "{1}" }})",
					   -1,
					   "CURL 初始化失败");
		return result;
	}

	if (p_ctx == nullptr)
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "错误信息": "{1}" }})",
					   -1,
					   "MD5 初始化失败");
		return result;
	}

	const _STD string id   = appid.empty() ? p_app_id : appid;
	const _STD string key  = appkey.empty() ? p_app_key : appkey;

	const __int32	  salt = p_dis(p_gen);
	const _STD string sign = GetMD5(_STD format("{0}{1}{2}{3}", id, query, salt, key));

	const _STD string fullUrl =
		_STD	format("{0}appid={1}&q={2}&from={3}&to={4}&salt={5}&sign={6}", p_url, id, query, from, to, salt, sign);

	_STD string readBuffer {};

	curl_easy_setopt(p_curl, CURLOPT_URL, fullUrl.c_str());
	curl_easy_setopt(p_curl, CURLOPT_WRITEFUNCTION, BaiduTranslateDLL::BaiduTranslate::CurlWriteCallback);
	curl_easy_setopt(p_curl, CURLOPT_WRITEDATA, &readBuffer);

	auto res = curl_easy_perform(p_curl);

	if (res != CURLE_OK)
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "错误信息": "{1}" }})",
					   -1,
					   curl_easy_strerror(res));
		return result;
	}

	::Json::Value  root {};
	::Json::Reader reader {};

	if (!reader.parse(readBuffer, root))
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "错误信息": "{1}" }})",
					   -1,
					   reader.getFormattedErrorMessages());
		return result;
	}

	if (root.isMember("error_code"))
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "错误信息": "{1}" }})",
					   root["error_code"].asString(),
					   GetAPIErrorInfo(root["error_code"].asInt()));
		return result;
	}

	return root["trans_result"][0]["dst"].asString();
}

void BaiduTranslateDLL::BaiduTranslate::SetAppIDAndKey(const _STD string& appid, const _STD string& appkey) noexcept
{
	p_app_id  = appid;
	p_app_key = appkey;
}

_STD string BaiduTranslateDLL::BaiduTranslate::GetAppIDAndKey(void) const noexcept
{
	_STD string result {};
	_STD		format_to(_STD back_inserter(result), R"({{ "appid": "{0}", "appkey": "{1}" }})", p_app_id, p_app_key);
	return result;
}

inline _STD string BaiduTranslateDLL::BaiduTranslate::GetMD5(const _STD string& str) const noexcept
{
	_STD array<unsigned char, MD5_DIGEST_LENGTH> md5 {};

	EVP_DigestUpdate(p_ctx, str.c_str(), str.size());
	EVP_DigestFinal_ex(p_ctx, md5.data(), nullptr);

	_STD string result {};

	for (const auto& c: md5)
	{
		result += _STD format("{0:02x}", c);
	}

	return result;
}

size_t BaiduTranslateDLL::BaiduTranslate::
	CurlWriteCallback(const char* contents, size_t size, size_t nmemb, _STD string* userp)
{
	userp->append(contents, size * nmemb);
	return size * nmemb;
}

_STD string BaiduTranslateDLL::BaiduTranslate::GetAPIErrorInfo(__int32 error_code) const noexcept
{
	const auto& iter = p_APIErrorInfo.find(error_code);

	if (iter != p_APIErrorInfo.end())
	{
		return iter->second;
	}
	else
	{
		return "未知错误";
	}
}
