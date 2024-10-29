#pragma once

#include "Baidu.h"
#include "CppStyle_BaiduAPI.h"

#include <type_traits>
#include <string>
#include <version>

using BaiduTranslate_Ptr = ::BaiduTranslateDLL::BaiduTranslateFunction*;

TranslateObj* BaiduTranslate_Init(CString appid, CString appkey)
{
	static auto p_translate_object = ::BaiduTranslateDLL::BaiduTranslateFunction(appid, appkey);

	if (!::BaiduTranslateDLL::BaiduTranslateFunction::IsInitSuccess())
	{
		return nullptr;
	}

	return (TranslateObj*)(&p_translate_object);
}

CString BaiduTranslate_Translate(TranslateObj* p_translate_object,
								 CString	   query,
								 CString	   from,
								 CString	   to,
								 CString	   appid,
								 CString	   appkey)
{
	if (p_translate_object == nullptr)
	{
		return nullptr;
	}

	static _STD string result {};

	result = _STD	   move(((BaiduTranslate_Ptr)p_translate_object)->Translate(query, from, to, appid, appkey));

	return result.c_str();
}

void BaiduTranslate_SetAppIDAndKey(TranslateObj* p_translate_object, CString appid, CString appkey)
{
	if (p_translate_object == nullptr)
	{
		return;
	}

	((BaiduTranslate_Ptr)p_translate_object)->SetAppIDAndKey(appid, appkey);
}

CString BaiduTranslate_GetAppIDAndKey(TranslateObj* p_translate_object)
{
	if (p_translate_object == nullptr)
	{
		return nullptr;
	}

	static _STD string result {};

	result = _STD	   move(((BaiduTranslate_Ptr)p_translate_object)->GetAppIDAndKey());

	return result.c_str();
}
