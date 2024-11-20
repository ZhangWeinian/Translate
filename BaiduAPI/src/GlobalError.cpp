#pragma once

#include "aaa_TypeDefined.h"

#include "GlobalError.h"

#include <iosfwd>
#include <memory>
#include <string>
#include <version>

#include <json/value.h>
#include <json/writer.h>

void BaiduTranslateDLL::GlobalErrorHandling::SetLastError(const ErrorCodeEnum error_code) noexcept
{
	p_error_code			= error_code;
	p_error_info.error_info = p_error_infos_def.at(error_code).error_info;
	p_error_info.error_tip	= p_error_infos_def.at(error_code).error_tip;
}

void BaiduTranslateDLL::GlobalErrorHandling::SetErrorTip(const _string& error_tip) noexcept
{
	p_error_info.error_tip = error_tip;
}

_string BaiduTranslateDLL::GlobalErrorHandling::GetErrorInfo(void) noexcept
{
	Json::Value		   root {};
	_STD ostringstream os {};

	root["错误代码"] = static_cast<_uint>(p_error_code);
	root["错误信息"] = p_error_info.error_info;
	root["提示信息"] = p_error_info.error_tip;

	Json::StreamWriterBuilder builder {};
	builder["commentStyle"] = "None";
	builder["indentation"]	= "    ";

	_STD unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root, &os);

	return os.str();
}
