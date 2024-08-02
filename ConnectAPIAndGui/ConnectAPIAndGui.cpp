#pragma once

#include "pch.h"

#include "../TranslateAPI/TranslateAPI.h"
#include "ConnectAPIAndGui.h"

void* BDTranslate_Begin(cstring appid, cstring appkey) noexcept
{
	return new BDTranslate(appid, appkey);
}

void BDTranslate_End(void* p) noexcept
{
	delete (BDTranslate*)p;
}

bool BDTranslate_SetAppID(void* p, cstring appid, cstring appkey) noexcept
{
	return ((BDTranslate*)p)->SetAppID(appid, appkey);
}

cstring BDTranslate_Translate(void* p, cstring source, cstring from, cstring to) noexcept
{
	return ((BDTranslate*)p)->Translate(source, from, to);
}

bool BDTranslate_isOK(void* p) noexcept
{
	return ((BDTranslate*)p)->isOK();
}

cstring BDTranslate_whatHappened(void* p) noexcept
{
	return ((BDTranslate*)p)->whatHappened();
}
