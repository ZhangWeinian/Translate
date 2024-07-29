#pragma once

#include <winrt/impl/Microsoft.UI.Xaml.2.h>
#include <cstdint>

#include "MainWindow.g.h"
#include "MainWindow.xaml.g.h"
#include "winrt/base.h"

namespace winrt::TranslateGUI::implementation
{
	struct MainWindow: MainWindowT<MainWindow>
	{
		MainWindow(void) = default;

		int32_t			  MyProperty(void) const;

		[[noreturn]] void MyProperty(int32_t value) const;

		void			  myButton_Click(const winrt::Windows::Foundation::IInspectable&	sender,
										 const winrt::Microsoft::UI::Xaml::RoutedEventArgs& e);
	};
}  // namespace winrt::TranslateGUI::implementation

namespace winrt::TranslateGUI::factory_implementation
{
	struct MainWindow: MainWindowT<MainWindow, implementation::MainWindow>
	{};
}  // namespace winrt::TranslateGUI::factory_implementation
