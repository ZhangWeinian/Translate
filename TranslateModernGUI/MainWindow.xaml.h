#pragma once

#include "MainWindow.g.h"
#include "MainWindow.xaml.g.h"
#include "winrt/base.h"
#include "winrt/impl/Microsoft.UI.Xaml.2.h"

#include <cstdint>

namespace winrt::TranslateModernGUI::implementation
{
	struct MainWindow: MainWindowT<MainWindow>
	{
		// Xaml objects should not call InitializeComponent during construction.
		// See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

		MainWindow(void) = default;

		int32_t MyProperty(void) const;

		void	MyProperty(int32_t value) const;

		void	myButton_Click(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
	};
}  // namespace winrt::TranslateModernGUI::implementation

namespace winrt::TranslateModernGUI::factory_implementation
{
	struct MainWindow: MainWindowT<MainWindow, implementation::MainWindow>
	{};
}  // namespace winrt::TranslateModernGUI::factory_implementation
