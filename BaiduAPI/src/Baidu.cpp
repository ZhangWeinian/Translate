#pragma once

#include "TypeDefined.h"

#include "Baidu.h"
#include "BaiduAPI.h"
#include "GlobalError.h"

#include <type_traits>
#include <mutex>
#include <string>
#include <version>

using ErrorCodeEnum		  = ::BaiduTranslateDLL::ErrorCodeEnum;
using GlobalErrorHandling = ::BaiduTranslateDLL::GlobalErrorHandling;

class TranslatePtr
{
public:
	static inline ::BaiduTranslateDLL::BaiduTranslateFunction* Ptr(const char* appid  = "",
																   const char* appkey = "")
	{
		static _STD once_flag once {};

		_STD				  call_once(once,
						[appid, appkey]()
						{
							static auto m_baidu_translate {
								::BaiduTranslateDLL::BaiduTranslateFunction::
									BaiduTranslateFunction(appid, appkey)
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

bool BaiduTranslate_Init(const char* appid, const char* appkey)
{
	if (TranslatePtr::Ptr(appid, appkey) != nullptr)
	{
		return true;
	}
	else
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::EXPORT_TRANSLATE_PTR_IS_NULL);
		GlobalErrorHandling::SetErrorTip(
			"公开头文件中的基指针为空。这发生在包装的初始化过程中，可能是 TranslatePtr "
			"实例化基指针时错误，也可能是 BaiduTranslateFunction 实例化过程中错误。");

		return false;
	}
}

const char* BaiduTranslate_Translate(const char* query,
									 const char* from,
									 const char* to,
									 const char* appid,
									 const char* appkey)
{
	static _string result {};

	if (TranslatePtr::Ptr() != nullptr)
	{
		result = _STD move(TranslatePtr::Ptr()->Translate(query, from, to, appid, appkey));
	}
	else
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::EXPORT_TRANSLATE_PTR_IS_NULL);
	}

	return result.c_str();
}

bool BaiduTranslate_SetAppIDAndKey(const char* appid, const char* appkey)
{
	if (TranslatePtr::Ptr() != nullptr)
	{
		TranslatePtr::Ptr()->SetAppIDAndKey(appid, appkey);
		return true;
	}
	else
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::EXPORT_TRANSLATE_PTR_IS_NULL);
		return false;
	}
}

const char* BaiduTranslate_GetAppIDAndKey(void)
{
	static _string result {};

	if (TranslatePtr::Ptr() != nullptr)
	{
		result = _STD move(TranslatePtr::Ptr()->GetAppIDAndKey());
	}
	else
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::EXPORT_TRANSLATE_PTR_IS_NULL);
	}

	return result.c_str();
}

const char* BaiduTranslate_GetLastError(void)
{
	static _string result {};

	if (TranslatePtr::Ptr() == nullptr)
	{
		GlobalErrorHandling::SetLastError(ErrorCodeEnum::EXPORT_TRANSLATE_PTR_IS_NULL);
	}

	result = _STD move(GlobalErrorHandling::GetErrorInfo());

	return result.c_str();
}
