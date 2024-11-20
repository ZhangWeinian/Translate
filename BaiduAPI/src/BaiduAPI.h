#pragma once

#ifndef BAIDUAPI

	#define BAIDUAPI

	#include "TypeDefined.h"

	#include "GlobalError.h"
	#include "PasswordAPI.h"

	#include <random>
	#include <version>

	#include <curl/curl.h>
	#include <curl/easy.h>

namespace BaiduTranslateDLL
{
	class BaiduTranslateFunction final
	{
	public:
		BaiduTranslateFunction(const _string& appid = "", const _string& appkey = "") noexcept;

		~BaiduTranslateFunction(void) noexcept;

		_string		Translate(const _string& query,
							  const _string& from,
							  const _string& to,
							  const _string& appid	= "",
							  const _string& appkey = "") noexcept;

		void		SetAppIDAndKey(const _string& appid, const _string& appkey) noexcept;

		_string		GetAppIDAndKey(void) noexcept;

		static bool InitIsNoError(void) noexcept;

	private:
		static _size_t
			CurlWriteCallback(const char* contents, _size_t size, _size_t nmemb, _string* userp);

	private:
		using enum ::BaiduTranslateDLL::ErrorCodeEnum;
		using ErrorHandling = ::BaiduTranslateDLL::GlobalErrorHandling;

		PasswordFunction   m_password {};

		static inline bool m_init_is_no_error { false };

		_string			   m_appid { "" };
		_string			   m_appkey { "" };

		const _string	   m_url { R"(http://api.fanyi.baidu.com/api/trans/vip/translate?)" };
		CURL*			   m_curl { curl_easy_init() };

		_STD mt19937_64	   m_gen { _STD random_device {}() };
		_STD uniform_int_distribution<_size_t> m_dis { 32'768, 65'536 };
	};
} // namespace BaiduTranslateDLL

#endif // !BAIDUAPI
