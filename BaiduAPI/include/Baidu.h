#pragma once

#ifndef BAIDUAPIEXPORT

	#define BAIDUAPIEXPORT

	#ifdef BAIDUAPI_EXPORTS
		#define BAIDU_API __declspec(dllexport)
	#else
		#define BAIDU_API __declspec(dllimport)
	#endif // BAIDUAPI_EXPORTS

	#if defined(__cplusplus) && !defined(EXTREN_C_BEGIN)
		#define EXTREN_C extern "C"
	#else
		#define EXTREN_C
	#endif // defined(__cplusplus) && !defined(EXTREN_C_BEGIN)



EXTREN_C BAIDU_API typedef struct CStyle_BaiduTranslate TranslateObj;
EXTREN_C BAIDU_API typedef const char*					CString;



EXTREN_C BAIDU_API TranslateObj*						BaiduTranslate_Init(CString appid, CString appkey);

EXTREN_C BAIDU_API CString								BaiduTranslate_Translate(TranslateObj* p_translate_object,
																				 CString	   query,
																				 CString	   from,
																				 CString	   to,
																				 CString	   appid,
																				 CString	   appkey);

EXTREN_C BAIDU_API void BaiduTranslate_SetAppIDAndKey(TranslateObj* p_translate_object, CString appid, CString appkey);

EXTREN_C BAIDU_API CString BaiduTranslate_GetAppIDAndKey(TranslateObj* p_translate_object);

#endif // !BAIDUAPIEXPORT
