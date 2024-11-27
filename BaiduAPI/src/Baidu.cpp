#pragma once

#include "TypeDefined.h"

#include "Baidu.h"
#include "BaiduAPI.h"
#include "GlobalError.h"

#include <type_traits>
#include <string>
#include <version>

using GlobalErrorHandling	 = ::BaiduTranslateDLL::GlobalErrorHandling;
using BaiduTranslateFunction = ::BaiduTranslateDLL::BaiduTranslateFunction;

bool BaiduTranslate_Init(void)
{
	// 注意：InitIsNoError() 方法必须要通过 BaiduTranslateFunction::GetInstance() 来调用
	// 而不是调用 BaiduTranslateFunction::InitIsNoError()
	return BaiduTranslateFunction::GetInstance().InitIsNoError();
}

const char*
	BaiduTranslate_Translate(const char* query, const char* from, const char* to, const char* appid, const char* appkey)
{
	static _string result {};
	result = _STD  move(BaiduTranslateFunction::GetInstance().Translate(query, from, to, appid, appkey));
	return result.c_str();
}

bool BaiduTranslate_SetAppIDAndKey(const char* appid, const char* appkey)
{
	return BaiduTranslateFunction::GetInstance().SetAppIDAndKey(appid, appkey);
}

const char* BaiduTranslate_GetAppIDAndKey(void)
{
	static _string result {};
	result = _STD  move(BaiduTranslateFunction::GetInstance().GetAppIDAndKey());
	return result.c_str();
}

const char* BaiduTranslate_GetLastError(void)
{
	static _string result {};
	result = _STD  move(GlobalErrorHandling::GetErrorInfo());
	return result.c_str();
}
