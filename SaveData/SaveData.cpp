#pragma once

#include "pch.h"

#include "../ExceptionHandling/ExceptionHandling.h"
#include "SaveData.h"

#include <source_location>
#include <cstddef>
#include <exception>
#include <format>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <version>

#include <openssl/evp.h>
#include <openssl/md5.h>



#ifndef EXCEPTIONHADLING
	#define EXCEPTIONHADLING ::
#endif	// !EXCEPTIONHADLING



SaveData::SaveData(void) noexcept
{
	try
	{
		ctx = EVP_MD_CTX_new();

		if (ctx == nullptr)
		{
			const auto&			   source_location { _STD source_location::current() };

			throw EXCEPTIONHADLING AppIDError("EVP_MD_CTX_new() 错误",
											  source_location.file_name(),
											  source_location.function_name(),
											  source_location.line());
		}

		if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1)
		{
			EVP_MD_CTX_free(ctx);

			ctx = nullptr;

			const auto&			   source_location { _STD source_location::current() };

			throw EXCEPTIONHADLING MD5Error("EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) 返回值不为 1",
											source_location.file_name(),
											source_location.function_name(),
											source_location.line());
		}
	}
	catch (const _STD exception& e)
	{
		EXCEPTIONHADLING HandleException(e);
	}
}

SaveData::~SaveData(void) noexcept
{
	if (ctx != nullptr)
	{
		EVP_MD_CTX_free(ctx);
	}
}

_STD string SaveData::GetMD5(const _STD string& str) const noexcept
{
	try
	{
		return InterGetMD5(str);
	}
	catch (const _STD exception& e)
	{
		return EXCEPTIONHADLING HandleException(e);
	}
}

_NODISCARD bool SaveData::SaveDataToLocal(const _STD string& appid, const _STD string& appkey) const noexcept
{
	try
	{
		return InterSaveDataToLocal(appid, appkey);
	}
	catch (const _STD exception& e)
	{
		EXCEPTIONHADLING HandleException(e);

		return false;
	}
}

AppIDAndKey SaveData::GetDataFromLocal(void) const noexcept
{
	try
	{
		return InterGetDataFromLocal();
	}
	catch (const _STD exception& e)
	{
		EXCEPTIONHADLING HandleException(e);

		return { "", "" };
	}
}

_STD string SaveData::InterGetMD5(const _STD string& str) const noexcept(false)
{
	if (ctx == nullptr)
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING AppIDError("ctx 为空",
										  source_location.file_name(),
										  source_location.function_name(),
										  source_location.line());
	}

	if (EVP_DigestUpdate(ctx, str.c_str(), str.size()) != 1)
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING MD5Error("EVP_DigestUpdate(ctx, str.c_str(), str.size()) 不为 1",
										source_location.file_name(),
										source_location.function_name(),
										source_location.line());
	}

	_STD vector<unsigned char> hash(MD5_DIGEST_LENGTH, 0);

	if (unsigned int hash_len = 0; EVP_DigestFinal_ex(ctx, hash.data(), &hash_len) != 1)
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING MD5Error("EVP_DigestFinal_ex(ctx, hash.data(), &hash_len) 不为 1",
										source_location.file_name(),
										source_location.function_name(),
										source_location.line());
	}

	if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1)
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING MD5Error("EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) 不为 1",
										source_location.file_name(),
										source_location.function_name(),
										source_location.line());
	}

	_STD string result {};

	for (const auto& ch: hash)
	{
		_STD format_to(_STD back_inserter(result), "{:02x}", ch);
	}

	return result;
}

_STD string SaveData::InterEncryption(const _STD string& str) const noexcept
{
	_STD string encrypted {};

	for (const auto& ch: str)
	{
		_STD byte encryptedByte = static_cast<_STD byte>(ch) ^ static_cast<_STD byte>('x');

		encrypted.push_back(static_cast<char>(encryptedByte));
	}

	return encrypted;
}

_STD string SaveData::InterDecryption(const _STD string& str) const noexcept
{
	_STD string decrypted {};

	for (const auto& ch: str)
	{
		_STD byte decryptedByte = static_cast<_STD byte>(ch) ^ static_cast<_STD byte>('x');

		decrypted.push_back(static_cast<char>(decryptedByte));
	}

	return decrypted;
}

bool SaveData::InterSaveDataToLocal(const _STD string& appid, const _STD string& appkey) const noexcept(false)
{
	if (_STD ofstream file("./setting.ini", _STD ios::out | _STD ios::trunc); !file.is_open())
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING FileError("打开 setting.ini 错误",
										 source_location.file_name(),
										 source_location.function_name(),
										 source_location.line());
	}
	else
	{
		file << InterEncryption(appid) << _STD	endl;
		file << InterEncryption(appkey) << _STD endl;

		file.close();
	}

	return true;
}

AppIDAndKey SaveData::InterGetDataFromLocal() const noexcept(false)
{
	_STD string appid {};
	_STD string appkey {};

	if (_STD ifstream file("./setting.ini"); !file.is_open())
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING FileError("打开 setting.ini 错误",
										 source_location.file_name(),
										 source_location.function_name(),
										 source_location.line());
	}
	else
	{
		_STD getline(file, appid);
		_STD getline(file, appkey);

		file.close();
	}

	if (appid.empty() || appkey.empty())
	{
		const auto&			   source_location { _STD source_location::current() };

		throw EXCEPTIONHADLING FileError("读取 setting.ini 的内容为空",
										 source_location.file_name(),
										 source_location.function_name(),
										 source_location.line());
	}

	return { InterDecryption(appid), InterDecryption(appkey) };
}
