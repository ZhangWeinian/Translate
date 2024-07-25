#pragma once

#include "pch.h"

#include "SaveData.h"

#include <array>
#include <cstddef>
#include <exception>
#include <format>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <iterator>
#include <string>
#include <version>

#include <openssl/evp.h>

namespace ERR_DEF
{
	class FileError final: public _STD exception
	{
	public:
		explicit FileError(const char* msg): _STD exception(msg) {}
	};

	class MD5Error final: public _STD exception
	{
	public:
		explicit MD5Error(const char* msg): _STD exception(msg) {}
	};

	class IDError final: public _STD exception
	{
	public:
		explicit IDError(const char* msg): _STD exception(msg) {}
	};

	using OtherError = _STD exception;
}  // namespace ERR_DEF

SaveData::SaveData(void) noexcept
{
	try
	{
		ctx = EVP_MD_CTX_new();

		if (ctx == nullptr)
		{
			throw ERR_DEF::IDError("EVP_MD_CTX_new error");
		}

		if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1)
		{
			EVP_MD_CTX_free(ctx);

			ctx = nullptr;

			throw ::ERR_DEF::MD5Error("EVP_DigestInit_ex error");
		}
	}
	catch (const _STD exception& e)
	{
		HandleException(e);
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
		HandleException(e);

		return "";
	}
}

_NODISCARD bool SaveData::SaveDataToLocal(const _STD string& appid, const _STD string& appkey) const noexcept
{
	try
	{
		return InterSaveData(appid, appkey);
	}
	catch (const _STD exception& e)
	{
		HandleException(e);

		return false;
	}
}

AppIDAndKey SaveData::GetDataFromLocal() const noexcept
{
	try
	{
		return InterGetData();
	}
	catch (const _STD exception& e)
	{
		HandleException(e);

		return { "", "" };
	}
}

_STD string SaveData::InterGetMD5(const _STD string& str) const noexcept(false)
{
	if (ctx == nullptr)
	{
		throw ERR_DEF::IDError("EVP_MD_CTX_new error");
	}

	if (EVP_DigestUpdate(ctx, str.c_str(), str.size()) != 1)
	{
		throw ::ERR_DEF::MD5Error("EVP_DigestUpdate error");
	}

	_STD array<unsigned char, EVP_MAX_MD_SIZE> hash = {};

	if (unsigned int hash_len = 0; EVP_DigestFinal_ex(ctx, hash.data(), &hash_len) != 1)
	{
		throw ::ERR_DEF::MD5Error("EVP_DigestFinal_ex error");
	}

	if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1)
	{
		throw ::ERR_DEF::MD5Error("EVP_DigestInit_ex error");
	}

	_STD string result;

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

bool SaveData::InterSaveData(const _STD string& appid, const _STD string& appkey) const noexcept(false)
{
	_STD ofstream file("./setting.ini", _STD ios::out | _STD ios::trunc);

	if (!file.is_open())
	{
		throw ERR_DEF::FileError("File open error");
	}

	file << InterEncryption(appid) << _STD	endl;
	file << InterEncryption(appkey) << _STD endl;

	file.close();

	return true;
}

AppIDAndKey SaveData::InterGetData() const noexcept(false)
{
	_STD ifstream file("./setting.ini");

	if (!file.is_open())
	{
		throw ERR_DEF::FileError("File open error");
	}

	_STD string appid;
	_STD string appkey;

	_STD		getline(file, appid);
	_STD		getline(file, appkey);

	file.close();

	return { InterDecryption(appid), InterDecryption(appkey) };
}

void SaveData::HandleException(const _STD exception& e) const noexcept
{
	_STD format_to(_STD ostream_iterator<char>(_STD cerr), "{}\n", e.what());
}
