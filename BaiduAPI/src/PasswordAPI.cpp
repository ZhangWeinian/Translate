#pragma once

#include "TypeDefined.h"

#include "GlobalError.h"
#include "PasswordAPI.h"

#include <ShlObj_core.h>
#include <type_traits>
#include <array>
#include <cctype>
#include <combaseapi.h>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <iosfwd>
#include <iterator>
#include <ostream>
#include <string>
#include <version>
#include <Windows.h>

#include <openssl/evp.h>
#include <openssl/md5.h>
#include <ini.h>

BaiduTranslateDLL::PasswordFunction::PasswordFunction(void) noexcept
{
	if (m_ctx != nullptr)
	{
		EVP_DigestInit_ex(m_ctx, EVP_md5(), nullptr);

		m_init_is_no_error = true;
	}
	else
	{
		ErrorHandling::SetLastError(PASSWORD_FUNC_CTX_IS_NULL);
		ErrorHandling::SetErrorTip("MD5 初始化失败在构造函数中。");

		m_init_is_no_error = false;

		return;
	}

	m_path = _STD move(GetLocalFilePath());

	if (m_path.empty())
	{
		ErrorHandling::SetLastError(PASSWORD_FUNC_OPEN_LOCALAPPDATA_FAILED);
		ErrorHandling::SetErrorTip("打开本地目录 AppData 失败在构造函数中。");

		m_init_is_no_error = false;

		return;
	}

	GetLocalAppIDAndKey(m_appid, m_appkey);
}

BaiduTranslateDLL::PasswordFunction::~PasswordFunction(void) noexcept
{
	if (m_ctx != nullptr)
	{
		EVP_MD_CTX_free(m_ctx);
	}
}

_string BaiduTranslateDLL::PasswordFunction::GetMD5(const _string& str) const noexcept
{
	_STD array<unsigned char, MD5_DIGEST_LENGTH> md5 {};

	EVP_DigestUpdate(m_ctx, str.c_str(), str.size());
	EVP_DigestFinal_ex(m_ctx, md5.data(), nullptr);

	_string result {};

	for (const auto& c: md5)
	{
		_STD format_to(_STD back_inserter(result), "{0:02x}", c);
	}

	return result;
}

_string BaiduTranslateDLL::PasswordFunction::GetAppID(void) const noexcept
{
	return Decryption(m_appid);
}

_string BaiduTranslateDLL::PasswordFunction::GetAppKey(void) const noexcept
{
	return Decryption(m_appkey);
}

void BaiduTranslateDLL::PasswordFunction::SetAppIDAndKey(const _string& appid,
														 const _string& appkey) noexcept
{
	if (!_STD filesystem::exists(m_path))
	{
		_STD ofstream ofs(m_path.string(), _STD ios::out);
		ofs.close();
	}

	_STD ofstream ofs(m_path.string(), _STD ios::out);

	if (!ofs.is_open())
	{
		ofs.close();
		ErrorHandling::SetLastError(PASSWORD_FUNC_MAKING_LOCAL_FILE_FAILED);
		ErrorHandling::SetErrorTip("创建本地文件失败在 SetAppIDAndKey 函数中。");
		return;
	}

	ofs << "[app]" << _STD							endl;
	ofs << "[appid]" << Encryption(appid) << _STD	endl;
	ofs << "[appkey]" << Encryption(appkey) << _STD endl;

	ofs.close();
}

bool BaiduTranslateDLL::PasswordFunction::InitIsNoError(void) noexcept
{
	return m_init_is_no_error;
}

_STD filesystem::path BaiduTranslateDLL::PasswordFunction::GetLocalFilePath(void) noexcept
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

	data_path = _STD move(appdata_path + L"\\MyBaiduTranslate\\local.data");

	return data_path;
}

void BaiduTranslateDLL::PasswordFunction::GetLocalAppIDAndKey(_string& appid,
															  _string& appkey) noexcept
{
	if (!_STD filesystem::exists(m_path))
	{
		_STD ofstream ofs(m_path.string(), _STD ios::out);
		ofs.close();

		appid  = "";
		appkey = "";

		return;
	}

	if (ini_parse(m_path.string().c_str(),
				  BaiduTranslateDLL::PasswordFunction::InihReadHandler,
				  &m_app_config) < 0)
	{
		appid  = "";
		appkey = "";

		return;
	}

	appid = _STD move(m_app_config.appid);
	m_app_config.appid.clear();

	appkey = _STD move(m_app_config.appkey);
	m_app_config.appkey.clear();
}

_string BaiduTranslateDLL::PasswordFunction::Encryption(const _string& str) const noexcept
{
	return str;
}

_string BaiduTranslateDLL::PasswordFunction::Decryption(const _string& str) const noexcept
{
	return str;
}

int BaiduTranslateDLL::PasswordFunction::
	InihReadHandler(void* user, const char* section, const char* name, const char* value) noexcept
{
	auto* config = static_cast<app_config*>(user);

	if (section == nullptr)
	{
		config->appkey = "";
		config->appid  = "";

		return -1;
	}

	if (_STD strcmp(section, "app") != 0)
	{
		config->appkey = "";
		config->appid  = "";

		return -1;
	}

	if (name != nullptr && value != nullptr)
	{
		if (_STD strcmp(name, "appid") == 0)
		{
			config->appid = value;
		}
		else if (_STD strcmp(name, "appkey") == 0)
		{
			config->appkey = value;
		}
	}

	return 1;
}
