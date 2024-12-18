#pragma once

#include "TypeDefined.h"

#include "BaiduAPI.h"
#include "GlobalError.h"
#include "PasswordAPI.h"

#include <type_traits>
#include <format>
#include <random>
#include <version>

#include <curl/curl.h>
#include <curl/easy.h>
#include <json/reader.h>
#include <json/value.h>

_string BaiduTranslateDLL::BaiduTranslateFunction::
	Translate(_string_view query, _string_view from, _string_view to, _string_view appid, _string_view appkey) noexcept
{
	if (!m_init_is_no_error)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		GlobalErrorHandling::SetErrorTip(
			R"(BaiduTranslatieFunction initialization failed, unable to perform operation.)");

		return GlobalErrorHandling::GetErrorInfo();
	}

	const _string& full_query { _STD move(CheckQuery(query)) };

	_string		   tmp_appid { appid };
	_string		   tmp_appkey { appkey };

	if (tmp_appid.empty() || tmp_appkey.empty())
	{
		if (m_appid.empty() || m_appkey.empty())
		{
			GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_APPID_OR_APPKEY_IS_EMPTY);
			GlobalErrorHandling::SetErrorTip(
				R"(Initialize appid, appkey, and the appid and appkey passed by the Translate )"
				R"(function are all null values.)");

			return GlobalErrorHandling::GetErrorInfo();
		}

		tmp_appid  = m_appid;
		tmp_appkey = m_appkey;
	}

	const _size_t  salt { m_dis(m_gen) };
	const _string& sign { m_password.GetMD5(_STD format(R"({0}{1}{2}{3})", tmp_appid, full_query, salt, tmp_appkey)) };
	const _string& full_uri { (_STD format(R"({0}appid={1}&q={2}&from={3}&to={4}&salt={5}&sign={6})",
										   m_uri,
										   tmp_appid,
										   full_query,
										   from,
										   to,
										   salt,
										   sign)) };

	_string		   readBuffer {};

	curl_easy_setopt(m_curl, CURLOPT_URL, full_uri.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, BaiduTranslateFunction::CurlWriteCallback);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &readBuffer);

	if (auto res { curl_easy_perform(m_curl) }; res != CURLE_OK)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_RETURN_ERROR);
		GlobalErrorHandling::SetErrorTip(_STD format(R"(The request initiated by CURL returned an error: {})",
													 curl_easy_strerror(res)));

		return GlobalErrorHandling::GetErrorInfo();
	}

	::Json::Value  root { ::Json::objectValue };
	::Json::Reader reader {};

	if (!reader.parse(readBuffer, root))
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_JSON_PARSE_ERROR);
		GlobalErrorHandling::SetErrorTip(R"(JSON text parsing error in Translate function.)");

		return GlobalErrorHandling::GetErrorInfo();
	}

	if (root.isMember("error_code"))
	{
		::Json::Value api_root { ::Json::objectValue };

		api_root["api error code"]	  = root["error_code"];
		api_root["api error message"] = root["error_msg"];

		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_API_RETURN_ERROR);
		GlobalErrorHandling::SetErrorTip(
			R"(The API request in the Translate function was successful but returned an error. )"
			R"(For detailed error information, please refer to 'api error info')");

		return GlobalErrorHandling::GetErrorInfo("api error info", api_root);
	}
	else
	{
		return root["trans_result"][0]["dst"].asString();
	}
}

bool BaiduTranslateDLL::BaiduTranslateFunction::SetAppIDAndKey(_string_view appid, _string_view appkey) noexcept
{
	if (!m_init_is_no_error)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		GlobalErrorHandling::SetErrorTip(
			R"(BaiduTranslatieFunction initialization failed, unable to perform operation.)");

		return false;
	}

	m_appkey = _STD move(_string(appkey));
	m_appid	 = _STD	 move(_string(appid));

	return m_password.SetAppIDAndKey(appid, appkey);
}

_string BaiduTranslateDLL::BaiduTranslateFunction::GetAppIDAndKey(void) const noexcept
{
	if (!m_init_is_no_error)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		GlobalErrorHandling::SetErrorTip(
			R"(BaiduTranslatieFunction initialization failed, unable to perform operation.)");

		return R"({{ "appid": "", "appkey": "" }})";
	}

	return m_password.GetAppIDAndKey();
}

BaiduTranslateDLL::BaiduTranslateFunction::~BaiduTranslateFunction(void) noexcept
{
	if (m_curl != nullptr)
	{
		curl_easy_cleanup(m_curl);
	}
}

_string BaiduTranslateDLL::BaiduTranslateFunction::CheckQuery(_string_view query) const noexcept
{
	_string result {};

	for (const auto& ch: query)
	{
		if (ch == ' ')
		{
			result += "%20";
		}
		else
		{
			result += ch;
		}
	}

	return result;
}

_size_t BaiduTranslateDLL::BaiduTranslateFunction::
	CurlWriteCallback(const char* contents, _size_t size, _size_t nmemb, _string* userp)
{
	userp->append(contents, size * nmemb);
	return size * nmemb;
}
