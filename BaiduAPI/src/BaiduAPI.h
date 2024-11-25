#pragma once

#ifndef BAIDUAPI

	#define BAIDUAPI

	#include "TypeDefined.h"

	#include "PasswordAPI.h"

	#include <random>
	#include <version>

	#include <curl/curl.h>

namespace BaiduTranslateDLL
{
	class BaiduTranslateFunction final
	{
	public:
		explicit BaiduTranslateFunction(_string_view appid, _string_view appkey) noexcept;

		~BaiduTranslateFunction(void) noexcept;

		_string		Translate(_string_view query,
							  _string_view from,
							  _string_view to,
							  _string_view appid  = "",
							  _string_view appkey = "") noexcept;

		void		SetAppIDAndKey(_string_view appid, _string_view appkey) noexcept;

		_string		GetAppIDAndKey(void) const noexcept;

		static bool InitIsNoError(void) noexcept;

	private:
		static _size_t
			CurlWriteCallback(const char* contents, _size_t size, _size_t nmemb, _string* userp);

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
