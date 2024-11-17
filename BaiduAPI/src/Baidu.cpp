#pragma once

#include "Baidu.h"
#include "BaiduAPI.h"

#include <type_traits>
#include <mutex>
#include <string>
#include <version>

class TranslatePtr
{
public:
	static inline ::BaiduTranslateDLL::BaiduTranslateFunction* Ptr(void)
	{
		static _STD once_flag once {};

		_STD				  call_once(once,
						[]()
						{
							static auto m_baidu_translate {
								::BaiduTranslateDLL::BaiduTranslateFunction::BaiduTranslateFunction()
							};

							if (::BaiduTranslateDLL::BaiduTranslateFunction::InitIsNoError())
							{
								m_baidu_translate_ptr = &m_baidu_translate;
							}
							else
							{
								m_baidu_translate_ptr = nullptr;
							}
						});

		return m_baidu_translate_ptr;
	}


private:
	static inline ::BaiduTranslateDLL::BaiduTranslateFunction* m_baidu_translate_ptr { nullptr };
};

CBool BaiduTranslate_Init(CString appid, CString appkey)
{
	if (TranslatePtr::Ptr() != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CString
	BaiduTranslate_Translate(CString query, CString from, CString to, CString appid, CString appkey)
{
	static _STD string result {};

	if (TranslatePtr::Ptr() != nullptr)
	{
		result = _STD move(TranslatePtr::Ptr()->Translate(query, from, to, appid, appkey).c_str());
	}

	return result.c_str();
}

CBool BaiduTranslate_SetAppIDAndKey(CString appid, CString appkey)
{
	if (TranslatePtr::Ptr() != nullptr)
	{
		TranslatePtr::Ptr()->SetAppIDAndKey(appid, appkey);

		return true;
	}
	else
	{
		return false;
	}
}

CString BaiduTranslate_GetAppIDAndKey(void)
{
	static _STD string result {};

	if (TranslatePtr::Ptr() != nullptr)
	{
		result = _STD move(TranslatePtr::Ptr()->GetAppIDAndKey().c_str());
	}

	return result.c_str();
}
