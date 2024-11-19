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

C_Bool BaiduTranslate_Init(C_String appid, C_String appkey)
{
	if (TranslatePtr::Ptr() != nullptr)
	{
		return true;
	}
	else
	{
		::BaiduTranslateDLL::BaiduTranslateDLLError::SetLastError(
			::BaiduTranslateDLL::ErrorCodeEnum::TRANSLATEPTR_PTR_IS_NULL);

		::BaiduTranslateDLL::BaiduTranslateDLLError::SetErrorTip(
			"公开头文件中的基指针为空。这发生在包装的初始化过程中，可能是 TranslatePtr "
			"实例化基指针时错误，也可能是 BaiduTranslateFunction 实例化过程中错误。");

		return false;
	}
}

C_String BaiduTranslate_Translate(C_String query,
								  C_String from,
								  C_String to,
								  C_String appid,
								  C_String appkey)
{
	static _STD string result {};

	if (TranslatePtr::Ptr() != nullptr)
	{
		result = _STD move(TranslatePtr::Ptr()->Translate(query, from, to, appid, appkey));
	}
	else
	{
		::BaiduTranslateDLL::BaiduTranslateDLLError::SetLastError(
			::BaiduTranslateDLL::ErrorCodeEnum::TRANSLATEPTR_PTR_IS_NULL);
	}


	return result.c_str();
}

C_Bool BaiduTranslate_SetAppIDAndKey(C_String appid, C_String appkey)
{
	if (TranslatePtr::Ptr() != nullptr)
	{
		TranslatePtr::Ptr()->SetAppIDAndKey(appid, appkey);
		return true;
	}
	else
	{
		::BaiduTranslateDLL::BaiduTranslateDLLError::SetLastError(
			::BaiduTranslateDLL::ErrorCodeEnum::TRANSLATEPTR_PTR_IS_NULL);

		return false;
	}
}

C_String BaiduTranslate_GetAppIDAndKey(void)
{
	static _STD string result {};

	if (TranslatePtr::Ptr() != nullptr)
	{
		result = _STD move(TranslatePtr::Ptr()->GetAppIDAndKey());
	}
	else
	{
		::BaiduTranslateDLL::BaiduTranslateDLLError::SetLastError(
			::BaiduTranslateDLL::ErrorCodeEnum::TRANSLATEPTR_PTR_IS_NULL);
	}

	return result.c_str();
}

C_String BaiduTranslate_GetLastError(void)
{
	static _STD string result {};

	if (TranslatePtr::Ptr() == nullptr)
	{
		::BaiduTranslateDLL::BaiduTranslateDLLError::SetLastError(
			::BaiduTranslateDLL::ErrorCodeEnum::TRANSLATEPTR_PTR_IS_NULL);
	}

	result = _STD move(::BaiduTranslateDLL::BaiduTranslateDLLError::GetErrorInfo());

	return result.c_str();
}
