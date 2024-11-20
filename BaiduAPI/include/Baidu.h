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



EXTREN_C typedef const char* _cstring;
EXTREN_C typedef bool		 _cbool;



EXTREN_C BAIDU_API _cbool	 BaiduTranslate_Init(_cstring appid, _cstring appkey);

EXTREN_C BAIDU_API _cstring	 BaiduTranslate_Translate(_cstring query,
													  _cstring from,
													  _cstring to,
													  _cstring appid,
													  _cstring appkey);

EXTREN_C BAIDU_API _cbool	 BaiduTranslate_SetAppIDAndKey(_cstring appid, _cstring appkey);

EXTREN_C BAIDU_API _cstring	 BaiduTranslate_GetAppIDAndKey(void);

EXTREN_C BAIDU_API _cstring	 BaiduTranslate_GetLastError(void);


#endif // !BAIDU
