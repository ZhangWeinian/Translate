#pragma once

#include <string>

namespace CLREncapsulation
{
public

	ref class UsingBDTranslateStatic
	{
	public:

		/// <summary>
		/// 调用此函数初始化使用百度翻译 API 的翻译器
		/// </summary>
		/// <param name="appid">百度翻译 AppID</param>
		/// <param name="appkey">对应的 key</param>
		/// <returns>无返回值</returns>
		static void BDTranslate(System::String ^ appid, System::String ^ appkey);

		/// <summary>
		/// 调用此函数设置百度翻译 API 的 AppID 和 AppKey
		/// </summary>
		/// <param name="appid">百度翻译 AppID</param>
		/// <param name="appkey">对应的 key</param>
		/// <returns>返回值确认是否设置成功</returns>
		static bool SetAppID(System::String ^ appid, System::String ^ appkey);

		/// <summary>
		/// 此函数调用百度翻译 API 进行翻译
		/// </summary>
		/// <param name="source">原文</param>
		/// <param name="from">原文语言类型</param>
		/// <param name="to">目标语言类型</param>
		/// <returns>翻译后的译文 或 错误信息</returns>
		static System::String ^ Translate(System::String ^ source, System::String ^ from, System::String ^ to);

		/// <summary>
		/// 检查 __inter__BDTranslate 对象的运行状态是否正确
		/// </summary>
		/// <param name="">无参数</param>
		/// <returns>返回值表示当前状态是否正确</returns>
		static bool isOK(void);

		/// <summary>
		/// 获取 __inter__BDTranslate 对象的运行状态的详细信息
		/// </summary>
		/// <param name="">无参数</param>
		/// <returns>返回运行状态的详细信息，空值表示无异常</returns>
		static System::String ^ whatHappened(void);

	private:

		/// <summary>
		/// 把 const char* 转换为 System::String
		/// </summary>
		/// <param name="str">const char* 字符串</param>
		/// <returns>转换后的 System::String 字符串</returns>
		static System::String ^ StdstrToSysstr(const char* str);

		/// <summary>
		/// 把 System::String 转换为 const char*
		/// </summary>
		/// <param name="str">System::String 字符串</param>
		/// <returns>转换后的 const char* 字符串</returns>
		static const char* SysstrToStdstr(System::String ^ str);

		static System::String ^ m_str {};
	};
}  // namespace CLREncapsulation
