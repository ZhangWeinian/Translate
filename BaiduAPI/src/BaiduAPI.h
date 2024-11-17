#pragma once

#ifndef BAIDUAPI

	#define BAIDUAPI

	#include <unordered_map>
	#include <atomic>
	#include <random>
	#include <stdexcept>
	#include <string>
	#include <version>

	#include <curl/curl.h>
	#include <openssl/types.h>

namespace BaiduTranslateDLL
{
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
		_STD string		  p_appid { "" };
		_STD string		  p_appkey { "" };

		const _STD string p_url { R"(http://api.fanyi.baidu.com/api/trans/vip/translate?)" };
		CURL*			  p_curl { nullptr };
		EVP_MD_CTX*		  p_ctx { nullptr };

		_STD mt19937_64	  p_gen { _STD random_device {}() };
		_STD uniform_int_distribution<_STD size_t> p_dis { 32'768, 65'536 };

		static inline bool						   p_init_is_no_error { false };

		static inline const _STD unordered_map<_STD string, _STD string> p_API_error_info {
			// clang-format off
			{ "52000", R"(成功)" },
			{ "52001", R"(请求超时，请重试)" },
			{ "52002", R"(系统错误，请重试)"  },
			{ "52003", R"(未授权用户！检查您的)"
					   R"( appid（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）)"
					   R"(是否正确，或者服务是否开通)" },
			{ "54000", R"(必填参数为空！检查是否少传参数)"
					   R"(（https://fanyi-api.baidu.com/api/trans/product/apidoc#joinFile）)" },
			{ "54001", R"(签名错误！请检查您的签名生成方法)"	},
			{ "54003", R"(访问频率受限！请降低您的调用频率)" },
			{ "54004", R"(账户余额不足！请前往 管理控制台)"
					   R"(（https://fanyi-api.baidu.com/api/trans/product/desktop）为账户充值)"	 },
			{ "54005", R"(长 query 请求频繁！请降低长 query 的发送频率，3s 后再试)" },
			{ "58000", R"(客户端IP非法！检查个人资料里填写的)"
					   R"( IP 地址（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）)"
					   R"(是否正确。可前往管理控制平台修改 IP 限制，IP 可留空)" },
			{ "58001", R"(译文语言方向不支持！检查译文语言是否在语言列表里)" },
			{ "58002", R"(服务当前已关闭！请前往)"
					   R"(管理控制台（https://fanyi-api.baidu.com/api/trans/product/desktop）开启服务)" },
			{ "90107", R"(认证未通过或未生效！请前往我的认证（https://fanyi-api.baidu.com/myIdentify）查看认证进度)" }
			// clang-format on
		};

		_STD string		   GetMD5(const _STD string& str) noexcept;

		static _STD size_t CurlWriteCallback(const char*  contents,
											 _STD size_t  size,
											 _STD size_t  nmemb,
											 _STD string* userp);

		_STD string		   GetAPIErrorInfo(const _STD string& error_code) noexcept;
	};
} // namespace BaiduTranslateDLL

#endif // !BAIDUAPI
