#pragma once

#include "__inter__AttributeDefinition.h"
#include "pch.h"



BAIDUTRANSLATE_API _NODISCARD bool SetBaiDuTranslateAppID(const _STD string&, const _STD string&) noexcept;

BAIDUTRANSLATE_API _STD string	   BaiduTranslate(const _STD string&, const _STD string&, const _STD string&) noexcept;
