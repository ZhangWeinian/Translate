#pragma once

#include "TypeDefined.h"

#include "BaiduAPI.h"
#include "GlobalError.h"
#include "PasswordAPI.h"

#include <type_traits>
#include <format>
#include <random>
#include <string>
#include <version>

#include <curl/curl.h>
#include <curl/easy.h>
#include <json/reader.h>
#include <json/value.h>

BaiduTranslateDLL::BaiduTranslateFunction::BaiduTranslateFunction(const _string& appid,
																  const _string& appkey) noexcept
{
	if (PasswordFunction::InitIsNoError())
	{
		m_init_is_no_error = true;
	}
	else
	{
		m_init_is_no_error = false;

		return;
	}

	if (m_curl != nullptr)
	{
		curl_easy_setopt(m_curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);

		m_init_is_no_error = true;
	}
	else
	{
		m_init_is_no_error = false;

		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		GlobalErrorHandling::SetErrorTip("CURL 上下文为空在 BaiduTranslateFunction 初始化中。");

		return;
	}

	if (m_curl == nullptr)
	{
		m_init_is_no_error = false;

		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		GlobalErrorHandling::SetErrorTip("CURL 初始化失败在 BaiduTranslateFunction 初始化中。");

		return;
	}

	m_appid	 = appid;
	m_appkey = appkey;
}

BaiduTranslateDLL::BaiduTranslateFunction::~BaiduTranslateFunction(void) noexcept
{
	if (m_curl != nullptr)
	{
		curl_easy_cleanup(m_curl);
	}
}

_string BaiduTranslateDLL::BaiduTranslateFunction::Translate(const _string& query,
															 const _string& from,
															 const _string& to,
															 const _string& appid,
															 const _string& appkey) noexcept
{
	_string result {};

	if (!m_init_is_no_error)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		GlobalErrorHandling::SetErrorTip("BaiduTranslateFunction 初始化失败，无法执行操作。");
		return _STD move(result);
	}

	if (!appid.empty() && !appkey.empty())
	{
		m_appid	 = appid;
		m_appkey = appkey;
	}
	else if (m_appid.empty() || m_appkey.empty())
	{
		GlobalErrorHandling::SetLastError(
			ErrorCodeEnum::BAIDUTRANSLATE_FUNC_APPID_OR_APPKEY_IS_EMPTY);
		GlobalErrorHandling::SetErrorTip(
			"初始化 appid、appkey 和 Translate 函数传入的 appid、appkey 都是空值。");
		return _STD move(result);
	}

	const _size_t salt { m_dis(m_gen) };
	const _string sign {
		m_password.GetMD5(_STD format("{0}{1}{2}{3}", m_appid, query, salt, m_appkey))
	};
	const _string fullUrl { (_STD format("{0}appid={1}&q={2}&from={3}&to={4}&salt={5}&sign={6}",
										 m_url,
										 m_appid,
										 query,
										 from,
										 to,
										 salt,
										 sign)) };

	_string		  readBuffer {};

	curl_easy_setopt(m_curl, CURLOPT_URL, fullUrl.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &readBuffer);

	if (auto res { curl_easy_perform(m_curl) }; res != CURLE_OK)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_RETURN_ERROR);
		GlobalErrorHandling::SetErrorTip("curl 发起的请求返回了错误。");
		return _STD move(result);
	}

	::Json::Value  root {};
	::Json::Reader reader {};

	if (!reader.parse(readBuffer, root))
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_JSON_PARSE_ERROR);
		GlobalErrorHandling::SetErrorTip("Translate 函数中 JSON 文本解析错误。");
		return _STD move(result);
	}

	if (root.isMember("error_code"))
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_API_RETURN_ERROR);
		GlobalErrorHandling::SetErrorTip("Translate 函数中 API 请求成功但返回错误。");
		return _STD move(result);
	}

	return _STD move(root["trans_result"][0]["dst"].asString());
}

void BaiduTranslateDLL::BaiduTranslateFunction::SetAppIDAndKey(const _string& appid,
															   const _string& appkey) noexcept
{
	if (!m_init_is_no_error)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		GlobalErrorHandling::SetErrorTip("BaiduTranslateFunction 初始化失败，无法执行操作。");
		return;
	}

	m_appid	 = appid;
	m_appkey = appkey;

	m_password.SetAppIDAndKey(appid, appkey);
}

_string BaiduTranslateDLL::BaiduTranslateFunction::GetAppIDAndKey(void) noexcept
{
	if (!m_init_is_no_error)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		GlobalErrorHandling::SetErrorTip("BaiduTranslateFunction 初始化失败，无法执行操作。");
		return "";
	}

	return _STD move(_STD format(R"({{ "appid": "{0}", "appkey": "{1}" }})", m_appid, m_appkey));
}

bool BaiduTranslateDLL::BaiduTranslateFunction::InitIsNoError(void) noexcept
{
	return m_init_is_no_error;
}

_size_t BaiduTranslateDLL::BaiduTranslateFunction::
	CurlWriteCallback(const char* contents, _size_t size, _size_t nmemb, _string* userp)
{
	userp->append(contents, size * nmemb);
	return size * nmemb;
}
