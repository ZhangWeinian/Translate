#pragma once

#include "__inter__AttributeDefinition.h"

/// <summary>
/// 便于在 C# 中调用接口，使用 const char* 代替 std::string
/// </summary>
using cstring = const char*;

extern "C"
{
	/// <summary>
	/// 用于初始化百度翻译 API
	/// </summary>
	/// <param name="appid">百度翻译的 AppID</param>
	/// <param name="appkey">对应的 key</param>
	/// <returns>返回一个指向实例的指针（ void* ）</returns>
	CONNECTAPIANDGUI_API void* BDTranslate_Begin(cstring appid, cstring appkey) noexcept;

	/// <summary>
	/// 用于释放百度翻译 API
	/// </summary>
	/// <param name="p">指向实例的指针（ void* ）</param>
	/// <returns>无返回值</returns>
	CONNECTAPIANDGUI_API void BDTranslate_End(void* p) noexcept;

	/// <summary>
	/// 用于设置百度翻译的 AppID 和 AppKey
	/// </summary>
	/// <param name="p">指向实例的指针（ void* ）</param>
	/// <param name="appid">百度翻译的 AppID</param>
	/// <param name="appkey">对应的 key</param>
	/// <returns>返回是否设置成功</returns>
	CONNECTAPIANDGUI_API bool BDTranslate_SetAppID(void* p, cstring appid, cstring appkey) noexcept;

	/// <summary>
	/// 用于翻译文本
	/// </summary>
	/// <param name="p">指向实例的指针（ void* ）</param>
	/// <param name="source">源文本</param>
	/// <param name="from">源语言</param>
	/// <param name="to">目标语言</param>
	/// <returns>返回翻译后的文本</returns>
	CONNECTAPIANDGUI_API cstring BDTranslate_Translate(void* p, cstring source, cstring from, cstring to) noexcept;

	/// <summary>
	/// 用于获取当前翻译器状态
	/// </summary>
	/// <param name="p">指向实例的指针（ void* ）</param>
	/// <returns>返回是否有错误</returns>
	CONNECTAPIANDGUI_API bool BDTranslate_isOK(void* p) noexcept;

	/// <summary>
	/// 用于获取当前翻译器错误信息（如果有的话）
	/// </summary>
	/// <param name="p">指向实例的指针（ void* ）</param>
	/// <returns>返回错误信息</returns>
	CONNECTAPIANDGUI_API cstring BDTranslate_whatHappened(void* p) noexcept;
}
