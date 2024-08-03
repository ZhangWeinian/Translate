#pragma once

#include "pch.h"

#include <version>

#if defined(_HAS_CXX20)

	#include <type_traits>
	#include <memory>
	#include <string>

	#if defined(_TRANSLATEAPI_ITEM)

		#include "__inter__BDTranslate.h"
		#include "TranslateAPI.h"

BDTranslateDll::BDTranslateDll(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (m_pBaiduTranslateAPI = _STD make_unique<_INTERBDTRANSLATE __inter__BDTranslate>(appid, appkey);
		m_pBaiduTranslateAPI == nullptr)
	{
		m_isOK	  = false;
		m_message = "创建 __inter__BDTranslate 对象失败";
	}
	else
	{
		m_isOK	  = m_pBaiduTranslateAPI->isOK();
		m_message = _STD move(m_pBaiduTranslateAPI->whatHappened());
	}
}

bool BDTranslateDll::SetAppID(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (!isOK())
	{
		return false;
	}

	return (m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->InterBaiduTranslateSetAppID(appid, appkey) : false;
}

const char*
	BDTranslateDll::Translate(const _STD string& source, const _STD string& from, const _STD string& to) noexcept
{
	if (!isOK())
	{
		m_message = _STD move((m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->whatHappened() : m_message);

		return m_message.c_str();
	}
	else
	{
		m_ans =
			_STD move((m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->InterBaiduTranslate(source, from, to)
														: m_message);

		return m_ans.c_str();
	}
}

bool BDTranslateDll::isOK(void) noexcept
{
	m_message.clear();

	if (m_pBaiduTranslateAPI == nullptr)
	{
		m_message = _STD move(_STD string("__inter__BDTranslate 对象为空"));

		return false;
	}

	if ((m_pBaiduTranslateAPI->isOK()) && m_isOK)
	{
		return true;
	}
	else
	{
		m_message = _STD move(m_pBaiduTranslateAPI->whatHappened());

		return false;
	}
}

const char* BDTranslateDll::whatHappened(void) noexcept
{
	m_message = _STD move((m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->whatHappened() : m_message);

	return m_message.c_str();
}
	#endif	// defined(_TRANSLATEAPI_ITEM)

#endif		// defined(_HAS_CXX20)
