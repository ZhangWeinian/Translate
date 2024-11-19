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



EXTREN_C typedef const char* C_String;
EXTREN_C typedef bool		 C_Bool;



EXTREN_C BAIDU_API C_Bool	 BaiduTranslate_Init(C_String appid, C_String appkey);

EXTREN_C BAIDU_API C_String	 BaiduTranslate_Translate(C_String query,
													  C_String from,
													  C_String to,
													  C_String appid,
													  C_String appkey);

EXTREN_C BAIDU_API C_Bool	 BaiduTranslate_SetAppIDAndKey(C_String appid, C_String appkey);

EXTREN_C BAIDU_API C_String	 BaiduTranslate_GetAppIDAndKey(void);

EXTREN_C BAIDU_API C_String	 BaiduTranslate_GetLastError(void);


#endif // !BAIDU
