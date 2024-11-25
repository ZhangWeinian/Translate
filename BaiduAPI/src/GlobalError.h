#pragma once

#ifndef GLOBALERROR

	#define GLOBALERROR

	#include "TypeDefined.h"

	#include <type_traits>
	#include <unordered_map>
	#include <iosfwd>
	#include <memory>
	#include <string>
	#include <version>

	#include <json/value.h>
	#include <json/writer.h>

namespace BaiduTranslateDLL
{
	enum class ErrorCodeEnum : _uint
	{
		// 全局标识符，以 0 开头
		GLOBAL_SUCCESS = 0,

		// 密码设置函数标识符，以 1 开头
		PASSWORD_FUNC_SUCCESS				   = 10'000,
		PASSWORD_FUNC_CTX_IS_NULL			   = 10'001,
		PASSWORD_FUNC_OPEN_LOCALAPPDATA_FAILED = 10'002,
		PASSWORD_FUNC_OPEN_CONFIG_FILE_FAILED  = 10'003,
		PASSWORD_FUNC_MAKING_LOCAL_FILE_FAILED = 10'004,
		PASSWORD_FUNC_OPEN_WITH_JSON_FAILED	   = 10'005,
		PAASWORD_FUNC_INIT_ERROR			   = 19'999,

		// 百度翻译函数标识符，以 3 开头
		BAIDUTRANSLATE_FUNC_SUCCESS					 = 30'000,
		BAIDUTRANSLATE_FUNC_CURL_IS_NULL			 = 30'001,
		BAIDUTRANSLATE_FUNC_APPID_OR_APPKEY_IS_EMPTY = 30'002,
		BAIDUTRANSLATE_FUNC_CURL_RETURN_ERROR		 = 30'003,
		BAIDUTRANSLATE_FUNC_JSON_PARSE_ERROR		 = 30'004,
		BAIDUTRANSLATE_FUNC_API_RETURN_ERROR		 = 30'005,
		BAIDUTRANSLATE_FUNC_INIT_ERROR				 = 39'999,

		// 公开头文件标识符，以 4 开头
		EXPORT_TRANSLATE_PTR_IS_NULL = 40'000,

		// 其他标识符，以 99 开头
		OTHER_UNDEFIND_ERROR = 99'999
	};

	class GlobalErrorHandling final
	{
	public:
		using enum ErrorCodeEnum;

		static void SetLastError(const ErrorCodeEnum& error_code) noexcept
		{
			p_error_code = error_code;

			if (const auto& p_error_info_iter = p_error_infos_def.find(p_error_code);
				p_error_info_iter != p_error_infos_def.end())
			{
				p_error_info.error_info = p_error_info_iter->second.error_info;
				p_error_info.error_tip	= p_error_info_iter->second.error_tip;
			}
			else
			{
				p_error_info.error_info = "Undefined error code.";
				p_error_info.error_tip	= "";

				return;
			}
		}

		static void SetErrorTip(_string_view error_tip) noexcept
		{
			p_error_info.error_tip = _STD move(_string(error_tip));
		}

		static _string GetErrorInfo(const _string& name = "", ::Json::Value value = {}) noexcept
		{
			::Json::Value	   root { ::Json::objectValue };
			_STD ostringstream os {};

			root["error code"]			 = static_cast<_uint>(p_error_code);
			root["error message"]		 = p_error_info.error_info;
			root["reminder information"] = p_error_info.error_tip;

			if (!name.empty())
			{
				root[name] = value;
			}

			::Json::StreamWriterBuilder builder {};
			builder["commentStyle"] = "None";
			builder["indentation"]	= "    ";

			_STD unique_ptr<::Json::StreamWriter> writer(builder.newStreamWriter());
			writer->write(root, &os);

			return os.str();
		}

	private:
		using ErrorStruct = struct
		{
			_string error_info;
			_string error_tip;
		};

		static inline const _STD unordered_map<ErrorCodeEnum, ErrorStruct> p_error_infos_def = {
			// clang-format off

			// 全局标识符，以 0 开头
			{	 GLOBAL_SUCCESS,
				{
					R"(Global: Successful.)", R"()"
				}
			},

			// 密码设置函数标识符，以 1 开头
			{
				PASSWORD_FUNC_SUCCESS,
				{
					R"(PasswordSettingFunction: Initialization successful.)", R"()"
				}
			},
			{
				PASSWORD_FUNC_CTX_IS_NULL,
				{
					R"(PasswordSettingFunction: MD5 context is empty.)", R"()"
				}
			},
			{
				PASSWORD_FUNC_OPEN_LOCALAPPDATA_FAILED,
				{
					R"(PasswordSettingFunction: Failed to open the local directory )"
					R"('%AppData%'.)", R"()"
				}
			},
			{
				PASSWORD_FUNC_OPEN_CONFIG_FILE_FAILED,
				{
					R"(PasswordSettingFunction: Failed to open configuration file.)",
					R"()"
				}
			},
			{
				PASSWORD_FUNC_MAKING_LOCAL_FILE_FAILED,
				{
					R"(PasswordSettingFunction: Failed to create local file.)", R"()"
				}
			},
			{
				PASSWORD_FUNC_OPEN_WITH_JSON_FAILED,
				{
					R"(PasswordSettingFunction: unable to read in JSON format.)", R"()"
				}
			},
			{
				PAASWORD_FUNC_INIT_ERROR,
				{
					R"(PasswordSettingFunction: PasswordSettingFunction )"
					R"(initialization failed.)", R"()"
				}
			},

			// 百度翻译函数标识符，以 3 开头
			{
				BAIDUTRANSLATE_FUNC_SUCCESS,
				{
					R"(BaiduTranslationFunction: Initialization successful.)", R"()"
				}
			},
			{
				BAIDUTRANSLATE_FUNC_CURL_IS_NULL,
				{
					R"(BaiduTranslationFunction: CURL handle is empty.)", R"()"
				}
			},
			{
				BAIDUTRANSLATE_FUNC_APPID_OR_APPKEY_IS_EMPTY,
				{
					R"(BaiduTranslationFunction: APPID or APPKEY is empty.)", R"()"
				}
			},
			{
				BAIDUTRANSLATE_FUNC_CURL_RETURN_ERROR,
				{
					R"(BaiduTranslationFunction: CURL returns an error.)", R"()"
				}
			},
			{
				BAIDUTRANSLATE_FUNC_JSON_PARSE_ERROR,
				{
					R"(BaiduTranslationFunction: JSON parsing error.)", R"()"
				}
			},
			{
				BAIDUTRANSLATE_FUNC_API_RETURN_ERROR,
				{
					R"(BaiduTranslationFunction: API returned an error.)", R"()"
				}
			},
			{
				BAIDUTRANSLATE_FUNC_INIT_ERROR,
				{
					R"(BaiduTranslationFunction: Initialization failed.)", R"()"
				}
			},

			// 公开头文件标识符，以 4 开头
			{
				EXPORT_TRANSLATE_PTR_IS_NULL,
				{
					R"(TranslatePtr: The base pointer is empty.)",
					R"(The base pointer in the public header file is empty.)"
				}
			},

			// 其他标识符，以 9 开头
			{
				OTHER_UNDEFIND_ERROR,
				{
					R"(Other: Undefined error.)", R"()"
				}
			}

			// clang-format on
		};

		static inline ErrorCodeEnum p_error_code { ErrorCodeEnum::GLOBAL_SUCCESS };
		static inline ErrorStruct	p_error_info { "", "" };
	};
} // namespace BaiduTranslateDLL

#endif // !GLOBALERROR
