#pragma once

#include <string>
#include <version>

#include "../TranslateAPIStatic/TranslateAPIStatic.h"
#include "ConnectAPIAndGui.h"

ConnectAPIAndGui::BaiduTranslateAPI::BaiduTranslateAPI(String ^ appid, String ^ appkey)
{
	m_pBaiduTranslate = new BaiduTranslate(SysstrToStdstr(appid), SysstrToStdstr(appkey));

	if (m_pBaiduTranslate == nullptr)
	{
		m_isOK	  = false;

		m_message = "创建 BaiduTranslate 对象失败";
	}
}

ConnectAPIAndGui::BaiduTranslateAPI::~BaiduTranslateAPI(void)
{
	if (m_pBaiduTranslate != nullptr)
	{
		delete (BaiduTranslate*)m_pBaiduTranslate;

		m_pBaiduTranslate = nullptr;
	}
}

bool ConnectAPIAndGui::BaiduTranslateAPI::SetAppID(String ^ appid, String ^ appkey)
{
	return (m_pBaiduTranslate != nullptr)
			   ? ((BaiduTranslate*)m_pBaiduTranslate)->SetAppID(SysstrToStdstr(appid), SysstrToStdstr(appkey))
			   : false;
}

String ^ ConnectAPIAndGui::BaiduTranslateAPI::Translate(String ^ source, String ^ from, String ^ to)
{
	return (m_pBaiduTranslate != nullptr)
			   ? StdstrToSysstr(((BaiduTranslate*)m_pBaiduTranslate)
									->Translate(SysstrToStdstr(source), SysstrToStdstr(from), SysstrToStdstr(to)))
			   : m_message;
}

bool ConnectAPIAndGui::BaiduTranslateAPI::isOK(void)
{
	return (m_pBaiduTranslate != nullptr) ? ((BaiduTranslate*)m_pBaiduTranslate)->isOK() : false;
}

String ^ ConnectAPIAndGui::BaiduTranslateAPI::whatHappened(void)
{
	return (m_pBaiduTranslate != nullptr) ? StdstrToSysstr(((BaiduTranslate*)m_pBaiduTranslate)->whatHappened())
										  : m_message;
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
