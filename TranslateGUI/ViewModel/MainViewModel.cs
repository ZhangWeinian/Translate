using CommunityToolkit.Mvvm.ComponentModel;

using Microsoft.UI.Xaml.Media;


namespace TranslateGUI.ViewModel
{
	using WinUIColor = Windows.UI.Color;

	public partial class MainViewModel : ObservableObject
	{
		[ObservableProperty]
		private SolidColorBrush borderBrushColor = new(WinUIColor.FromArgb(255, 3, 150, 255));

		[ObservableProperty]
		private int defaultFontSize = 16;

		[ObservableProperty]
		private string translateSource =  string.Empty;

		[ObservableProperty]
		private string translateAns = string.Empty;

		public MainViewModel()
		{
		}
	}
}
