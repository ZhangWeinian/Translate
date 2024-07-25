#pragma once

#include "pch.h"

#include "__inter__AttributeDefinition.h"

#include <string>

#include <openssl/types.h>

class SaveData final
{
public:
	SAVEDATA_API explicit SaveData(void) noexcept;

	SAVEDATA_API ~SaveData(void) noexcept;

	/// <summary>
	/// 获取字符串的 MD5 值
	/// </summary>
	/// <param name="str">目标字符串</param>
	/// <returns>字符串的 MD5 值</returns>
	SAVEDATA_API _STD string GetMD5(const _STD string& str) const noexcept;

	/// <summary>
	/// 保存数据到本地
	/// </summary>
	/// <param name="appid">与身份码关联的 AppID</param>
	/// <param name="appkey">对应 AppID 的 key</param>
	/// <param name="identifyingCode">唯一身份凭证码</param>
	SAVEDATA_API _NODISCARD bool SaveDataToLocal(const _STD string&, const _STD string&) const noexcept;

	/// <summary>
	/// 获取本地数据
	/// </summary>
	/// <param name="dentifyingCode">唯一身份凭证码</param>
	/// <returns>返回一个 pair ，分别对应与身份码关联的 AppID 和对应的 key</returns>
	SAVEDATA_API AppIDAndKey GetDataFromLocal() const noexcept;

private:
	EVP_MD_CTX*		ctx = nullptr;

	_STD string		InterGetMD5(const _STD string& str) const noexcept(false);

	_STD string		InterEncryption(const _STD string& str) const noexcept;

	_STD string		InterDecryption(const _STD string& str) const noexcept;

	_NODISCARD bool InterSaveData(const _STD string& appid, const _STD string& appkey) const noexcept(false);

	AppIDAndKey		InterGetData() const noexcept(false);

	void			HandleException(const _STD exception& e) const noexcept;
};
