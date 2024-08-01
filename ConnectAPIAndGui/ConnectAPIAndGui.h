#pragma once

#include <version>

#if defined(_HAS_CXX20)

	#ifndef _STD
		#define _STD ::std::
	#endif	// !_STD

	#include <string>

using namespace System;

namespace ConnectAPIAndGui
{
public

	ref class BaiduTranslateAPI
	{
	public:
		BaiduTranslateAPI(String ^ appid, String ^ appkey);

		~BaiduTranslateAPI(void);

		bool SetAppID(String ^ appid, String ^ appkey);

		String ^ Translate(String ^ source, String ^ from, String ^ to);

		bool isOK(void);

		String ^ whatHappened(void);

	private:
		void* m_pBaiduTranslate { nullptr };

		bool  m_isOK { true };

		String ^ m_message { "" };

		_STD string SysstrToStdstr(String ^ s);

		String ^ StdstrToSysstr(const _STD string& s);
	};
}  // namespace ConnectAPIAndGui

#endif	// defined(_HAS_CXX20)
