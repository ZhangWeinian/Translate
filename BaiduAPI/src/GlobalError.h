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

		template <typename ErrorCodeType = ErrorCodeEnum>
		static void SetLastError(ErrorCodeType&& error_code) noexcept
		{
			p_error_code			= _STD forward<ErrorCodeType>(error_code);
			p_error_info.error_info = p_error_infos_def.at(p_error_code).error_info;
			p_error_info.error_tip	= p_error_infos_def.at(p_error_code).error_tip;
		}

		template <_string_type StringType>
		static void SetErrorTip(StringType&& error_tip) noexcept
		{
			p_error_info.error_tip = _STD move(_string(error_tip));
		}

		static _string GetErrorInfo(void) noexcept
		{
			::Json::Value	   root {};
			_STD ostringstream os {};

			root["错误代码"s] = static_cast<_uint>(p_error_code);
			root["错误信息"s] = p_error_info.error_info;
			root["提示信息"s] = p_error_info.error_tip;

			::Json::StreamWriterBuilder builder {};
			builder["commentStyle"s] = "None"s;
			builder["indentation"s]	 = "    "s;

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

		// clang-format off
		static inline const _STD unordered_map<ErrorCodeEnum, ErrorStruct> p_error_infos_def = {
			// 全局标识符，以 0 开头
			{ GLOBAL_SUCCESS, { "全局：成功。"s, ""s } },

			// 密码设置函数标识符，以 1 开头
			{ PASSWORD_FUNC_SUCCESS, { "密码设置函数：初始化成功。"s, ""s } },
			{ PASSWORD_FUNC_CTX_IS_NULL, { "密码设置函数：MD5 上下文为空。"s, ""s } },
			{ PASSWORD_FUNC_OPEN_LOCALAPPDATA_FAILED, { "密码设置函数：打开本地目录 AppData 失败。"s, ""s } },
			{ PASSWORD_FUNC_OPEN_CONFIG_FILE_FAILED, { "密码设置函数：打开配置文件失败。"s, ""s } },
			{ PASSWORD_FUNC_MAKING_LOCAL_FILE_FAILED, { "密码设置函数：创建本地文件失败。"s, ""s } },
			{ PASSWORD_FUNC_OPEN_WITH_JSON_FAILED, { "密码设置函数：无法以 JSON 格式读取。"s, ""s } },
			{ PAASWORD_FUNC_INIT_ERROR, { "密码设置函数：密码设置函数初始化失败。"s, ""s } },

			// 百度翻译函数标识符，以 3 开头
			{ BAIDUTRANSLATE_FUNC_SUCCESS, { "百度翻译函数：初始化成功。"s, ""s } },
			{ BAIDUTRANSLATE_FUNC_CURL_IS_NULL, { "百度翻译函数：CURL 句柄为空。"s, ""s } },
			{ BAIDUTRANSLATE_FUNC_APPID_OR_APPKEY_IS_EMPTY, { "百度翻译函数：APPID 或 APPKEY 为空。"s, ""s } },
			{ BAIDUTRANSLATE_FUNC_CURL_RETURN_ERROR, { "百度翻译函数：CURL 返回错误。"s, ""s } },
			{ BAIDUTRANSLATE_FUNC_JSON_PARSE_ERROR, { "百度翻译函数：JSON 解析错误。"s, ""s } },
			{ BAIDUTRANSLATE_FUNC_API_RETURN_ERROR, { "百度翻译函数：API 返回错误。"s, ""s } },
			{ BAIDUTRANSLATE_FUNC_INIT_ERROR, { "百度翻译函数：初始化失败。"s, ""s } },

			// 公开头文件标识符，以 4 开头
			{ EXPORT_TRANSLATE_PTR_IS_NULL, { "TranslatePtr：基指针为空。"s, "公开头文件中的基指针为空。"s } },

			// 百度翻译 API 标识符，以官方错误码为准
			{ TRANSLATEURL_API_SUCCESS, { "百度翻译 API ：翻译成功。"s, ""s } },
			{ TRANSLATEURL_API_REQUEST_TIMEOUT, { "百度翻译 API ：请求超时，请重试。"s, ""s } },
			{ TRANSLATEURL_API_SYSTEM_ERROR, { "百度翻译 API ：系统错误，请重试。"s, ""s } },
			{ TRANSLATEURL_API_UNAUTHORIZED_USER,
				{ R"(百度翻译 API ：未授权用户！检查您的)"
				  R"( appid（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）)"
				  R"(是否正确，或者服务是否开通)"s, ""s }
			},
			{ TRANSLATEURL_API_MISSING_REQUIRED_PARAMETERS,
				{ R"(百度翻译 API ：必填参数为空！检查是否少传参数)"
				  R"(（https://fanyi-api.baidu.com/api/trans/product/apidoc#joinFile）)"s, ""s }
			},
			{ TRANSLATEURL_API_SIGNATURE_ERROR, { R"(百度翻译 API ：签名错误！请检查您的签名生成方法)"s, ""s } },
			{ TRANSLATEURL_API_ACCESS_FREQUENCY_LIMITED,
				{ R"(百度翻译 API ：访问频率受限！请降低您的调用频率)"s, ""s } },
			{ TRANSLATEURL_API_ACCOUNT_BALANCE_INSUFFICIENT,
				{ R"(百度翻译 API ：账户余额不足！请前往 管理控制台)"
				  R"(（https://fanyi-api.baidu.com/api/trans/product/desktop）为账户充值)"s, ""s }
			},
			{ TRANSLATEURL_API_LONG_QUERY_REQUEST_FREQUENCY_LIMITED,
				{ R"(百度翻译 API ：长 query 请求频繁！请降低长 query 的发送频率，3s 后再试)"s, ""s }
			},
			{ TRANSLATEURL_API_CLIENT_IP_ILLEGAL,
				{ R"(百度翻译 API ：客户端IP非法！检查个人资料里填写的)"
				  R"( IP 地址（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）)"
				  R"(是否正确。可前往管理控制平台修改 IP 限制，IP 可留空)"s, ""s }
			},
			{ TRANSLATEURL_API_TRANSLATION_LANGUAGE_DIRECTION_NOT_SUPPORT,
				{ R"(百度翻译 API ：译文语言方向不支持！检查译文语言是否在语言列表里)"s, ""s }
			},
			{ TRANSLATEURL_API_SERVICE_CURRENTLY_CLOSED,
				{ R"(百度翻译 API ：服务当前已关闭！请前往)"
				  R"(管理控制台（https://fanyi-api.baidu.com/api/trans/product/desktop）开启服务)"s, ""s }
			},

			{ TRANSLATEURL_API_AUTHENTICATION_NOT_PASSED_OR_NOT_EFFECTIVE,
				{ R"(百度翻译 API ：认证未通过或未生效！请前往我的认证)"
				  R"(（https://fanyi-api.baidu.com/myIdentify）查看认证进度)"s, ""s }
			},

			// 其他标识符，以 9 开头
			{ OTHER_UNDEFIND_ERROR, { "其他：未定义错误。"s, ""s } }
		};
		// clang-format on

		static inline ErrorCodeEnum p_error_code { ErrorCodeEnum::GLOBAL_SUCCESS };
		static inline ErrorStruct	p_error_info { ""s, ""s };
	};
} // namespace BaiduTranslateDLL

#endif // !GLOBALERROR
