#pragma once

#include <type_traits>
#include <cstddef>
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

#include "__inter__AttributeDefinition.h"
#include "SaveData.h"

SaveData::SaveData(void) noexcept
{
	m_Ctx = EVP_MD_CTX_new();

	if (m_Ctx == nullptr)
	{
		return;
	}

	if (const auto& init_ex = EVP_DigestInit_ex(m_Ctx, EVP_md5(), nullptr); init_ex != 1)
	{
		EVP_MD_CTX_free(m_Ctx);

		m_Ctx = nullptr;

		return;
	}
}

SaveData::~SaveData(void) noexcept
{
	if (m_Ctx != nullptr)
	{
		EVP_MD_CTX_free(m_Ctx);
	}
}

_STD string SaveData::GetMD5(const _STD string& str) noexcept
{
	if ((m_Ctx == nullptr) || (EVP_DigestUpdate(m_Ctx, str.c_str(), str.size()) != 1))
	{
		return "";
	}

	_STD vector<unsigned char> hash(MD5_DIGEST_LENGTH, 0);

	if (unsigned int hash_len = 0;
		(EVP_DigestFinal_ex(m_Ctx, hash.data(), &hash_len) != 1) || (EVP_DigestInit_ex(m_Ctx, EVP_md5(), nullptr) != 1))
	{
		return "";
	}

	_STD string result {};

	for (const auto& ch: hash)
	{
		_STD format_to(_STD back_inserter(result), "{:02x}", ch);
	}

	return result;
}

_NODISCARD bool SaveData::SaveDataToLocal(const _STD string& appid, const _STD string& appkey) noexcept
{
	_STD string InterEncryptionAppID { _STD move(InterEncryption(appid)) };
	_STD string InterEncryptionAppKey { _STD move(InterEncryption(appkey)) };

	if ((InterEncryptionAppID == m_InterEncryptionAppID) && (InterEncryptionAppKey == m_InterEncryptionAppKey))
	{
		return true;
	}

	if (_STD ofstream file("./setting.ini", _STD ios::out | _STD ios::trunc); !file.is_open())
	{
		return false;
	}
	else
	{
		m_InterEncryptionAppID	= InterEncryptionAppID;
		m_InterEncryptionAppKey = InterEncryptionAppKey;

		file << InterEncryptionAppID << _STD  endl;
		file << InterEncryptionAppKey << _STD endl;

		file.close();
	}

	return true;
}

AppIDAndKey SaveData::GetDataFromLocal(void) noexcept
{
	if (!m_InterEncryptionAppID.empty() && !m_InterEncryptionAppKey.empty())
	{
		return { InterDecryption(m_InterEncryptionAppID), InterDecryption(m_InterEncryptionAppKey) };
	}

	if (_STD ifstream file("./setting.ini"); !file.is_open())
	{
		return { "", "" };
	}
	else
	{
		_STD getline(file, m_InterEncryptionAppID);
		_STD getline(file, m_InterEncryptionAppKey);

		file.close();
	}

	if (!m_InterEncryptionAppID.empty() && !m_InterEncryptionAppKey.empty())
	{
		return { InterDecryption(m_InterEncryptionAppID), InterDecryption(m_InterEncryptionAppKey) };
	}
	else
	{
		return { "", "" };
	}
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
