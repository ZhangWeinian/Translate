#pragma once

#ifndef BAIDUAPI

	#define BAIDUAPI

	#include "TypeDefined.h"

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

namespace BaiduTranslateDLL
{
	class BaiduTranslateFunction final
	{
	public:
		template <_string_type StringType1, _string_type StringType2>
		BaiduTranslateFunction(StringType1&& appid, StringType2&& appkey) noexcept:
			m_uri(R"(http://api.fanyi.baidu.com/api/trans/vip/translate?)"s),
			m_curl(curl_easy_init()),
			m_gen(_STD random_device {}()),
			m_dis(32'768, 65'536)
		{
			if (!PasswordFunction::InitIsNoError())
			{
				m_init_is_no_error = false;

				return;
			}

			if (m_curl == nullptr)
			{
				m_init_is_no_error = false;

				GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
				GlobalErrorHandling::SetErrorTip(
					"CURL 上下文为空在 BaiduTranslateFunction 初始化中。"s);

				return;
			}

			curl_easy_setopt(m_curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
			curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);

			if (m_curl == nullptr)
			{
				m_init_is_no_error = false;

				GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
				GlobalErrorHandling::SetErrorTip(
					"CURL 初始化失败在 BaiduTranslateFunction 初始化中。"s);

				return;
			}

			m_appid			   = _STD  move(_string(appid));
			m_appkey		   = _STD move(_string(appkey));

			m_init_is_no_error = true;
		}

		~BaiduTranslateFunction(void) noexcept
		{
			if (m_curl != nullptr)
			{
				curl_easy_cleanup(m_curl);
			}
		}

		_string Translate(const _string& query,
						  const _string& from,
						  const _string& to,
						  const _string& appid	= ""s,
						  const _string& appkey = ""s) noexcept
		{
			_string result {};

			if (!m_init_is_no_error)
			{
				GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
				GlobalErrorHandling::SetErrorTip(
					"BaiduTranslateFunction 初始化失败，无法执行操作。"s);
				return _STD move(result);
			}

			_string tmp_appid { appid };
			_string tmp_appkey { appkey };

			if (tmp_appid.empty() || tmp_appkey.empty())
			{
				if (m_appid.empty() || m_appkey.empty())
				{
					GlobalErrorHandling::SetLastError(
						ErrorCodeEnum::BAIDUTRANSLATE_FUNC_APPID_OR_APPKEY_IS_EMPTY);
					GlobalErrorHandling::SetErrorTip(
						"初始化 appid、appkey 和 Translate 函数传入的 appid、appkey 都是空值。"s);
					return _STD move(result);
				}

				tmp_appid  = m_appid;
				tmp_appkey = m_appkey;
			}

			const _size_t salt { m_dis(m_gen) };
			const _string sign {
				m_password.GetMD5(_STD format("{0}{1}{2}{3}", tmp_appid, query, salt, tmp_appkey))
			};
			const _string fullUrl {
				(_STD format("{0}appid={1}&q={2}&from={3}&to={4}&salt={5}&sign={6}",
							 m_uri,
							 tmp_appid,
							 query,
							 from,
							 to,
							 salt,
							 sign))
			};

			_string readBuffer {};

			curl_easy_setopt(m_curl, CURLOPT_URL, fullUrl.c_str());
			curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
			curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &readBuffer);

			if (auto res { curl_easy_perform(m_curl) }; res != CURLE_OK)
			{
				GlobalErrorHandling::SetLastError(
					ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_RETURN_ERROR);
				GlobalErrorHandling::SetErrorTip("curl 发起的请求返回了错误。"s);
				return _STD move(result);
			}

			::Json::Value  root {};
			::Json::Reader reader {};

			if (!reader.parse(readBuffer, root))
			{
				GlobalErrorHandling::SetLastError(
					ErrorCodeEnum::BAIDUTRANSLATE_FUNC_JSON_PARSE_ERROR);
				GlobalErrorHandling::SetErrorTip("Translate 函数中 JSON 文本解析错误。"s);
				return _STD move(result);
			}

			if (root.isMember("error_code"))
			{
				GlobalErrorHandling::SetLastError(
					ErrorCodeEnum::BAIDUTRANSLATE_FUNC_API_RETURN_ERROR);
				GlobalErrorHandling::SetErrorTip("Translate 函数中 API 请求成功但返回错误。"s);
				return _STD move(result);
			}

			return _STD move(root["trans_result"][0]["dst"].asString());
		}

		template <_string_type StringType1, _string_type StringType2>
		void SetAppIDAndKey(StringType1&& appid, StringType2&& appkey) noexcept
		{
			if (!m_init_is_no_error)
			{
				GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
				GlobalErrorHandling::SetErrorTip(
					"BaiduTranslateFunction 初始化失败，无法执行操作。"s);
				return;
			}

			m_appid	 = _STD	 move(_string(appid));
			m_appkey = _STD move(_string(appkey));

			m_password.SetAppIDAndKey(_STD forward<StringType1>(appid),
									  _STD forward<StringType2>(appkey));
		}

		_string GetAppIDAndKey(void) const noexcept
		{
			if (!m_init_is_no_error)
			{
				GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
				GlobalErrorHandling::SetErrorTip(
					"BaiduTranslateFunction 初始化失败，无法执行操作。"s);
				return "";
			}

			return _STD move(
				_STD format(R"({{ "appid": "{0}", "appkey": "{1}" }})", m_appid, m_appkey));
		}

		static bool InitIsNoError(void) noexcept
		{
			return m_init_is_no_error;
		}

	private:
		static _size_t
			CurlWriteCallback(const char* contents, _size_t size, _size_t nmemb, _string* userp)
		{
			userp->append(contents, size * nmemb);
			return size * nmemb;
		}

	private:
		static inline bool m_init_is_no_error { false };

		PasswordFunction   m_password {};

		_string			   m_appid {};
		_string			   m_appkey {};

		const _string	   m_uri {};
		CURL*			   m_curl {};

		_STD mt19937_64	   m_gen {};
		_STD uniform_int_distribution<_size_t> m_dis {};
	};
} // namespace BaiduTranslateDLL

#endif // !BAIDUAPI
