#pragma once

#ifndef BAIDUAPI

	#define BAIDUAPI

	#include "TypeDefined.h"

	#include "GlobalError.h"
	#include "PasswordAPI.h"

	#include <type_traits>
	#include <random>
	#include <version>

	#include <curl/curl.h>
	#include <curl/easy.h>

namespace BaiduTranslateDLL
{
	template <typename TranslateFunc>
	class BaiduTranslateFunctionBase
	{
	public:
		BaiduTranslateFunctionBase(const BaiduTranslateFunctionBase&)				  = delete;
		BaiduTranslateFunctionBase(BaiduTranslateFunctionBase&&) noexcept			  = delete;
		BaiduTranslateFunctionBase&		 operator=(const BaiduTranslateFunctionBase&) = delete;
		BaiduTranslateFunctionBase&		 operator=(BaiduTranslateFunctionBase&&)	  = delete;

		_NODISCARD static TranslateFunc& GetInstance() noexcept
		{
			static TranslateFunc instance {};
			return instance;
		}

	protected:
		explicit BaiduTranslateFunctionBase(void) noexcept = default;
		virtual ~BaiduTranslateFunctionBase(void) noexcept = default;
	};

	class BaiduTranslateFunction final: public BaiduTranslateFunctionBase<BaiduTranslateFunction>
	{
	public:
		_NODISCARD _string Translate(_string_view query,
									 _string_view from,
									 _string_view to,
									 _string_view appid	 = "",
									 _string_view appkey = "") noexcept;

		_NODISCARD bool	   SetAppIDAndKey(_string_view appid, _string_view appkey) noexcept;

		_NODISCARD _string GetAppIDAndKey(void) const noexcept;

	private:
		friend class BaiduTranslateFunctionBase<BaiduTranslateFunction>;

		explicit BaiduTranslateFunction(void) noexcept:
			m_uri { R"(http://api.fanyi.baidu.com/api/trans/vip/translate?)" },
			m_curl { curl_easy_init() },
			m_gen { _STD random_device {}() },
			m_dis { 32'768, 65'536 }
		{
			if (!PasswordFunction::InitIsNoError())
			{
				// 具体的错误信息在 PasswordFunction 类中设置，此处不做处理
				m_init_is_no_error = false;

				return;
			}

			if (m_curl == nullptr)
			{
				m_init_is_no_error = false;

				GlobalErrorHandling::SetLastError(ErrorCodeEnum::BAIDUTRANSLATE_FUNC_CURL_IS_NULL);
				GlobalErrorHandling::SetErrorTip(R"(CURL context is empty in BaiduTranslatieFunction initialization.)");

				return;
			}

			curl_easy_setopt(m_curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
			//curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
			//curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);

			m_appid			   = _STD  move(m_password.GetAppID());
			m_appkey		   = _STD move(m_password.GetAppKey());

			m_init_is_no_error = true;
		}

		~BaiduTranslateFunction(void) noexcept override;

		_NODISCARD _string		  CheckQuery(_string_view query) const noexcept;

		_NODISCARD static _size_t CurlWriteCallback(const char* contents, _size_t size, _size_t nmemb, _string* userp);

	private:
		PasswordFunction   m_password {};

		static inline bool m_init_is_no_error { false };

		_string			   m_appid {};
		_string			   m_appkey {};

		const _string	   m_uri {};
		CURL*			   m_curl {};

		_STD mt19937_64	   m_gen {};
		_STD uniform_int_distribution<_size_t> m_dis {};
	};
} // namespace BaiduTranslateDLL

#endif // !BAIDUAPI
