#pragma once

#ifndef PASSWORDAPI

	#define PASSWORDAPI

	#include "TypeDefined.h"

	#include "GlobalError.h"

	#include <ShlObj_core.h>
	#include <type_traits>
	#include <array>
	#include <cstring>
	#include <filesystem>
	#include <format>
	#include <fstream>
	#include <iosfwd>
	#include <iterator>
	#include <memory>
	#include <string>
	#include <version>
	#include <Windows.h>

	#include <openssl/evp.h>
	#include <openssl/md5.h>
	#include <openssl/types.h>
	#include <json/reader.h>
	#include <json/value.h>
	#include <json/writer.h>

namespace BaiduTranslateDLL
{
	class PasswordFunction final
	{
	public:
		explicit PasswordFunction(void) noexcept:
			m_path(GetLocalFilePath()),
			m_appid(R"()"),
			m_appkey(R"()"),
			m_ctx(EVP_MD_CTX_new())
		{
			if (m_ctx == nullptr)
			{
				GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_CTX_IS_NULL);
				GlobalErrorHandling::SetErrorTip("MD5 初始化失败在构造函数中。"s);

				m_init_is_no_error = false;

				return;
			}

			EVP_DigestInit_ex(m_ctx, EVP_md5(), nullptr);

			if (m_path.empty())
			{
				GlobalErrorHandling::SetLastError(
					ErrorCodeEnum::PASSWORD_FUNC_OPEN_LOCALAPPDATA_FAILED);
				GlobalErrorHandling::SetErrorTip("打开本地目录 AppData 失败在构造函数中。"s);

				m_init_is_no_error = false;

				return;
			}

			ReadLocalAppidAndAppkey(m_appid, m_appkey);

			m_init_is_no_error = true;
		}

		~PasswordFunction(void) noexcept
		{
			if (m_ctx != nullptr)
			{
				EVP_MD_CTX_free(m_ctx);
			}
		}

		template <_string_type StringType>
		_string GetMD5(StringType&& str) const noexcept
		{
			_STD array<unsigned char, MD5_DIGEST_LENGTH> md5 {};

			if constexpr (_STD is_same_v<_STD remove_cvref_t<StringType>, _string> ||
						  _STD is_same_v<_STD remove_cvref_t<StringType>, _wstring>)
			{
				EVP_DigestUpdate(m_ctx, str.c_str(), str.size());
			}
			else if constexpr (_STD is_same_v<_STD remove_cvref_t<StringType>, char*> ||
							   _STD is_same_v<_STD remove_cvref_t<StringType>, wchar_t*>)
			{
				EVP_DigestUpdate(m_ctx, str, _STD strlen(str));
			}
			else
			{
				return;
			}

			EVP_DigestFinal_ex(m_ctx, md5.data(), nullptr);

			_string result {};

			for (const auto& c: md5)
			{
				_STD format_to(_STD back_inserter(result), "{0:02x}", c);
			}

			return result;
		}

		_string GetAppID(void) const noexcept
		{
			return Decryption(m_appid);
		}

		_string GetAppKey(void) const noexcept
		{
			return Decryption(m_appkey);
		}

		template <_string_type StringType1, _string_type StringType2>
		void SetAppIDAndKey(StringType1&& appid, StringType2&& appkey) noexcept
		{
			if (!_STD filesystem::exists(m_path))
			{
				_STD ofstream ofs(m_path.string(), _STD ios::out);
				ofs.close();
			}

			_STD ofstream ofs(m_path.string());

			if (!ofs.is_open())
			{
				ofs.close();
				GlobalErrorHandling::SetLastError(
					ErrorCodeEnum::PASSWORD_FUNC_MAKING_LOCAL_FILE_FAILED);
				GlobalErrorHandling::SetErrorTip("创建本地文件失败在 SetAppIDAndKey 函数中。"s);
				return;
			}

			::Json::Value root {};
			root["app"s]["appid"s]	= Encryption(appid);
			root["app"s]["appkey"s] = Encryption(appkey);

			::Json::StreamWriterBuilder builder {};
			builder["commentStyle"s] = "None"s;
			builder["indentation"s]	 = "    "s;

			std::unique_ptr<::Json::StreamWriter> writer(builder.newStreamWriter());
			writer->write(root, &ofs);

			ofs.close();
		}

		static bool InitIsNoError(void) noexcept
		{
			return m_init_is_no_error;
		}

	private:
		static _STD filesystem::path GetLocalFilePath(void) noexcept
		{
			_STD filesystem::path data_path { L"" };

			_STD array<wchar_t, MAX_PATH> path { L'\0' };

			if (SHGetFolderPathW(nullptr,
								 CSIDL_APPDATA,
								 nullptr,
								 SHGFP_TYPE::SHGFP_TYPE_CURRENT,
								 path.data()) != S_OK)
			{
				return data_path;
			}

			_wstring appdata_path {};

			for (const auto& c: path)
			{
				if (c == L'\0')
				{
					break;
				}

				appdata_path += c;
			}

			data_path = _STD move(appdata_path + L"\\MyBaiduTranslate\\"s);

			if (!_STD filesystem::exists(data_path))
			{
				_STD filesystem::create_directory(data_path);
			}

			data_path += L"\\config.json"s;

			return data_path;
		}

		void ReadLocalAppidAndAppkey(_string& appid, _string& appkey) const noexcept
		{
			if (!_STD filesystem::exists(m_path))
			{
				_STD ofstream ofs(m_path.string(), _STD ios::out);
				ofs.close();

				appid  = ""s;
				appkey = ""s;

				return;
			}

			_STD ifstream ifs(m_path.string());

			if (!ifs.is_open())
			{
				ifs.close();
				GlobalErrorHandling::SetLastError(
					ErrorCodeEnum::PASSWORD_FUNC_OPEN_CONFIG_FILE_FAILED);
				GlobalErrorHandling::SetErrorTip(
					"打开配置文件失败在 ReadLocalAppidAndAppkey 函数中。"s);
				return;
			}

			_string str((_STD istreambuf_iterator<char>(ifs)), _STD istreambuf_iterator<char>());
			ifs.close();

			::Json::Value  root {};
			::Json::Reader reader {};

			if (!reader.parse(str, root))
			{
				GlobalErrorHandling::SetLastError(
					ErrorCodeEnum::PASSWORD_FUNC_OPEN_WITH_JSON_FAILED);
				GlobalErrorHandling::SetErrorTip(
					"无法以 JSON 格式读取配置文件在 ReadLocalAppidAndAppkey 函数中。"s);
				return;
			}

			appid  = _STD  move(Decryption(root["app"s]["appid"s].asString()));
			appkey = _STD move(Decryption(root["app"s]["appkey"s].asString()));
		}

		_string Encryption(const _string& str) const noexcept
		{
			return str;
		}

		_string Decryption(const _string& str) const noexcept
		{
			return str;
		}

	private:
		static inline bool m_init_is_no_error { false };

		_STD filesystem::path m_path {};

		_string				  m_appid {};
		_string				  m_appkey {};

		EVP_MD_CTX*			  m_ctx {};
	};
} // namespace BaiduTranslateDLL

#endif // !PASSWORDAPI
