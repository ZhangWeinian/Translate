#pragma once

#ifndef TRANSLATE

	#define TRANSLATE

	#ifdef TRANSLATEAPI_EXPORTS
		#define TRANSLATE_API __declspec(dllexport)
	#else
		#define TRANSLATE_API __declspec(dllimport)
	#endif // TRANSLATE_EXPORTS

	#if defined(__cplusplus) && !defined(EXTREN_C_BEGIN)
		#define EXTREN_C extern "C"
	#else
		#define EXTREN_C
	#endif // defined(__cplusplus) && !defined(EXTREN_C_BEGIN)



EXTREN_C typedef struct CTranslateFunction TranslateFunction;

EXTREN_C typedef const char*			   CString;

EXTREN_C TRANSLATE_API TranslateFunction*  InitTranslateFunction(CString appid, CString appkey);

EXTREN_C TRANSLATE_API CString			   Translate(TranslateFunction* p_translate_function,
													 CString			query,
													 CString			from,
													 CString			to,
													 CString			appid,
													 CString			appkey);

EXTREN_C TRANSLATE_API void	   SetAppIDAndKey(TranslateFunction* p_translate_function, CString appid, CString appkey);

EXTREN_C TRANSLATE_API CString GetAppIDAndKey(TranslateFunction* p_translate_function);

#endif // !TRANSLATE
