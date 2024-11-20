#pragma once

#ifndef PASSWORDAPI

	#define PASSWORDAPI

	#include "TypeDefined.h"

	#include "GlobalError.h"

	#include <filesystem>
	#include <string>
	#include <version>

	#include <openssl/evp.h>
	#include <openssl/types.h>

namespace BaiduTranslateDLL
{
	class PasswordFunction final
	{
	public:
		explicit PasswordFunction(void) noexcept;

		~PasswordFunction(void) noexcept;

		_string		GetMD5(const _string& str) const noexcept;

		_string		GetAppID(void) const noexcept;

		_string		GetAppKey(void) const noexcept;

		void		SetAppIDAndKey(const _string& appid, const _string& appkey) noexcept;

		static bool InitIsNoError(void) noexcept;

	private:
		_STD filesystem::path GetLocalFilePath(void) noexcept;

		void				  GetLocalAppIDAndKey(_string& appid, _string& appkey) noexcept;

		_string				  Encryption(const _string& str) const noexcept;

		_string				  Decryption(const _string& str) const noexcept;

	private:
		using enum ::BaiduTranslateDLL::ErrorCodeEnum;
		using ErrorHandling = ::BaiduTranslateDLL::GlobalErrorHandling;

		static inline bool m_init_is_no_error { false };

		_STD filesystem::path m_path { "" };

		_string				  m_appid { "" };
		_string				  m_appkey { "" };

		EVP_MD_CTX*			  m_ctx { EVP_MD_CTX_new() };
	};
} // namespace BaiduTranslateDLL

#endif // !PASSWORDAPI
