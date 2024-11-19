#pragma once

#ifndef BAIDUAPI

	#define BAIDUAPI

	#include <unordered_map>
	#include <random>
	#include <string>
	#include <version>

	#include <curl/curl.h>
	#include <curl/easy.h>
	#include <openssl/evp.h>
	#include <openssl/types.h>

namespace BaiduTranslateDLL
{
	enum class ErrorCodeEnum : unsigned int
	{
		GLOBAL_SUCCESS											= 0,

		BAIDUTRANSLATE_FUNC_SUCCESS								= 10'000,
		BAIDUTRANSLATE_FUNC_CURL_IS_NULL						= 10'001,
		BAIDUTRANSLATE_FUNC_CTX_IS_NULL							= 10'002,
		BAIDUTRANSLATE_FUNC_APPID_OR_APPKEY_IS_EMPTY			= 10'003,
		BAIDUTRANSLATE_FUNC_CURL_RETURN_ERROR					= 10'004,
		BAIDUTRANSLATE_FUNC_JSON_PARSE_ERROR					= 10'005,
		BAIDUTRANSLATE_FUNC_API_RETURN_ERROR					= 10'006,

		TRANSLATEPTR_PTR_IS_NULL								= 20'000,

		BAIDUURL_API_SUCCESS									= 52'000,
		BADUIURL_API_REQUEST_TIMEOUT							= 52'001,
		BAIDUURL_API_SYSTEM_ERROR								= 52'002,
		BAIDUURL_API_UNAUTHORIZED_USER							= 52'003,
		BAIDUURL_API_MISSING_REQUIRED_PARAMETERS				= 54'000,
		BAIDUURL_API_SIGNATURE_ERROR							= 54'001,
		BAIDUURL_API_ACCESS_FREQUENCY_LIMITED					= 54'003,
		BAIDUURL_API_ACCOUNT_BALANCE_INSUFFICIENT				= 54'004,
		BAIDUURL_API_LONG_QUERY_REQUEST_FREQUENCY_LIMITED		= 54'005,
		BAIDUURL_API_CLIENT_IP_ILLEGAL							= 58'000,
		BAIDUURL_API_TRANSLATION_LANGUAGE_DIRECTION_NOT_SUPPORT = 58'001,
		BAIDUURL_API_SERVICE_CURRENTLY_CLOSED					= 58'002,
		BAIDUURL_API_AUTHENTICATION_NOT_PASSED_OR_NOT_EFFECTIVE = 90'107,

		OTHER_UNDEFIND_ERROR									= 99'999
	};

	class BaiduTranslateDLLError final
	{
	public:
		using enum ErrorCodeEnum;

		static void		   SetLastError(const ErrorCodeEnum error_code) noexcept;

		static void		   SetErrorTip(const _STD string& error_tip) noexcept;

		static _STD string GetErrorInfo(void) noexcept;

	private:
		using ErrorStruct = struct
		{
			_STD string error_info;
			_STD string error_tip;
		};

