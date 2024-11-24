#pragma once

#include "TypeDefined.h"

#include "GlobalError.h"
#include "PasswordAPI.h"

#include <ShlObj_core.h>
#include <type_traits>
#include <array>
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
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>

BaiduTranslateDLL::PasswordFunction::PasswordFunction(void) noexcept
{
	if (m_ctx != nullptr)
	{
		EVP_DigestInit_ex(m_ctx, EVP_md5(), nullptr);

		m_init_is_no_error = true;
	}
	else
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_CTX_IS_NULL);
		GlobalErrorHandling::SetErrorTip("MD5 初始化失败在构造函数中。");

		m_init_is_no_error = false;

		return;
	}

	m_path = _STD move(GetLocalFilePath());

	if (m_path.empty())
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_OPEN_LOCALAPPDATA_FAILED);
		GlobalErrorHandling::SetErrorTip("打开本地目录 AppData 失败在构造函数中。");

		m_init_is_no_error = false;

		return;
	}

	ReadLocalAppidAndAppkey(m_appid, m_appkey);
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

	_STD ofstream ofs(m_path.string());

	if (!ofs.is_open())
	{
		ofs.close();
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_MAKING_LOCAL_FILE_FAILED);
		GlobalErrorHandling::SetErrorTip("创建本地文件失败在 SetAppIDAndKey 函数中。");
		return;
	}

	::Json::Value root {};
	root["app"]["appid"]  = Encryption(appid);
	root["app"]["appkey"] = Encryption(appkey);

	::Json::StreamWriterBuilder builder {};
	builder["commentStyle"] = "None";
	builder["indentation"]	= "    ";

	std::unique_ptr<::Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root, &ofs);

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

	data_path = _STD move(appdata_path + L"\\MyBaiduTranslate\\");

	if (!_STD filesystem::exists(data_path))
	{
		_STD filesystem::create_directory(data_path);
	}

	data_path += L"\\config.json";

	return data_path;
}

void BaiduTranslateDLL::PasswordFunction::ReadLocalAppidAndAppkey(_string& appid,
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

	_STD ifstream ifs(m_path.string());

	if (!ifs.is_open())
	{
		ifs.close();
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_OPEN_CONFIG_FILE_FAILED);
		GlobalErrorHandling::SetErrorTip("打开配置文件失败在 ReadLocalAppidAndAppkey 函数中。");
		return;
	}

	_string str((_STD istreambuf_iterator<char>(ifs)), _STD istreambuf_iterator<char>());
	ifs.close();

	::Json::Value  root {};
	::Json::Reader reader {};

	if (!reader.parse(str, root))
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_OPEN_WITH_JSON_FAILED);
		GlobalErrorHandling::SetErrorTip(
			"无法以 JSON 格式读取配置文件在 ReadLocalAppidAndAppkey 函数中。");
		return;
	}

	appid  = _STD  move(Decryption(root["app"]["appid"].asString()));
	appkey = _STD move(Decryption(root["app"]["appkey"].asString()));
}

_string BaiduTranslateDLL::PasswordFunction::Encryption(const _string& str) const noexcept
{
	return str;
}

_string BaiduTranslateDLL::PasswordFunction::Decryption(const _string& str) const noexcept
{
	return str;
}
