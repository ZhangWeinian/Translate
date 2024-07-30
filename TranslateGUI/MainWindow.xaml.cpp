#pragma once

#include "pch.h"

#include "MainWindow.xaml.h"

#if __has_include("MainWindow.g.cpp")
	#include "MainWindow.g.cpp"
#endif

#include <cstdint>

#include "winrt/base.h"
#include "winrt/impl/Microsoft.UI.Xaml.2.h"
#include "winrt/impl/Microsoft.UI.Xaml.Controls.2.h"
#include "winrt/Windows.Foundation.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::TranslateGUI::implementation
{
	int32_t MainWindow::MyProperty(void) const
	{
		throw hresult_not_implemented();
	}

	void MainWindow::MyProperty(int32_t /* value */) const
	{
		throw hresult_not_implemented();
	}
}  // namespace winrt::TranslateGUI::implementation

void winrt::TranslateGUI::implementation::MainWindow::ClickedToTranslate(
	[[maybe_unused]] const winrt::Windows::Foundation::IInspectable&	sender,
	[[maybe_unused]] const winrt::Microsoft::UI::Xaml::RoutedEventArgs& e)
{
	TranslateButton().Content(box_value(L"Clicked"));
}

void winrt::TranslateGUI::implementation::MainWindow::ChooseFrom(
	[[maybe_unused]] const winrt::Windows::Foundation::IInspectable&						sender,
	[[maybe_unused]] const winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs& e)
{}

void winrt::TranslateGUI::implementation::MainWindow::ChooseTo(
	[[maybe_unused]] const winrt::Windows::Foundation::IInspectable&						sender,
	[[maybe_unused]] const winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs& e)
{}

void winrt::TranslateGUI::implementation::MainWindow::ClickedToExchange(
	[[maybe_unused]] const winrt::Windows::Foundation::IInspectable&	sender,
	[[maybe_unused]] const winrt::Microsoft::UI::Xaml::RoutedEventArgs& e)
{}