		// clang-format off
		static inline const _STD unordered_map<ErrorCodeEnum, ErrorStruct> p_error_infos_def = {
			{ GLOBAL_SUCCESS, { "全局：成功。", "" } },

			{ BAIDUTRANSLATE_FUNC_SUCCESS, { "百度翻译函数：初始化成功。", "" } },
			{ BAIDUTRANSLATE_FUNC_CURL_IS_NULL, { "百度翻译函数：CURL 句柄为空。", "" } },
			{ BAIDUTRANSLATE_FUNC_CTX_IS_NULL, { "百度翻译函数：MD5 上下文为空。", "" } },
			{ BAIDUTRANSLATE_FUNC_APPID_OR_APPKEY_IS_EMPTY, { "百度翻译函数：APPID 或 APPKEY 为空。", "" } },
			{ BAIDUTRANSLATE_FUNC_CURL_RETURN_ERROR, { "百度翻译函数：CURL 返回错误。", "" } },
			{ BAIDUTRANSLATE_FUNC_JSON_PARSE_ERROR, { "百度翻译函数：JSON 解析错误。", "" } },
			{ BAIDUTRANSLATE_FUNC_API_RETURN_ERROR, { "百度翻译函数：API 返回错误。", "" } },

			{ TRANSLATEPTR_PTR_IS_NULL, { "TranslatePtr：基指针为空。", "公开头文件中的基指针为空。" } },

			{ BAIDUURL_API_SUCCESS, { "百度翻译 API ：翻译成功。", "" } },
			{ BADUIURL_API_REQUEST_TIMEOUT, { "百度翻译 API ：请求超时，请重试。", "" } },
			{ BAIDUURL_API_SYSTEM_ERROR, { "百度翻译 API ：系统错误，请重试。", "" } },
			{
				BAIDUURL_API_UNAUTHORIZED_USER,
				{ R"(百度翻译 API ：未授权用户！检查您的)"
				  R"( appid（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）)"
				  R"(是否正确，或者服务是否开通)", "" }
			},
			{
				BAIDUURL_API_MISSING_REQUIRED_PARAMETERS,
				{ R"(百度翻译 API ：必填参数为空！检查是否少传参数)"
				  R"(（https://fanyi-api.baidu.com/api/trans/product/apidoc#joinFile）)", "" }
			},
			{ BAIDUURL_API_SIGNATURE_ERROR, { R"(百度翻译 API ：签名错误！请检查您的签名生成方法)", "" } },
			{ BAIDUURL_API_ACCESS_FREQUENCY_LIMITED, { R"(百度翻译 API ：访问频率受限！请降低您的调用频率)", "" } },
			{
				BAIDUURL_API_ACCOUNT_BALANCE_INSUFFICIENT,
				{ R"(百度翻译 API ：账户余额不足！请前往 管理控制台)"
				  R"(（https://fanyi-api.baidu.com/api/trans/product/desktop）为账户充值)", "" }
			},
			{
				BAIDUURL_API_LONG_QUERY_REQUEST_FREQUENCY_LIMITED,
				{ R"(百度翻译 API ：长 query 请求频繁！请降低长 query 的发送频率，3s 后再试)", "" }
			},
			{
				BAIDUURL_API_CLIENT_IP_ILLEGAL,
				{ R"(百度翻译 API ：客户端IP非法！检查个人资料里填写的)"
				  R"( IP 地址（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）)"
				  R"(是否正确。可前往管理控制平台修改 IP 限制，IP 可留空)", "" }
			},
			{
				BAIDUURL_API_TRANSLATION_LANGUAGE_DIRECTION_NOT_SUPPORT,
				{ R"(百度翻译 API ：译文语言方向不支持！检查译文语言是否在语言列表里)", "" }
			},
			{
				BAIDUURL_API_SERVICE_CURRENTLY_CLOSED,
				{ R"(百度翻译 API ：服务当前已关闭！请前往)"
				  R"(管理控制台（https://fanyi-api.baidu.com/api/trans/product/desktop）开启服务)", "" }
			},
			{
				BAIDUURL_API_AUTHENTICATION_NOT_PASSED_OR_NOT_EFFECTIVE,
				{ R"(百度翻译 API ：认证未通过或未生效！请前往我的认证)"
				  R"(（https://fanyi-api.baidu.com/myIdentify）查看认证进度)", "" }
			},
			{ OTHER_UNDEFIND_ERROR, { "其他：未定义错误。", "" } }
		};
		// clang-format on

		static inline ErrorCodeEnum p_error_code { ErrorCodeEnum::GLOBAL_SUCCESS };
		static inline ErrorStruct	p_error_info { "", "" };
	};

	class BaiduTranslateFunction final
	{
	public:
		BaiduTranslateFunction(const _STD string& appid	 = "",
							   const _STD string& appkey = "") noexcept;

		~BaiduTranslateFunction(void) noexcept;

		_STD string Translate(const _STD string& query,
							  const _STD string& from,
							  const _STD string& to,
							  const _STD string& appid	= "",
							  const _STD string& appkey = "") noexcept;

		void		SetAppIDAndKey(const _STD string& appid, const _STD string& appkey) noexcept;

		_STD string GetAppIDAndKey(void) noexcept;

		static bool InitIsNoError(void) noexcept;

	private:
		_STD string		   GetMD5(const _STD string& str) const noexcept;

		static _STD size_t CurlWriteCallback(const char*  contents,
											 _STD size_t  size,
											 _STD size_t  nmemb,
											 _STD string* userp);

	private:
		using enum ErrorCodeEnum;

		static inline bool m_init_is_no_error { false };

		_STD string		   m_appid { "" };
		_STD string		   m_appkey { "" };

		const _STD string  m_url { R"(http://api.fanyi.baidu.com/api/trans/vip/translate?)" };
		CURL*			   m_curl { curl_easy_init() };
		EVP_MD_CTX*		   m_ctx { EVP_MD_CTX_new() };

		_STD mt19937_64	   m_gen { _STD random_device {}() };
		_STD uniform_int_distribution<_STD size_t> m_dis { 32'768, 65'536 };
	};
} // namespace BaiduTranslateDLL

#endif // !BAIDUAPI
