#pragma once

#include <string>

#include <atomic>
#include <stdexcept>
#include <version>

#include <curl/curl.h>
#include <openssl/types.h>



#ifdef BAIDUAPI_EXPORTS
	#define BAIDUAPI_API __declspec(dllexport)
#else
	#define BAIDUAPI_API __declspec(dllimport)
#endif // BAIDUAPI_EXPORTS



namespace BaiDuTranslateDLL
{
	namespace ErrorDefine
	{
		struct LimitInstanceError: public _STD runtime_error
		{
			using _STD runtime_error::runtime_error;
		};

		template <typename ClassType, size_t N>
		class BAIDUAPI_API LimitInstance
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

			virtual ~LimitInstance(void) noexcept
			{
				--p_instance_count;
			}

		private:
			static inline _STD atomic<size_t> p_instance_count = 0;
		};
	} // namespace ErrorDefine

	class BAIDUAPI_API BaiDuTranslate: public ErrorDefine::LimitInstance<BaiDuTranslate, 1>
	{
	public:
		explicit BaiDuTranslate(const _STD string& appid = "", const _STD string& appkey = "") noexcept;
		~BaiDuTranslate(void) noexcept final;

		_STD string Translate(const _STD string& query,
							  const _STD string& from,
							  const _STD string& to,
							  const _STD string& appid	= "",
							  const _STD string& appkey = "") const noexcept;

		void		SetAppIDAndKey(const _STD string& appid, const _STD string& appkey) noexcept;
		_STD string GetAppIDAndKey(void) const noexcept;

	private:
		_STD string		   p_appid {};
		_STD string		   p_appkey {};
		_STD string		   p_query {};
		_STD string		   p_from {};
		_STD string		   p_to {};

		const _STD string  p_url { "http://api.fanyi.baidu.com/api/trans/vip/translate?" };
		EVP_MD_CTX*		   p_ctx {};
		CURL*			   p_curl {};

		inline __int32	   GetIntRandom(void) const noexcept;
		inline _STD string GetMD5(const _STD string& str) const noexcept;

		static size_t	   CurlWriteCallback(const char* contents, size_t size, size_t nmemb, _STD string* userp);
	};
} // namespace BaiDuTranslateDLL
