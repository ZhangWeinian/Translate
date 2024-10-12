#pragma once

#include <unordered_map>
#include <atomic>
#include <random>
#include <stdexcept>
#include <string>
#include <version>

#include <curl/curl.h>
#include <openssl/types.h>



#ifdef BAIDUAPI_EXPORTS
	#define BAIDUAPI_API __declspec(dllexport)
#else
	#define BAIDUAPI_API __declspec(dllimport)
#endif // BAIDUAPI_EXPORTS



namespace BaiduTranslateDLL
{
	namespace ErrorDefine
	{
		class NoCommonObject
		{
		public:
			explicit NoCommonObject(void) noexcept				 = default;
			virtual ~NoCommonObject(void) noexcept				 = default;

			NoCommonObject(const NoCommonObject&)				 = delete;
			NoCommonObject(NoCommonObject&&) noexcept			 = delete;
			NoCommonObject& operator=(const NoCommonObject&)	 = delete;
			NoCommonObject& operator=(NoCommonObject&&) noexcept = delete;
		};

		struct LimitInstanceError: public _STD runtime_error
		{
			using _STD runtime_error::runtime_error;
		};

		template <typename ClassType, size_t N>
		class LimitInstance: public NoCommonObject
		{
		public:
			LimitInstance(void)
			{
				if (p_instance_count >= N)
				{
					throw LimitInstanceError("实例对象的个数超过最大值");
				}
				else
				{
					++p_instance_count;
				}
			}

			~LimitInstance(void) noexcept override
			{
				--p_instance_count;
			}

		private:
			static inline _STD atomic<size_t> p_instance_count = 0;
		};
	} // namespace ErrorDefine

	class BaiduTranslate final: public ErrorDefine::LimitInstance<BaiduTranslate, 1>
	{
	public:
		BAIDUAPI_API explicit BaiduTranslate(const _STD string& appid, const _STD string& appkey) noexcept;

		BAIDUAPI_API ~BaiduTranslate(void) noexcept override;

		BAIDUAPI_API _STD string Translate(const _STD string& query,
										   const _STD string& from,
										   const _STD string& to,
										   const _STD string& appid	 = "",
										   const _STD string& appkey = "") noexcept;

		BAIDUAPI_API void		 SetAppIDAndKey(const _STD string& appid, const _STD string& appkey) noexcept;

		BAIDUAPI_API _STD string GetAppIDAndKey(void) const noexcept;

	private:
		_STD string p_app_id { "" };
		_STD string p_app_key { "" };

		//_STD string		  p_query { "" };
		//_STD string		  p_from { "" };
		//_STD string		  p_to { "" };

		const _STD string p_url { "http://api.fanyi.baidu.com/api/trans/vip/translate?" };
		EVP_MD_CTX*		  p_ctx { nullptr };
		CURL*			  p_curl { nullptr };

		_STD mt19937_64	  p_gen { _STD random_device {}() };
		_STD uniform_int_distribution<__int32> p_dis { 32'768, 65'536 };

		static inline const _STD unordered_map<__int32, _STD string> p_APIErrorInfo {
			// clang-format off
			{ 52'000, "成功" },
			{ 52'001, "请求超时，请重试" },
			{ 52'002, "系统错误，请重试"  },
			{ 52'003, "未授权用户！检查您的 appid（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）"
					  "是否正确，或者服务是否开通" },
			{ 54'000, "必填参数为空！检查是否少传参数"
					  "（https://fanyi-api.baidu.com/api/trans/product/apidoc#joinFile）" },
			{ 54'001, "签名错误！请检查您的签名生成方法"	},
			{ 54'003, "访问频率受限！请降低您的调用频率" },
			{ 54'004, "账户余额不足！请前往 管理控制台（https://fanyi-api.baidu.com/api/trans/product/desktop）为账户充值"	 },
			{ 54'005, "长 query 请求频繁！请降低长 query 的发送频率，3s 后再试" },
			{ 58'000, "客户端IP非法！检查个人资料里填写的 "
					  "IP 地址（https://fanyi-api.baidu.com/api/trans/product/desktop?req=developer）"
					  "是否正确。可前往管理控制平台修改 IP 限制，IP 可留空" },
			{ 58'001, "译文语言方向不支持！检查译文语言是否在语言列表里" },
			{ 58'002, "服务当前已关闭！请前往管理控制台（https://fanyi-api.baidu.com/api/trans/product/desktop）开启服务" },
			{ 90'107, "认证未通过或未生效！请前往我的认证（https://fanyi-api.baidu.com/myIdentify）查看认证进度" }
			// clang-format on
		};

		_STD string	  GetMD5(const _STD string& str) const noexcept;

		static size_t CurlWriteCallback(const char* contents, size_t size, size_t nmemb, _STD string* userp);

		_STD string	  GetAPIErrorInfo(__int32 error_code) const noexcept;
	};
} // namespace BaiduTranslateDLL
