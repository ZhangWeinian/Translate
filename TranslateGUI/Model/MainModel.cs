using System;
using System.Runtime.InteropServices;

namespace TranslateGUI.Model
{
	public partial class MainModel
	{
		public MainModel()
		{
			CallTranslateFunc callTranslateFunc = new();
		}

		private class CallTranslateFunc
		{
			public CallTranslateFunc()
			{
				UseTranslationAPI();
			}

			[DllImport(@"E:\VS2022 Item\Translate\x64\Debug\ConnectAPIAndGui.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
			public static extern IntPtr BeginBaiduTranslate(string appid, string appkey);

			[DllImport(@"E:\VS2022 Item\Translate\x64\Debug\ConnectAPIAndGui.dll", CallingConvention = CallingConvention.Cdecl)]
			public static extern void EndBaiduTranslate(IntPtr instance);

			[DllImport(@"E:\VS2022 Item\Translate\x64\Debug\ConnectAPIAndGui.dll", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			public static extern bool SetAppID(IntPtr instance, string appid, string appkey);

			[DllImport(@"E:\VS2022 Item\Translate\x64\Debug\ConnectAPIAndGui.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
			public static extern IntPtr Translate(IntPtr instance, string source, string from, string to);

			[DllImport(@"E:\VS2022 Item\Translate\x64\Debug\ConnectAPIAndGui.dll", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			public static extern bool IsOK(IntPtr instance);

			[DllImport(@"E:\VS2022 Item\Translate\x64\Debug\ConnectAPIAndGui.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
			public static extern IntPtr whatHappened(IntPtr instance);

			private static string ConvertString(IntPtr ptr)
			{
				if(ptr == IntPtr.Zero)
					return null;

				try
				{
					return Marshal.PtrToStringAnsi(ptr);
				}
				finally
				{
					Marshal.FreeHGlobal(ptr);
				}
			}

			// 示例方法
			public static void UseTranslationAPI()
			{
				IntPtr instance = BeginBaiduTranslate("your_appid_here", "your_appkey_here");

				if(instance != IntPtr.Zero)
				{
					bool success = SetAppID(instance, "your_appid_here", "your_appkey_here");
					if(success && IsOK(instance))
					{
						// 获取翻译后的文本
						IntPtr translatedTextPtr = Translate(instance, "Hello World", "en", "zh");
						string translatedText = ConvertString(translatedTextPtr);
						Console.WriteLine($"Translated text: {translatedText}");
					}
					else
					{
						string error = ConvertString(whatHappened(instance));
						Console.WriteLine($"Error: {error}");
					}

					EndBaiduTranslate(instance);
				}
				else
				{
					Console.WriteLine("Failed to initialize translation API.");
				}
			}
		}
	}
}
