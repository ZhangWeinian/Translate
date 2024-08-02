#pragma once

#include "pch.h"

#include "../TranslateAPI/TranslateAPI.h"
#include "ConnectAPIAndGui.h"

BaiduTranslate* BeginBaiduTranslate(cstring appid, cstring appkey) noexcept
{
	return new BaiduTranslate(appid, appkey);
}

void EndBaiduTranslate(BaiduTranslate* p) noexcept
{
	delete p;
}

bool SetAppID(BaiduTranslate* p, cstring appid, cstring appkey) noexcept
{
	return p->SetAppID(appid, appkey);
}

cstring Translate(BaiduTranslate* p, cstring source, cstring from, cstring to) noexcept
{
	return p->Translate(source, from, to);
}

bool isOK(BaiduTranslate* p) noexcept
{
	return p->isOK();
}

cstring whatWrong(BaiduTranslate* p) noexcept
{
	return p->whatHappened();
}
