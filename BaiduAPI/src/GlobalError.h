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

		// 百度翻译 API 标识符，以官方错误码为准
		TRANSLATEURL_API_SUCCESS									= 52'000,
		TRANSLATEURL_API_REQUEST_TIMEOUT							= 52'001,
		TRANSLATEURL_API_SYSTEM_ERROR								= 52'002,
		TRANSLATEURL_API_UNAUTHORIZED_USER							= 52'003,
		TRANSLATEURL_API_MISSING_REQUIRED_PARAMETERS				= 54'000,
		TRANSLATEURL_API_SIGNATURE_ERROR							= 54'001,
		TRANSLATEURL_API_ACCESS_FREQUENCY_LIMITED					= 54'003,
		TRANSLATEURL_API_ACCOUNT_BALANCE_INSUFFICIENT				= 54'004,
		TRANSLATEURL_API_LONG_QUERY_REQUEST_FREQUENCY_LIMITED		= 54'005,
		TRANSLATEURL_API_CLIENT_IP_ILLEGAL							= 58'000,
		TRANSLATEURL_API_TRANSLATION_LANGUAGE_DIRECTION_NOT_SUPPORT = 58'001,
		TRANSLATEURL_API_SERVICE_CURRENTLY_CLOSED					= 58'002,
		TRANSLATEURL_API_AUTHENTICATION_NOT_PASSED_OR_NOT_EFFECTIVE = 90'107,

		// 其他标识符，以 9 开头
		OTHER_UNDEFIND_ERROR = 99'999
	};

	class GlobalErrorHandling final
	{
	public:
		using enum ErrorCodeEnum;

		template <typename CodeType>
			requires (_STD is_same_v<CodeType, ErrorCodeEnum> || _STD is_same_v<CodeType, _uint>)
		static void SetLastError(CodeType&& error_code) noexcept
		{
			p_error_code = _STD forward<CodeType>(error_code);

			if (const auto& p_error_info_iter = p_error_infos_def.find(p_error_code);
				p_error_info_iter == p_error_infos_def.end())
			{
				p_error_info.error_info = "Undefined error code.";
				p_error_info.error_tip	= "";

				return;
			}
			else
			{
				p_error_info.error_info = p_error_info_iter->second.error_info;
				p_error_info.error_tip	= p_error_info_iter->second.error_tip;
			}
		}

		static void SetErrorTip(_string_view error_tip) noexcept
		{
			p_error_info.error_tip = _STD move(_string(error_tip));
		}

		static _string GetErrorInfo(void) noexcept
		{
			::Json::Value	   root {};
			_STD ostringstream os {};

			root["error code"]			 = static_cast<_uint>(p_error_code);
			root["error message"]		 = p_error_info.error_info;
			root["reminder information"] = p_error_info.error_tip;

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

			// 百度翻译 API 标识符，以官方错误码为准
			{
				TRANSLATEURL_API_SUCCESS,
				{
					R"(BaiduTranslationAPI: Translation successful.)", R"()"
				}
			},
			{
				TRANSLATEURL_API_REQUEST_TIMEOUT,
				{
					R"(BaiduTranslationAPI: Request timed out, please try again.)", R"()"
				}
			},
			{
				TRANSLATEURL_API_SYSTEM_ERROR,
				{
					R"(BaiduTranslationAPI: System error, please try again.)", R"()"
				}
			},
			{
				TRANSLATEURL_API_UNAUTHORIZED_USER,
				{
					R"(BaiduTranslationAPI: Unauthorized user! )"
					R"(Check if your APPID is correct or if the service is enabled.)"
					R"(（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）)",
					R"()"
				}
			},
			{
				TRANSLATEURL_API_MISSING_REQUIRED_PARAMETERS,
				{
					R"(BaiduTranslationAPI: The required parameter is empty! )"
					R"(Check if parameters are missing.)"
					R"(（https://fanyi-api.baidu.com/api/trans/product/apidoc#joinFile）)",
					R"()"
				}
			},
			{
				TRANSLATEURL_API_SIGNATURE_ERROR,
				{
					R"(BaiduTranslationAPI: Signature error! Please check your )"
					R"(signature generation method)", R"()"
				}
			},
			{
				TRANSLATEURL_API_ACCESS_FREQUENCY_LIMITED,
				{
					R"(BaiduTranslationAPI: Access frequency is limited! )"
					R"(Please reduce your call frequency)", R"()"
				}
			},
			{
				TRANSLATEURL_API_ACCOUNT_BALANCE_INSUFFICIENT,
				{
					R"(BaiduTranslationAPI: Insufficient account balance! )"
					R"(Please go to the management console to recharge your account.)"
					R"(（https://fanyi-api.baidu.com/api/trans/product/desktop）)",
					R"()"
				}
			},
			{
				TRANSLATEURL_API_LONG_QUERY_REQUEST_FREQUENCY_LIMITED,
				{
					R"(BaiduTranslationAPI: Frequent long query requests! )"
					R"(Please reduce the frequency of sending long queries )"
					R"(and try again in 3 seconds)", R"()"
				}
			},
			{
				TRANSLATEURL_API_CLIENT_IP_ILLEGAL,
				{
					R"(BaiduTranslationAPI: The client IP is illegal! )"
					R"(Check if the IP address filled in the personal information is )"
					R"(correct. You can go to the management control platform to )"
					R"(modify IP restrictions, and the IP can be left blank.)"
					R"(（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）)",
					R"()"
				}
			},
			{
				TRANSLATEURL_API_TRANSLATION_LANGUAGE_DIRECTION_NOT_SUPPORT,
				{
					R"(BaiduTranslationAPI: Translation language direction not supported! )"
					R"(Check if the translated language is in the language list)", R"()"
				}
			},
			{
				TRANSLATEURL_API_SERVICE_CURRENTLY_CLOSED,
				{
					R"(BaiduTranslationAPI: The service is currently closed! )"
					R"(Please go to the management console to activate the service.)"
					R"(（https://fanyi-api.baidu.com/api/trans/product/desktop）)",
					R"()"
				}
			},
			{
				TRANSLATEURL_API_AUTHENTICATION_NOT_PASSED_OR_NOT_EFFECTIVE,
				{
					R"(BaiduTranslationAPI: Certification not passed or not )"
					R"(effective! Please go to my authentication to check the )"
					R"(authentication progress.（https://fanyi-api.baidu.com/myIdentify）)",
					R"()"
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
