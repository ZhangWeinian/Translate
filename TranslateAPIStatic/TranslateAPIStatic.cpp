#pragma once

#include <type_traits>
#include <memory>
#include <string>
#include <version>

#include "../ExceptionHandling/ExceptionHandling.h"
#include "__inter__BaiduTranslateAPIStatic.h"
#include "TranslateAPIStatic.h"

#ifndef _EXCEPTIONHADLING
	#define _EXCEPTIONHADLING ::
#endif	// !_EXCEPTIONHADLING

BaiduTranslate::BaiduTranslate(const _STD string& appid, const _STD string& appkey) noexcept
{
	m_pBaiduTranslateAPI = new InterBaiduTranslateAPI(appid, appkey);

	if (m_pBaiduTranslateAPI == nullptr)
	{
		m_isOK	  = false;
		m_message = "创建 InterBaiduTranslateAPI 对象失败";
	}
	else
	{
		const auto* tmp = ((InterBaiduTranslateAPI*)m_pBaiduTranslateAPI)->whatHappened();

		m_isOK			= tmp->isOK;
		m_message		= tmp->message;
	}
}

BaiduTranslate::~BaiduTranslate(void) noexcept
{
	if (m_pBaiduTranslateAPI != nullptr)
	{
		delete (InterBaiduTranslateAPI*)m_pBaiduTranslateAPI;

		m_pBaiduTranslateAPI = nullptr;
	}
}

bool BaiduTranslate::SetAppID(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (!isOK())
	{
		return false;
	}

	return (m_pBaiduTranslateAPI != nullptr)
			   ? ((InterBaiduTranslateAPI*)m_pBaiduTranslateAPI)->InterBaiduTranslateSetAppID(appid, appkey)
			   : false;
}

_STD string BaiduTranslate::Translate(const _STD string& source,
									  const _STD string& from,
									  const _STD string& to) noexcept
{
	if (!isOK())
	{
		return (m_pBaiduTranslateAPI != nullptr)
				   ? ((InterBaiduTranslateAPI*)m_pBaiduTranslateAPI)->m_pBaiduTranslateAPIRuntimeStatus->message
				   : m_message;
	}

	return (m_pBaiduTranslateAPI != nullptr)
			   ? ((InterBaiduTranslateAPI*)m_pBaiduTranslateAPI)->InterBaiduTranslateTranslate(source, from, to)
			   : m_message;
}

bool BaiduTranslate::isOK(void) noexcept
{
	if (m_pBaiduTranslateAPI == nullptr)
	{
		m_message = _STD move(_STD string("InterBaiduTranslateAPI 对象为空"));

		return false;
	}

	if ((((InterBaiduTranslateAPI*)m_pBaiduTranslateAPI)->m_pBaiduTranslateAPIRuntimeStatus->isOK) && m_isOK)
	{
		return true;
	}
	else
	{
		m_message = ((InterBaiduTranslateAPI*)m_pBaiduTranslateAPI)->m_pBaiduTranslateAPIRuntimeStatus->message;

		return false;
	}
}

_STD string BaiduTranslate::whatHappened(void) noexcept
{
	return (m_pBaiduTranslateAPI != nullptr)
			   ? ((InterBaiduTranslateAPI*)m_pBaiduTranslateAPI)->m_pBaiduTranslateAPIRuntimeStatus->message
			   : m_message;
}
