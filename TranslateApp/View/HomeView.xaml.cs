using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

using TranslateApp.ViewModel;

// To learn more about WinUI, the WinUI project structure, and more about our project templates,
// see: http://aka.ms/winui-project-info.

namespace TranslateApp.View
{
	using WinUIColor = Windows.UI.Color;

	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class HomeView : Page
	{
		public HomeViewModel HomeVM { set; get; } = new();

		public HomeView()
		{
			this.InitializeComponent();
		}

		private void OnFocusChanged(object sender, WindowActivatedEventArgs args)
		{
			var hasFocus = (args.WindowActivationState != WindowActivationState.Deactivated);

			HomeVM.BorderBrushColor = hasFocus
				? new(WinUIColor.FromArgb(255, 3, 150, 255))
				: new(WinUIColor.FromArgb(255, 201, 222, 245));
		}

		private void ClickedTotranslate(object sender, RoutedEventArgs e)
		{
			HomeVM.TranslateAns = "ok! " + HomeVM.TranslateSource;
		}

		private void ClickedToExchanged(object sender, RoutedEventArgs e)
		{
			// Method intentionally left empty.
		}
	}
}
