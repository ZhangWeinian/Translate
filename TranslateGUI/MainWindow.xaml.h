#pragma once

#include <cstdint>

#include "MainWindow.g.h"
#include "MainWindow.xaml.g.h"
#include "winrt/base.h"
#include "winrt/impl/Microsoft.UI.Xaml.2.h"
#include "winrt/impl/Microsoft.UI.Xaml.Controls.2.h"

#if defined(_HAS_CXX20)
	#include "../TranslateAPI/TranslateAPI.h"
#endif	// defined(_HAS_CXX20)


namespace winrt::TranslateGUI::implementation
{
	struct MainWindow: MainWindowT<MainWindow>
	{
		MainWindow(void) = default;

		int32_t			  MyProperty(void) const;

		[[noreturn]] void MyProperty(int32_t value) const;

		/// <summary>
		/// 点击以翻译
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		/// <returns>无返回值</returns>
		void ClickedToTranslate(const winrt::Windows::Foundation::IInspectable&	   sender,
								const winrt::Microsoft::UI::Xaml::RoutedEventArgs& e);

		/// <summary>
		/// 选择原文语言类型
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		/// <returns>无返回值</returns>
		void ChooseFrom(const winrt::Windows::Foundation::IInspectable&						   sender,
						const winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs& e);

		/// <summary>
		/// 选择目标语言类型
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		/// <returns>无返回值</returns>
		void ChooseTo(const winrt::Windows::Foundation::IInspectable&						 sender,
					  const winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs& e);

		/// <summary>
		/// 点击以交换原文语言类型和目标语言类型
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		/// <returns>无返回值</returns>
		void ClickedToExchange(const winrt::Windows::Foundation::IInspectable&	  sender,
							   const winrt::Microsoft::UI::Xaml::RoutedEventArgs& e);
	};
}  // namespace winrt::TranslateGUI::implementation

namespace winrt::TranslateGUI::factory_implementation
{
	struct MainWindow: MainWindowT<MainWindow, implementation::MainWindow>
	{};
}  // namespace winrt::TranslateGUI::factory_implementation
