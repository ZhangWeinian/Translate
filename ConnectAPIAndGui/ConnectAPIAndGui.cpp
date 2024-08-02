#pragma once

#include "pch.h"

#include "../TranslateAPI/TranslateAPI.h"
#include "ConnectAPIAndGui.h"

void* BeginBaiduTranslate(cstring appid, cstring appkey) noexcept
{
	return new BaiduTranslate(appid, appkey);
}

void EndBaiduTranslate(void* p) noexcept
{
	delete (BaiduTranslate*)p;
}

bool SetAppID(void* p, cstring appid, cstring appkey) noexcept
{
	return ((BaiduTranslate*)p)->SetAppID(appid, appkey);
}

cstring Translate(void* p, cstring source, cstring from, cstring to) noexcept
{
	return ((BaiduTranslate*)p)->Translate(source, from, to);
}

bool isOK(void* p) noexcept
{
	return ((BaiduTranslate*)p)->isOK();
}

cstring whatHappened(void* p) noexcept
{
	return ((BaiduTranslate*)p)->whatHappened();
}
