#pragma once

#include "pch.h"

#include <memory>
#include <string>
#include <version>

#include "InterBaiduTranslateAPI.h"
#include "TranslateAPI.h"

BaiduTranslate::BaiduTranslate(const _STD string& appid, const _STD string& appkey) noexcept
{
	m_pBaiduTranslateAPI = _STD make_unique<BaiduTranslateAPI>(appid, appkey);

	if (m_pBaiduTranslateAPI == nullptr)
	{
		m_pBaiduTranslateRuntimeStatus			= _STD make_unique<BaiduTranslateRuntimeStatus>();

		m_pBaiduTranslateRuntimeStatus->isOK	= false;
		m_pBaiduTranslateRuntimeStatus->message = "创建 BaiduTranslateAPI 对象失败";
	}
	else
	{
		const auto* tmp							= m_pBaiduTranslateAPI->whatHappened();

		m_pBaiduTranslateRuntimeStatus->isOK	= tmp->isOK;
		m_pBaiduTranslateRuntimeStatus->message = tmp->message;
	}
}

bool BaiduTranslate::SetAppID(const _STD string& appid, const _STD string& appkey) noexcept
{
	if (!isOK())
	{
		return false;
	}

	return m_pBaiduTranslateAPI->BaiduTranslateSetAppID(appid, appkey);
}

_STD string BaiduTranslate::Translate(const _STD string& source,
									  const _STD string& from,
									  const _STD string& to) noexcept
{
	if (!isOK())
	{
		return m_pBaiduTranslateRuntimeStatus->message;
	}

	return m_pBaiduTranslateAPI->BaiduTranslateTranslate(source, from, to);
}

bool BaiduTranslate::isOK(void) const noexcept
{
	return m_pBaiduTranslateRuntimeStatus->isOK;
}

_STD string BaiduTranslate::whatHappened(void) const noexcept
{
	return m_pBaiduTranslateRuntimeStatus->message;
}

void BaiduTranslate::iSolved(void) noexcept
{
	m_pBaiduTranslateRuntimeStatus->Resetting();
}
