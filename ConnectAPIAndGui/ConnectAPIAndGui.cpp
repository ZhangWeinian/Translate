#pragma once

#include "pch.h"

#include <string>
#include <version>

#include "../TranslateAPI/TranslateAPIStatic.h"
#include "ConnectAPIAndGui.h"

ConnectAPIAndGui::BaiduTranslateAPI::BaiduTranslateAPI(String ^ appid, String ^ appkey)
{
	m_pBaiduTranslate = new BaiduTranslate(SysstrToStdstr(appid), SysstrToStdstr(appkey));
}

ConnectAPIAndGui::BaiduTranslateAPI::~BaiduTranslateAPI(void)
{
	if (m_pBaiduTranslate != nullptr)
	{
		delete m_pBaiduTranslate;
	}
}

bool ConnectAPIAndGui::BaiduTranslateAPI::SetAppID(String ^ appid, String ^ appkey)
{
	return m_pBaiduTranslate->SetAppID(SysstrToStdstr(appid), SysstrToStdstr(appkey));
}

String ^ ConnectAPIAndGui::BaiduTranslateAPI::Translate(String ^ source, String ^ from, String ^ to)
{
	return StdstrToSysstr(
		m_pBaiduTranslate->Translate(SysstrToStdstr(source), SysstrToStdstr(from), SysstrToStdstr(to)));
}

bool ConnectAPIAndGui::BaiduTranslateAPI::isOK(void)
{
	return m_pBaiduTranslate->isOK();
}

String ^ ConnectAPIAndGui::BaiduTranslateAPI::whatHappened(void)
{
	return StdstrToSysstr(m_pBaiduTranslate->whatHappened());
}

_STD string ConnectAPIAndGui::BaiduTranslateAPI::SysstrToStdstr(String ^ s)
{
	using namespace System;
	using namespace Runtime::InteropServices;

	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();

	_STD string str	  = chars;

	Marshal::FreeHGlobal(IntPtr((void*)chars));

	return str;
}

String ^ ConnectAPIAndGui::BaiduTranslateAPI::StdstrToSysstr(const _STD string& s)
{
	using namespace System;
	using namespace Runtime::InteropServices;

	return Marshal::PtrToStringAnsi((IntPtr)(char*)s.c_str());
}
