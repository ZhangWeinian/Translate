#pragma once

#include "__inter__AttributeDefinition.h"
#include "pch.h"



/// <summary>
/// 获取字符串的 MD5 值
/// </summary>
/// <param name="str">目标字符串</param>
/// <returns>字符串的 MD5 值</returns>
SAVEDATA_API _STD string GetMD5(const _STD string&) noexcept;

/// <summary>
/// 保存数据到本地
/// </summary>
/// <param name="appid">与身份码关联的 AppID</param>
/// <param name="appkey">对应 AppID 的 key</param>
/// <param name="identifyingCode">唯一身份凭证码</param>
SAVEDATA_API _NODISCARD bool SaveData(const _STD string&, const _STD string&) noexcept;

/// <summary>
/// 获取本地数据
/// </summary>
/// <param name="dentifyingCode">唯一身份凭证码</param>
/// <returns>返回一个 pair ，分别对应与身份码关联的 AppID 和对应的 key</returns>
SAVEDATA_API AppIDAndKey GetData() noexcept;
