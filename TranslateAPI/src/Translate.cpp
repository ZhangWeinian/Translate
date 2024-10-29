#pragma once

#include "Translate.h"
#include "./Baidu/BaiduAPI.h"

#include <type_traits>
#include <string>
#include <version>

using CPPBaiduTranslateFunction = ::CPPBaiduTranslateDLL::CPPBaiduTranslateFunction;

TranslateFunction* InitTranslateFunction(CString appid, CString appkey)
{
	static auto p_translate_function = CPPBaiduTranslateFunction(appid, appkey);

	if (!CPPBaiduTranslateFunction::IsInitSuccess())
	{
		return nullptr;
	}

	return (TranslateFunction*)(&p_translate_function);
}

CString Translate(TranslateFunction* p_translate_function,
				  CString			 query,
				  CString			 from,
				  CString			 to,
				  CString			 appid,
				  CString			 appkey)
{
	if (p_translate_function == nullptr)
	{
		return nullptr;
	}

	static _STD string result {};

	result = _STD move(((CPPBaiduTranslateFunction*)p_translate_function)->Translate(query, from, to, appid, appkey));

	return result.c_str();
}

void SetAppIDAndKey(TranslateFunction* p_translate_function, CString appid, CString appkey)
{
	if (p_translate_function == nullptr)
	{
		return;
	}

	((CPPBaiduTranslateFunction*)p_translate_function)->SetAppIDAndKey(appid, appkey);
}

CString GetAppIDAndKey(TranslateFunction* p_translate_function)
{
	if (p_translate_function == nullptr)
	{
		return nullptr;
	}

	static _STD string result {};

	result = _STD	   move(((CPPBaiduTranslateFunction*)p_translate_function)->GetAppIDAndKey());

	return result.c_str();
}
