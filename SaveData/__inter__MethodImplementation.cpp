#pragma once

#include "pch.h"
#include "SaveData.h"

#include <array>
#include <exception>
#include <format>
#include <iostream>

#include <openssl/evp.h>

namespace Data::inline ERR_DEF
{
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

	_STD string GetIdentifyingCode(const _STD string& userName) noexcept(false)
	{
		return userName;
	}

	_NODISCARD bool SaveData(const _STD string& identifyingCode, const _STD string& appid, const _STD string& appkey)
		noexcept(false)
	{
		return false;
	}

	info GetData(const _STD string& dentifyingCode) noexcept(false)
	{
		return { "appid--", "appkey++" };
	}
}  // namespace Data

/// <summary>
/// 获取字符串的 MD5 值
/// </summary>
/// <param name="str">目标字符串</param>
/// <returns>字符串的 MD5 值</returns>
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

/// <summary>
/// 生成与你 userName 关联的唯一身份凭证码
/// </summary>
/// <param name="userName">用户名，建议使用 AppID</param>
/// <returns>返回身份码</returns>
_STD string GetIdentifyingCode(const _STD string& userName) noexcept
{
	return ::Data::GetIdentifyingCode(userName);
}

/// <summary>
/// 保存数据到本地
/// </summary>
/// <param name="identifyingCode">唯一身份凭证码</param>
/// <param name="appid">与身份码关联的 AppID</param>
/// <param name="appkey">对应 AppID 的 key</param>
_NODISCARD bool
	SaveData(const _STD string& identifyingCode, const _STD string& appid, const _STD string& appkey) noexcept
{
	try
	{
		if (identifyingCode != GetIdentifyingCode(appid))
		{
			throw ::Data::IDError("ID error");
		}

		return ::Data::SaveData(identifyingCode, appid, appkey);
	}
	catch (const _STD exception& e)
	{
		::Data::HandleException(e);

		return false;
	}
}

/// <summary>
/// 获取本地数据
/// </summary>
/// <param name="dentifyingCode">唯一身份凭证码</param>
/// <returns>返回一个 pair ，分别对应与身份码关联的 AppID 和对应的 key</returns>
info GetData(const _STD string& dentifyingCode) noexcept
{
	return ::Data::GetData(dentifyingCode);
}
