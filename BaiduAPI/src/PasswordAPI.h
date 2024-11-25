#pragma once

#ifndef PASSWORDAPI

	#define PASSWORDAPI

	#include "TypeDefined.h"

	#include <openssl/types.h>

namespace BaiduTranslateDLL
{
	class PasswordFunction final
	{
	public:
		explicit PasswordFunction(void) noexcept;

		~PasswordFunction(void) noexcept;

		_string		GetMD5(_string_view str) const noexcept;

		_string		GetAppID(void) const noexcept;

		_string		GetAppKey(void) const noexcept;

		_string		GetAppIDAndKey(void) const noexcept;

		void		SetAppIDAndKey(_string_view appid, _string_view appkey) noexcept;

		static bool InitIsNoError(void) noexcept;

	private:
		static _file_path GetLocalFilePath(void) noexcept;

		void			  ReadLocalAppidAndAppkey(void) noexcept;

		_string			  Encryption(_string_view str) const noexcept;

		_string			  Decryption(_string_view str) const noexcept;

	private:
		static inline bool m_init_is_no_error { false };

		_file_path		   m_path {};

		_string			   m_appid {};
		_string			   m_appkey {};

		EVP_MD_CTX*		   m_ctx {};
	};
} // namespace BaiduTranslateDLL

#endif // !PASSWORDAPI
