using CommunityToolkit.Mvvm.ComponentModel;

using Microsoft.UI.Xaml.Media;

namespace TranslateGUI.ViewModel
{
	public partial class MainViewModel : ObservableObject
	{
		[ObservableProperty]
		private SolidColorBrush borderBrushColor= new(Windows.UI.Color.FromArgb(255, 3, 150, 255));

		public MainViewModel()
		{
		}
	}
}
