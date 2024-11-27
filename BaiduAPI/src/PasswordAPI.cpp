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
#include <minwindef.h>
#include <version>
#include <winerror.h>

#include <openssl/evp.h>
#include <openssl/md5.h>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>

BaiduTranslateDLL::PasswordFunction::PasswordFunction(void) noexcept:
	m_path { GetLocalFilePath() },
	m_appid { R"()" },
	m_appkey { R"()" },
	m_ctx { EVP_MD_CTX_new() }
{
	if (m_ctx == nullptr)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_CTX_IS_NULL);
		GlobalErrorHandling::SetErrorTip(
			R"(MD5 initialization failed in PasswordFunction constructor.)");

		m_init_is_no_error = false;

		return;
	}

	EVP_DigestInit_ex(m_ctx, EVP_md5(), nullptr);

	if (m_path.empty())
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_OPEN_LOCALAPPDATA_FAILED);
		GlobalErrorHandling::SetErrorTip(
			R"(Failed to open the local directory MyAdata in the PasswordFunction constructor.)");

		m_init_is_no_error = false;

		return;
	}

	ReadLocalAppidAndAppkey();

	m_init_is_no_error = true;
}

BaiduTranslateDLL::PasswordFunction::~PasswordFunction(void) noexcept
{
	if (m_ctx != nullptr)
	{
		EVP_MD_CTX_free(m_ctx);
	}
}

_string BaiduTranslateDLL::PasswordFunction::GetMD5(_string_view str) const noexcept
{
	_STD array<unsigned char, MD5_DIGEST_LENGTH> md5 {};

	EVP_DigestUpdate(m_ctx, str.data(), str.size());

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

_string BaiduTranslateDLL::PasswordFunction::GetAppIDAndKey(void) const noexcept
{
	::Json::Value				root { ::Json::objectValue };
	_STD ostringstream			os {};

	root["appid"]  = _STD		 move(Encryption(m_appid));
	root["appkey"] = _STD		move(Encryption(m_appkey));

	::Json::StreamWriterBuilder builder {};
	builder["commentStyle"] = "None";
	builder["indentation"]	= "    ";

	_STD unique_ptr<::Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root, &os);

	return os.str();
}

void BaiduTranslateDLL::PasswordFunction::SetAppIDAndKey(_string_view appid,
														 _string_view appkey) noexcept
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
		GlobalErrorHandling::SetErrorTip(
			R"(Failed to create local file in PasswordFunction->SetAppIDAndKey function.)");

		return;
	}

	::Json::Value root { ::Json::objectValue };
	root["app"s]["appid"s]	= Encryption(appid);
	root["app"s]["appkey"s] = Encryption(appkey);

	::Json::StreamWriterBuilder builder {};
	builder["commentStyle"s] = "None"s;
	builder["indentation"s]	 = "    "s;

	std::unique_ptr<::Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root, &ofs);

	ofs.close();
}

bool BaiduTranslateDLL::PasswordFunction::InitIsNoError(void) noexcept
{
	return m_init_is_no_error;
}

_file_path BaiduTranslateDLL::PasswordFunction::GetLocalFilePath(void) noexcept
{
	_file_path data_path { L"" };

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

void BaiduTranslateDLL::PasswordFunction::ReadLocalAppidAndAppkey(void) noexcept
{
	if (!_STD filesystem::exists(m_path))
	{
		_STD ofstream ofs(m_path.string(), _STD ios::out);
		ofs.close();

		m_appid	 = _STD	 move(""s);
		m_appkey = _STD move(""s);

		return;
	}

	_STD ifstream ifs(m_path.string());

	if (!ifs.is_open())
	{
		ifs.close();

		GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_OPEN_CONFIG_FILE_FAILED);
		GlobalErrorHandling::SetErrorTip(R"(Failed to open configuration file in the )"
										 R"(PasswordFunction->ReadLocalAppidAndAppkey function.)");

		return;
	}

	_string str((_STD istreambuf_iterator<char>(ifs)), _STD istreambuf_iterator<char>());
	ifs.close();

	::Json::Value  root { ::Json::objectValue };
	::Json::Reader reader {};

	if (!reader.parse(str, root))
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::PASSWORD_FUNC_OPEN_WITH_JSON_FAILED);
		GlobalErrorHandling::SetErrorTip(
			R"(Unable to read configuration file in JSON format in the )"
			R"(PasswordFunction->ReadLocalAppidAndAppkey function.)");

		return;
	}

	m_appid	 = _STD	 move(Decryption(root["app"s]["appid"s].asString()));
	m_appkey = _STD move(Decryption(root["app"s]["appkey"s].asString()));
}

_string BaiduTranslateDLL::PasswordFunction::Encryption(_string_view str) const noexcept
{
	return _string(str);
}

_string BaiduTranslateDLL::PasswordFunction::Decryption(_string_view str) const noexcept
{
	return _string(str);
}
