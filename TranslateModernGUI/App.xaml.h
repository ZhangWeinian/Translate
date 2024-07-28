#pragma once

#include "App.xaml.g.h"
#include "winrt/impl/Microsoft.UI.Xaml.2.h"

namespace winrt::TranslateModernGUI::implementation
{
	struct App: AppT<App>
	{
		App();

		void OnLaunched(const Microsoft::UI::Xaml::LaunchActivatedEventArgs &);

	private:
		winrt::Microsoft::UI::Xaml::Window window { nullptr };
	};
}  // namespace winrt::TranslateModernGUI::implementation
