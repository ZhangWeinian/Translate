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

BaiDuTranslateDLL::BaiDuTranslate::BaiDuTranslate(const _STD string& appid, const _STD string& appkey) noexcept:
	p_appid(appid),
	p_appkey(appkey)
{
	p_ctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(p_ctx, EVP_md5(), nullptr);

	p_curl = curl_easy_init();
}

BaiDuTranslateDLL::BaiDuTranslate::~BaiDuTranslate(void) noexcept
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

_STD string BaiDuTranslateDLL::BaiDuTranslate::Translate(const _STD string& query,
														 const _STD string& from,
														 const _STD string& to,
														 const _STD string& appid,
														 const _STD string& appkey) const noexcept
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

	const _STD string id   = appid.empty() ? p_appid : appid;
	const _STD string key  = appkey.empty() ? p_appkey : appkey;

	const __int32	  salt = GetIntRandom();
	const _STD string sign = GetMD5(_STD format("{0}{1}{2}{3}", id, query, salt, key));

	const _STD string fullUrl =
		_STD	format("{0}appid={1}&q={2}&from={3}&to={4}&salt={5}&sign={6}", p_url, id, query, from, to, salt, sign);

	_STD string readBuffer {};

	curl_easy_setopt(p_curl, CURLOPT_URL, fullUrl.c_str());
	curl_easy_setopt(p_curl, CURLOPT_WRITEFUNCTION, BaiDuTranslateDLL::BaiDuTranslate::CurlWriteCallback);
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
					   root["error_msg"].asString());
		return result;
	}

	return root["trans_result"][0]["dst"].asString();
}

void BaiDuTranslateDLL::BaiDuTranslate::SetAppIDAndKey(const _STD string& appid, const _STD string& appkey) noexcept
{
	p_appid	 = appid;
	p_appkey = appkey;
}

_STD string BaiDuTranslateDLL::BaiDuTranslate::GetAppIDAndKey(void) const noexcept
{
	_STD string result {};
	_STD		format_to(_STD back_inserter(result), R"({{ "appid": "{0}", "appkey": "{1}" }})", p_appid, p_appkey);
	return result;
}

inline __int32 BaiDuTranslateDLL::BaiDuTranslate::GetIntRandom(void) const noexcept
{
	_STD mt19937_64 gen(_STD random_device {}());
	_STD uniform_int_distribution<__int32> dis(32'768, 65'536);

	return dis(gen);
}

inline _STD string BaiDuTranslateDLL::BaiDuTranslate::GetMD5(const _STD string& str) const noexcept
{
	_STD array<unsigned char, MD5_DIGEST_LENGTH> md5 {};

	EVP_DigestUpdate(p_ctx, str.c_str(), str.size());
	EVP_DigestFinal_ex(p_ctx, md5.data(), nullptr);

	_STD string result;

	for (const auto& c: md5)
	{
		result += _STD format("{0:02x}", c);
	}

	return result;
}

size_t BaiDuTranslateDLL::BaiDuTranslate::
	CurlWriteCallback(const char* contents, size_t size, size_t nmemb, _STD string* userp)
{
	userp->append(contents, size * nmemb);
	return size * nmemb;
}
