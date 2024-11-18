using System.Threading.Tasks;

using CommunityToolkit.Mvvm.ComponentModel;

namespace TranslateWinUI3.ViewModel
{
	public partial class MainViewModel : ObservableObject
	{
		public MainViewModel()
		{
		}

		[ObservableProperty]
		private double inputFontSize = 18; // 初始字体大小

		[ObservableProperty]
		private string translatedText = string.Empty; // 翻译结果

		internal async Task<bool> Translate(string query, string form, string to)
		{
			await Task.Run(() =>
			{
				TranslatedText = "Translating..." + query + "|||" + form + "|||" + to;
			});

			return true;
		}
	}
}
