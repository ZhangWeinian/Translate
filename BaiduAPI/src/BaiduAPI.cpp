#pragma once

#include "BaiduAPI.h"

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

BaiduTranslateDLL::BaiduTranslateFunction::
	BaiduTranslateFunction(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (p_curl = curl_easy_init(); p_curl != nullptr)
	{
		curl_easy_setopt(p_curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
		curl_easy_setopt(p_curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(p_curl, CURLOPT_SSL_VERIFYHOST, 0L);

		p_init_is_no_error = true;
	}
	else
	{
		p_init_is_no_error = false;
		return;
	}

	if (p_ctx = EVP_MD_CTX_new(); p_ctx != nullptr)
	{
		EVP_DigestInit_ex(p_ctx, EVP_md5(), nullptr);

		p_init_is_no_error = true;
	}
	else
	{
		p_init_is_no_error = false;
		return;
	}

	p_appid	 = appid;
	p_appkey = appkey;
}

BaiduTranslateDLL::BaiduTranslateFunction::~BaiduTranslateFunction(void) noexcept
{
	if (p_curl != nullptr)
	{
		curl_easy_cleanup(p_curl);
	}

	if (p_ctx != nullptr)
	{
		EVP_MD_CTX_free(p_ctx);
	}
}

_STD string BaiduTranslateDLL::BaiduTranslateFunction::Translate(const _STD string& query,
																 const _STD string& from,
																 const _STD string& to,
																 const _STD string& appid,
																 const _STD string& appkey) noexcept
{
	_STD string result {};

	if (p_curl == nullptr)
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "提示信息": "{1}" }})",
					   -1,
					   "CURL 初始化失败");
		return result;
	}

	if (p_ctx == nullptr)
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "提示信息": "{1}" }})",
					   -1,
					   "MD5 初始化失败");
		return result;
	}

	if (!appid.empty() && !appkey.empty())
	{
		p_appid	 = appid;
		p_appkey = appkey;
	}
	else if (p_appid.empty() || p_appkey.empty())
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "提示信息": "{1}" }})",
					   -1,
					   "APPID 或 APPKEY 为空");
		return result;
	}

	const _STD size_t salt	  = p_dis(p_gen);
	const _STD string sign	  = GetMD5(_STD format("{0}{1}{2}{3}", p_appid, query, salt, p_appkey));

	const _STD string fullUrl = _STD format("{0}appid={1}&q={2}&from={3}&to={4}&salt={5}&sign={6}",
											p_url,
											p_appid,
											query,
											from,
											to,
											salt,
											sign);

	_STD string						 readBuffer {};

	curl_easy_setopt(p_curl, CURLOPT_URL, fullUrl.c_str());
	curl_easy_setopt(p_curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
	curl_easy_setopt(p_curl, CURLOPT_WRITEDATA, &readBuffer);

	if (auto res = curl_easy_perform(p_curl); res != CURLE_OK)
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "提示信息": "{1}" }})",
					   -1,
					   curl_easy_strerror(res));
		return result;
	}

	::Json::Value  root {};
	::Json::Reader reader {};

	if (!reader.parse(readBuffer, root))
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "提示信息": "{1}" }})",
					   -1,
					   reader.getFormattedErrorMessages());
		return result;
	}

	if (root.isMember("error_code"))
	{
		_STD format_to(_STD back_inserter(result),
					   R"({{ "错误代码": "{0}", "提示信息": "{1}" }})",
					   root["error_code"].asString(),
					   GetAPIErrorInfo(root["error_code"].asString()));
		return result;
	}

	return root["trans_result"][0]["dst"].asString();
}

void BaiduTranslateDLL::BaiduTranslateFunction::SetAppIDAndKey(const _STD string& appid,
															   const _STD string& appkey) noexcept
{
	p_appid	 = appid;
	p_appkey = appkey;
}

_STD string BaiduTranslateDLL::BaiduTranslateFunction::GetAppIDAndKey(void) noexcept
{
	_STD string result {};
	_STD		format_to(_STD back_inserter(result),
						  R"({{ "appid": "{0}", "appkey": "{1}" }})",
						  p_appid,
						  p_appkey);
	return result;
}

bool BaiduTranslateDLL::BaiduTranslateFunction::InitIsNoError(void) noexcept
{
	return p_init_is_no_error;
}

inline _STD string BaiduTranslateDLL::BaiduTranslateFunction::GetMD5(
	const _STD string& str) noexcept
{
	_STD array<unsigned char, MD5_DIGEST_LENGTH> md5 {};

	EVP_DigestUpdate(p_ctx, str.c_str(), str.size());
	EVP_DigestFinal_ex(p_ctx, md5.data(), nullptr);

	_STD string result {};

	for (const auto& c: md5)
	{
		_STD format_to(_STD back_inserter(result), "{0:02x}", c);
	}

	return result;
}

_STD size_t BaiduTranslateDLL::BaiduTranslateFunction::
	CurlWriteCallback(const char* contents, _STD size_t size, _STD size_t nmemb, _STD string* userp)
{
	userp->append(contents, size * nmemb);
	return size * nmemb;
}

_STD string BaiduTranslateDLL::BaiduTranslateFunction::GetAPIErrorInfo(
	const _STD string& error_code) noexcept
{
	auto iter = p_API_error_info.find(error_code);

	if (iter != p_API_error_info.end())
	{
		return iter->second;
	}
	else
	{
		return "未知错误";
	}
}
