#pragma once

#include <type_traits>
#include <memory>
#include <string>
#include <version>

#include "__inter__BDTranslate.h"
#include "TranslateAPI.h"

#ifndef _EXCEPTIONHADLING
	#define _EXCEPTIONHADLING ::
#endif	// !_EXCEPTIONHADLING

BDTranslate::BDTranslate(const _STD string& appid, const _STD string& appkey) noexcept
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

bool BDTranslate::SetAppID(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (!isOK())
	{
		return false;
	}

	return (m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->InterBaiduTranslateSetAppID(appid, appkey) : false;
}

const char* BDTranslate::Translate(const _STD string& source, const _STD string& from, const _STD string& to) noexcept
{
	if (!isOK())
	{
		m_ans = _STD move((m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->whatHappened() : m_message);
	}
	else
	{
		m_ans = _STD move((m_pBaiduTranslateAPI != nullptr)
							  ? m_pBaiduTranslateAPI->InterBaiduTranslateTranslate(source, from, to)
							  : m_message);
	}

	return m_ans.c_str();
}

bool BDTranslate::isOK(void) noexcept
{
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

const char* BDTranslate::whatHappened(void) noexcept
{
	m_message = _STD move((m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->whatHappened() : m_message);

	return m_message.c_str();
}
