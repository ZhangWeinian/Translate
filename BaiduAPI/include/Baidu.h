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



/// <summary>
/// 初始化百度翻译API
/// </summary>
/// <returns>bool 表示的初始化状态，如果为 true ，表示初始化成功，可以后续工作</returns>
EXTREN_C BAIDU_API bool BaiduTranslate_Init(void);

/// <summary>
/// 翻译文本
/// </summary>
/// <param name="query">要翻译的文本</param>
/// <param name="from">原文语言类型</param>
/// <param name="to">目标语言类型</param>
/// <param name="appid">临时翻译 API APPID 。可以为空</param>
/// <param name="appkey">临时翻译 API APPKEY 。可以为空</param>
/// <returns>翻译结果。如果翻译失败，返回空字符串</returns>
EXTREN_C BAIDU_API const char* BaiduTranslate_Translate(const char* query,
														const char* from,
														const char* to,
														const char* appid,
														const char* appkey);

/// <summary>
/// 设置百度翻译 API 的 APPID 和 APPKEY
/// </summary>
/// <param name="appid">必填的翻译 API APPID</param>
/// <param name="appkey">必填的翻译 API APPKEY</param>
/// <returns>bool 表示的设置状态，如果为 true ，表示设置成功</returns>
EXTREN_C BAIDU_API bool BaiduTranslate_SetAppIDAndKey(const char* appid, const char* appkey);

/// <summary>
/// 获取百度翻译 API 的 APPID 和 APPKEY
/// </summary>
/// <returns>返回一个 JSON 字符串，包含 APPID 和 APPKEY</returns>
EXTREN_C BAIDU_API const char* BaiduTranslate_GetAppIDAndKey(void);

/// <summary>
/// 获取最后一次错误信息
/// </summary>
/// <returns>返回一个字符串，包含最后一次错误信息</returns>
EXTREN_C BAIDU_API const char* BaiduTranslate_GetLastError(void);


#endif // !BAIDU
