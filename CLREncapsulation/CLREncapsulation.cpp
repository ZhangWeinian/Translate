#pragma once

#include "pch.h"

#include <msclr/marshal.h>

#include "../TranslateAPIStatic/TranslateAPIStatic.h"
#include "CLREncapsulation.h"

#ifndef _TRANSLATEAPISTATIC

	#define _TRANSLATEAPISTATIC ::

	#ifndef _BDTRANSLATEAPISTATIC
		#define _BDTRANSLATEAPISTATIC _TRANSLATEAPISTATIC BDTranslateLib::
	#endif	// !_BDTRANSLATEAPISTATIC

#endif		// !_TRANSLATEAPISTATIC


void CLREncapsulation::UsingBDTranslateStatic::BDTranslate(System::String ^ appid, System::String ^ appkey)
{
	return _BDTRANSLATEAPISTATIC BDTranslate(SysstrToStdstr(appid), SysstrToStdstr(appkey));
}

bool CLREncapsulation::UsingBDTranslateStatic::SetAppID(System::String ^ appid, System::String ^ appkey)
{
	return _BDTRANSLATEAPISTATIC SetAppID(SysstrToStdstr(appid), SysstrToStdstr(appkey));
}

System::String ^ CLREncapsulation::UsingBDTranslateStatic::Translate(System::String ^ source,
																	 System::String ^ from,
																	 System::String ^ to)
{
	return StdstrToSysstr(
		_BDTRANSLATEAPISTATIC Translate(SysstrToStdstr(source), SysstrToStdstr(from), SysstrToStdstr(to)));
}

bool CLREncapsulation::UsingBDTranslateStatic::isOK(void)
{
	return _BDTRANSLATEAPISTATIC isOK();
}

System::String ^ CLREncapsulation::UsingBDTranslateStatic::whatHappened(void)
{
	return StdstrToSysstr(_BDTRANSLATEAPISTATIC whatHappened());
}

System::String ^ CLREncapsulation::UsingBDTranslateStatic::StdstrToSysstr(const char* str)
{
	System::String ^ systemString = msclr::interop::marshal_as<System::String ^>(str);

	return systemString;
}

const char* CLREncapsulation::UsingBDTranslateStatic::SysstrToStdstr(System::String ^ str)
{
	m_str				= str;

	const char* cString = msclr::interop::marshal_as<const char*>(str);

	return cString;
}
