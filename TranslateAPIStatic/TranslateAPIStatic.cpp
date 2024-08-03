#pragma once

#include "pch.h"

#include <version>

#if defined(_HAS_CXX20)

	#include <source_location>
	#include <type_traits>
	#include <format>
	#include <iterator>
	#include <memory>
	#include <string>

	#if defined(_TRANSLATEAPISTATIC_ITEM)

		#include "__inter__BDTranslate.h"
		#include "TranslateAPIStatic.h"

int BDTranslateStatic::test_add(int a, int b)
{
	return a + b;
}

void BDTranslateStatic::BDTranslate(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (m_pBaiduTranslateAPI = _STD make_unique<_INTERBDTRANSLATE __inter__BDTranslate>(appid, appkey);
		m_pBaiduTranslateAPI == nullptr)
	{
		const auto& source_location { _STD source_location::current() };

		m_message.clear();

		_STD format_to(_STD back_inserter(m_message),
					   "创建 __inter__BDTranslate 对象失败\n文件名：{}\n函数名：{}\n行号：{}\n",
					   source_location.file_name(),
					   source_location.function_name(),
					   source_location.line());

		m_isOK = false;

		return;
	}
	else
	{
		m_isOK	  = m_pBaiduTranslateAPI->isOK();
		m_message = _STD move(m_pBaiduTranslateAPI->whatHappened());
	}
}

bool BDTranslateStatic::SetAppID(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (!isOK())
	{
		return false;
	}

	return (m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->InterBaiduTranslateSetAppID(appid, appkey) : false;
}

const char*
	BDTranslateStatic::Translate(const _STD string& source, const _STD string& from, const _STD string& to) noexcept
{
	if (!isOK())
	{
		m_ans = _STD move((m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->whatHappened() : m_message);
	}
	else
	{
		m_ans =
			_STD move((m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->InterBaiduTranslate(source, from, to)
														: m_message);
	}

	return m_ans.c_str();
}

bool BDTranslateStatic::isOK(void) noexcept
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

const char* BDTranslateStatic::whatHappened(void) noexcept
{
	m_message = _STD move((m_pBaiduTranslateAPI != nullptr) ? m_pBaiduTranslateAPI->whatHappened() : m_message);

	return m_message.c_str();
}

	#endif	// defined(_TRANSLATEAPISTATIC_ITEM)

#endif		// defined(_HAS_CXX20)
