#pragma once

#include "pch.h"

#include "../SaveData/SaveData.h"
#include "__inter__AttributeDefinition.h"

#include <curl/curl.h>
#include <exception>
#include <memory>
#include <string>

class BaiduTranslate final
{
public:
	BAIDUTRANSLATE_API explicit BaiduTranslate(const _STD string& appid, const _STD string& appkey) noexcept;

	BAIDUTRANSLATE_API ~BaiduTranslate(void) noexcept;

	/// <summary>
	/// 设置百度翻译的 AppID 和 AppKey
	/// </summary>
	/// <param name="appid">你的百度翻译 AppID</param>
	/// <param name="appkey">与 AppID 对应的 key</param>
	/// <returns>返回一个 bool 值，表示身份验证是否成功</returns>
	BAIDUTRANSLATE_API _NODISCARD bool SetAppID(const _STD string& appid, const _STD string& appkey) noexcept;

	/// <summary>
	/// 翻译文本
	/// </summary>
	/// <param name="source">原文</param>
	/// <param name="from">原文语言类型</param>
	/// <param name="to">目标语言类型</param>
	/// <returns>翻译后的译文 或 错误信息</returns>
	BAIDUTRANSLATE_API _STD string Translate(const _STD string& source,
											 const _STD string& from,
											 const _STD string& to) noexcept;

private:
	TranslateInfoType TranslateInfo		= {};

	CURL*			  curl				= nullptr;

	_STD unique_ptr<SaveData> pSaveData = nullptr;

	// 写回调函数必须是静态的
	static _STD size_t WriteCallback(const char* data, _STD size_t size, _STD size_t nmemb, _STD string* userdata);

	_STD string		   SourceEncode(const _STD string& source) const;

	_STD string		   GetURL(void);

	_STD string		   GetErrorInfo(const _STD string& errorCode) const noexcept;

	_STD string		   InterTranslate(void);

	void			   HandleException(const _STD exception& e) const noexcept;
};
