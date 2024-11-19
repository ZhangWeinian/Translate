#pragma once

#include "BaiduAPI.h"

#include <type_traits>
#include <array>
#include <format>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <random>
#include <string>
#include <version>

#include <curl/curl.h>
#include <curl/easy.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>

void BaiduTranslateDLL::BaiduTranslateDLLError::SetLastError(
	const ErrorCodeEnum error_code) noexcept
{
	p_error_code			= error_code;
	p_error_info.error_info = p_error_infos_def.at(error_code).error_info;
	p_error_info.error_tip	= p_error_infos_def.at(error_code).error_tip;
}

void BaiduTranslateDLL::BaiduTranslateDLLError::SetErrorTip(const _STD string& error_tip) noexcept
{
	p_error_info.error_tip = error_tip;
}

_STD string BaiduTranslateDLL::BaiduTranslateDLLError::GetErrorInfo(void) noexcept
{
	Json::Value		   root {};
	_STD ostringstream os {};

	root["错误代码"] = static_cast<unsigned int>(p_error_code);
	root["错误信息"] = p_error_info.error_info;
	root["提示信息"] = p_error_info.error_tip;

	Json::StreamWriterBuilder builder {};
	builder["commentStyle"] = "None";
	builder["indentation"]	= "    ";

	_STD unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root, &os);

	return os.str();
}

BaiduTranslateDLL::BaiduTranslateFunction::
	BaiduTranslateFunction(const _STD string& appid, const _STD string& appkey) noexcept
{
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

		BaiduTranslateDLLError::SetLastError(BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		BaiduTranslateDLLError::SetErrorTip("CURL 初始化失败在构造函数中。");

		return;
	}

	if (m_ctx != nullptr)
	{
		EVP_DigestInit_ex(m_ctx, EVP_md5(), nullptr);

		m_init_is_no_error = true;
	}
	else
	{
		m_init_is_no_error = false;

		BaiduTranslateDLLError::SetLastError(BAIDUTRANSLATE_FUNC_CTX_IS_NULL);
		BaiduTranslateDLLError::SetErrorTip("MD5 初始化失败在构造函数中。");

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

	if (m_ctx != nullptr)
	{
		EVP_MD_CTX_free(m_ctx);
	}
}

_STD string BaiduTranslateDLL::BaiduTranslateFunction::Translate(const _STD string& query,
																 const _STD string& from,
																 const _STD string& to,
																 const _STD string& appid,
																 const _STD string& appkey) noexcept
{
	_STD string result {};

	if (m_curl == nullptr)
	{
		BaiduTranslateDLLError::SetLastError(BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
		BaiduTranslateDLLError::SetErrorTip("Translate 函数中发现 CURL 句柄为空。");

		return _STD move(result);
	}

	if (m_ctx == nullptr)
	{
		BaiduTranslateDLLError::SetLastError(BAIDUTRANSLATE_FUNC_CTX_IS_NULL);
		BaiduTranslateDLLError::SetErrorTip("Translate 函数中发现 MD5 上下文为空。");

		return _STD move(result);
	}

	if (!appid.empty() && !appkey.empty())
	{
		m_appid	 = appid;
		m_appkey = appkey;
	}
	else if (m_appid.empty() || m_appkey.empty())
	{
		BaiduTranslateDLLError::SetLastError(BAIDUTRANSLATE_FUNC_APPID_OR_APPKEY_IS_EMPTY);
		BaiduTranslateDLLError::SetErrorTip(
			"初始化 appid、appkey 和 Translate 函数传入的 appid、appkey 都是空值。");

		return _STD move(result);
	}

	const _STD size_t salt { m_dis(m_gen) };
	const _STD string sign { GetMD5(_STD format("{0}{1}{2}{3}", m_appid, query, salt, m_appkey)) };
	const _STD string fullUrl { (_STD format("{0}appid={1}&q={2}&from={3}&to={4}&salt={5}&sign={6}",
											 m_url,
											 m_appid,
											 query,
											 from,
											 to,
											 salt,
											 sign)) };

	_STD string		  readBuffer {};

	curl_easy_setopt(m_curl, CURLOPT_URL, fullUrl.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &readBuffer);

	if (auto res { curl_easy_perform(m_curl) }; res != CURLE_OK)
	{
		BaiduTranslateDLLError::SetLastError(BAIDUTRANSLATE_FUNC_CURL_RETURN_ERROR);
		BaiduTranslateDLLError::SetErrorTip("curl 发起的请求返回了错误。");

		return _STD move(result);
	}

	::Json::Value  root {};
	::Json::Reader reader {};

	if (!reader.parse(readBuffer, root))
	{
		BaiduTranslateDLLError::SetLastError(BAIDUTRANSLATE_FUNC_JSON_PARSE_ERROR);
		BaiduTranslateDLLError::SetErrorTip("Translate 函数中 JSON 文本解析错误。");

		return _STD move(result);
	}

	if (root.isMember("error_code"))
	{
		BaiduTranslateDLLError::SetLastError(BAIDUTRANSLATE_FUNC_API_RETURN_ERROR);
		BaiduTranslateDLLError::SetErrorTip("Translate 函数中 API 请求成功但返回错误。");

		return _STD move(result);
	}

	return _STD move(root["trans_result"][0]["dst"].asString());
}

void BaiduTranslateDLL::BaiduTranslateFunction::SetAppIDAndKey(const _STD string& appid,
															   const _STD string& appkey) noexcept
{
	m_appid	 = appid;
	m_appkey = appkey;
}

_STD string BaiduTranslateDLL::BaiduTranslateFunction::GetAppIDAndKey(void) noexcept
{
	return _STD move(_STD format(R"({{ "appid": "{0}", "appkey": "{1}" }})", m_appid, m_appkey));
}

bool BaiduTranslateDLL::BaiduTranslateFunction::InitIsNoError(void) noexcept
{
	return m_init_is_no_error;
}

_STD string BaiduTranslateDLL::BaiduTranslateFunction::GetMD5(const _STD string& str) const noexcept
{
	_STD array<unsigned char, MD5_DIGEST_LENGTH> md5 {};

	EVP_DigestUpdate(m_ctx, str.c_str(), str.size());
	EVP_DigestFinal_ex(m_ctx, md5.data(), nullptr);

	_STD string result {};

	for (const auto& c: md5)
	{
		_STD format_to(_STD back_inserter(result), "{0:02x}", c);
	}

	return result;
}

size_t BaiduTranslateDLL::BaiduTranslateFunction::
	CurlWriteCallback(const char* contents, _STD size_t size, _STD size_t nmemb, _STD string* userp)
{
	userp->append(contents, size * nmemb);
	return size * nmemb;
}
