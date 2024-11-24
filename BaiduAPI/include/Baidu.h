#pragma once

#ifndef BAIDU

	#define BAIDU

	#ifdef BAIDUAPI_EXPORTS
		#define BAIDU_API __declspec(dllexport)
	#else
		#define BAIDU_API __declspec(dllimport)
	#endif // BAIDUAPI_EXPORTS

	#ifdef __cplusplus
		#define EXTREN_C extern "C"
	#else
		#include <stdbool.h>
		#define EXTREN_C
	#endif // __cplusplus



EXTREN_C BAIDU_API bool		   BaiduTranslate_Init(const char* appid, const char* appkey);

EXTREN_C BAIDU_API const char* BaiduTranslate_Translate(const char* query,
														const char* from,
														const char* to,
														const char* appid,
														const char* appkey);

EXTREN_C BAIDU_API bool		   BaiduTranslate_SetAppIDAndKey(const char* appid, const char* appkey);

EXTREN_C BAIDU_API const char* BaiduTranslate_GetAppIDAndKey(void);

EXTREN_C BAIDU_API const char* BaiduTranslate_GetLastError(void);


#endif // !BAIDU
