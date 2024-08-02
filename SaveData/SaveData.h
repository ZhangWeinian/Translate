#pragma once

/*
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
* 此文件中大量使用了 C++20 的新特性，因此使用此文件之前请确保你的编译器支持并开启 C++20
*/

#include <version>

#if defined(_HAS_CXX20)

	#include "__inter__AttributeDefinition.h"

	#include <string>

	#include <openssl/types.h>

/// <summary>
/// 处理数据的类，包括加密、解密、保存、读取、获取 md5 值等操作
/// </summary>
class SaveData final
{
public:

	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>无返回值</returns>
	SAVEDATA_API explicit SaveData(void) noexcept;

	/// <summary>
	/// 析构函数
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>无返回值</returns>
	SAVEDATA_API ~SaveData(void) noexcept;

	/// <summary>
	/// 获取字符串的 MD5 值
	/// </summary>
	/// <param name="str">目标字符串</param>
	/// <returns>字符串的 MD5 值</returns>
	SAVEDATA_API _STD string GetMD5(const _STD string& str) noexcept;

	/// <summary>
	/// 保存数据到本地
	/// </summary>
	/// <param name="appid">要保存的 AppID</param>
	/// <param name="appkey">要保存的 key</param>
	/// <returns>是否保存成功</returns>
	SAVEDATA_API _NODISCARD bool SaveDataToLocal(const _STD string& appid, const _STD string& appkey) noexcept;

	/// <summary>
	/// 获取本地数据
	/// </summary>
	/// <param name="">无参数</param>
	/// <returns>返回一个包含 AppID 和 key 的结构体</returns>
	SAVEDATA_API AppIDAndKey GetDataFromLocal(void) noexcept;

private:

	/// <summary>
	/// 用于保存加密后的 AppID ，避免重复加密和反复读写
	/// </summary>
	_STD string m_InterEncryptionAppID { "" };

	/// <summary>
	/// 用于保存加密后的 AppKey ，避免重复加密和反复读写
	/// </summary>
	_STD string m_InterEncryptionAppKey { "" };

	/// <summary>
	/// 一个指向 EVP_MD_CTX 的指针，用于获取 MD5 值，避免重复创建和销毁
	/// </summary>
	EVP_MD_CTX* m_Ctx {};

	/// <summary>
	/// 此函数仅供内部使用，加密字符串
	/// </summary>
	/// <param name="str">源字符串</param>
	/// <returns>加密后的字符串</returns>
	_STD string InterEncryption(const _STD string& str) const noexcept;

	/// <summary>
	/// 此函数仅供内部使用，解密字符串
	/// </summary>
	/// <param name="str">源字符串</param>
	/// <returns>解密后的字符串</returns>
	_STD string InterDecryption(const _STD string& str) const noexcept;
};

#endif	// defined(_HAS_CXX20)
