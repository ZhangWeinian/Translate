#pragma once

#include "pch.h"

#include <debugapi.h>
#include <intrin.h>

#include "App.xaml.h"
#include "MainWindow.xaml.h"
#include "winrt/base.h"
#include "winrt/impl/Microsoft.UI.Xaml.2.h"


using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::TranslateGUI::implementation
{
	/// <summary>
	/// Initializes the singleton application object.  This is the first line of authored code
	/// executed, and as such is the logical equivalent of main() or WinMain().
	/// </summary>
	App::App()
	{
		// Xaml objects should not call InitializeComponent during construction.
		// See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
		UnhandledException(
			[](const IInspectable &, const UnhandledExceptionEventArgs &e)
			{
				if (IsDebuggerPresent())
				{
					auto errorMessage = e.Message();
					__debugbreak();
				}
			});
#endif
	}

	/// <summary>
	/// Invoked when the application is launched.
	/// </summary>
	/// <param name="e">Details about the launch request and process.</param>
	void App::OnLaunched([[maybe_unused]] const LaunchActivatedEventArgs &e)
	{
		window = make<MainWindow>();
		window.Activate();
	}
}  // namespace winrt::TranslateGUI::implementation
