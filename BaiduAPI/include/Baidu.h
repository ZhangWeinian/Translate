#pragma once

#ifndef BAIDU

	#define BAIDU



	#include <stdbool.h>
	#include <stddef.h>
	#include <stdlib.h>



	#ifdef BAIDUAPI_EXPORTS
		#define BAIDU_API __declspec(dllexport)
	#else
		#define BAIDU_API __declspec(dllimport)
	#endif // BAIDUAPI_EXPORTS

	#ifdef __cplusplus
		#define EXTREN_C extern "C"
	#else
		#define EXTREN_C
	#endif // __cplusplus



EXTREN_C typedef const char* CString;
EXTREN_C typedef bool		 CBool;



EXTREN_C BAIDU_API CBool	 BaiduTranslate_Init(CString appid, CString appkey);

EXTREN_C BAIDU_API CString	 BaiduTranslate_Translate(CString query,
													  CString from,
													  CString to,
													  CString appid,
													  CString appkey);

EXTREN_C BAIDU_API CBool	 BaiduTranslate_SetAppIDAndKey(CString appid, CString appkey);

EXTREN_C BAIDU_API CString	 BaiduTranslate_GetAppIDAndKey(void);



#endif // !BAIDU
