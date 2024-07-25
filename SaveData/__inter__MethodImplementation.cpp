#pragma once

#include "pch.h"
#include "SaveData.h"

#include <array>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>

#include <openssl/evp.h>

namespace Data::inline ERR_DEF
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

	void					HandleException(const _STD exception& e) noexcept
	{
		_STD cerr << _STD format("{}", e.what()) << _STD endl;
	}
}  // namespace Data::inline ERR_DEF

namespace Data
{
	_STD string GetMD5(const _STD string& str) noexcept(false)
	{
		_STD array<unsigned char, EVP_MAX_MD_SIZE> hash		= {};
		unsigned int							   hash_len = 0;

		auto									   ctx		= EVP_MD_CTX_new();

		if (ctx == nullptr)
		{
			throw MD5Error("EVP_MD_CTX_new error");
		}

		if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1)
		{
			throw MD5Error("EVP_DigestInit_ex error");
		}

		if (EVP_DigestUpdate(ctx, str.c_str(), str.size()) != 1)
		{
			throw MD5Error("EVP_DigestUpdate error");
		}

		if (EVP_DigestFinal_ex(ctx, hash.data(), &hash_len) != 1)
		{
			throw MD5Error("EVP_DigestFinal_ex error");
		}

		EVP_MD_CTX_free(ctx);

		_STD string result;
		for (unsigned int i = 0; i < hash_len; ++i)
		{
			result += _STD format("{:02x}", hash[i]);
		}

		return result;
	}

	_STD string Encryption(const _STD string& str)
	{
		_STD string encrypted {};
		for (const auto& ch: str)
		{
			encrypted.push_back(ch ^ 'x');
		}

		return encrypted;
	}

	_STD string Decryption(const _STD string& str)
	{
		_STD string decrypted {};
		for (const auto& ch: str)
		{
			decrypted.push_back(ch ^ 'x');
		}

		return decrypted;
	}

	_NODISCARD bool SaveData(const _STD string& appid, const _STD string& appkey) noexcept(false)
	{
		_STD ofstream file("./setting.ini", _STD ios::out | _STD ios::trunc);

		if (!file.is_open())
		{
			throw ERR_DEF::FileError("File open error");
		}

		file << Encryption(appid) << _STD  endl;
		file << Encryption(appkey) << _STD endl;

		file.close();

		return true;
	}

	AppIDAndKey GetData() noexcept(false)
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

		return { Decryption(appid), Decryption(appkey) };
	}
}  // namespace Data

_STD string GetMD5(const _STD string& str) noexcept
{
	try
	{
		return ::Data::GetMD5(str);
	}
	catch (const std::exception& e)
	{
		::Data::HandleException(e);

		return "";
	}
}

_NODISCARD bool SaveData(const _STD string& appid, const _STD string& appkey) noexcept
{
	try
	{
		return ::Data::SaveData(appid, appkey);
	}
	catch (const _STD exception& e)
	{
		::Data::HandleException(e);

		return false;
	}
}

AppIDAndKey GetData() noexcept
{
	return ::Data::GetData();
}
