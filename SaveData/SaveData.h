#pragma once

#include "__inter__AttributeDefinition.h"
#include "pch.h"



SAVEDATA_API _STD string	 GetMD5(const _STD string&) noexcept;

SAVEDATA_API _STD string	 GetIdentifyingCode(const _STD string&) noexcept;

SAVEDATA_API _NODISCARD bool SaveData(const _STD string&, const _STD string&, const _STD string&) noexcept;

SAVEDATA_API info			 GetData(const _STD string&) noexcept;
