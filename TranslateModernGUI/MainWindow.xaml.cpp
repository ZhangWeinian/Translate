#include "pch.h"

#include "MainWindow.xaml.h"

#if __has_include("MainWindow.g.cpp")
	#include "MainWindow.g.cpp"
#endif

#include "winrt/base.h"
#include "winrt/impl/Microsoft.UI.Xaml.2.h"
#include "winrt/Windows.Foundation.h"

#include <cstdint>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::TranslateModernGUI::implementation
{
	int32_t MainWindow::MyProperty(void) const
	{
		throw hresult_not_implemented();
	}

	void MainWindow::MyProperty(int32_t /* value */) const
	{
		throw hresult_not_implemented();
	}

	void MainWindow::myButton_Click(const IInspectable &, const RoutedEventArgs &)
	{
		myButton().Content(box_value(L"Clicked"));
	}
}  // namespace winrt::TranslateModernGUI::implementation
