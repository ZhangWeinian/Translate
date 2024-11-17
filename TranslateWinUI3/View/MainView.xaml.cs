using System.Threading.Tasks;

using Microsoft.UI.Xaml;

using TranslateWinUI3.ViewModel;

// To learn more about WinUI, the WinUI project structure, and more about our project templates,
// see: http://aka.ms/winui-project-info.

namespace TranslateWinUI3.View
{
	/// <summary>
	/// An empty window that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainView : Window
	{
		public MainViewModel ViewModel { get; } = new MainViewModel();

		public MainView()
		{
			this.InitializeComponent();

			// 设置默认窗口大小
			this.AppWindow.Resize(new Windows.Graphics.SizeInt32 { Width = 900, Height = 1200 });

			// 隐藏原生标题栏
			this.ExtendsContentIntoTitleBar = true;

			// 设置自定义标题栏
			this.SetTitleBar(this.AppTitleBar);

			// 设置 from 和 to 的默认语言
			this.From.SelectedItem = this.From.Items[0];
			this.To.SelectedItem = this.To.Items[0];
		}

		private void Switch_Tapped(object sender, RoutedEventArgs e)
		{
			if(this.To.SelectedItem == this.To.Items[0])
			{
				this.To.SelectedItem = this.To.Items[1];
				this.From.SelectedItem = this.From.Items[2];
			}
			else
			{
				this.To.SelectedItem = this.To.Items[0];
				this.From.SelectedItem = this.From.Items[1];
			}
		}

		private async void Run_Tapped(object sender, Microsoft.UI.Xaml.Input.TappedRoutedEventArgs e)
		{
			string query = InputTextBox.Text;

			if(string.IsNullOrEmpty(query))
			{
				return;
			}

			string form="";
			string to="";

			if(this.From.SelectedItem == this.From.Items[0])
			{
				form = "auto";
			}
			else if(this.From.SelectedItem == this.From.Items[1])
			{
				form = "en";
			}
			else if(this.From.SelectedItem == this.From.Items[2])
			{
				form = "zh";
			}

			if(this.To.SelectedItem == this.To.Items[0])
			{
				to = "zh";
			}
			else if(this.To.SelectedItem == this.To.Items[1])
			{
				to = "en";
			}

			await Task.Run(() =>
			{
				string result= "Translating..." + form + "|||" + to;
				this.OutputTextBlock.Text = result;
			});
		}

		private void From_SelectionChanged()
		{
			if(this.From.SelectedItem == this.From.Items[1])
			{
				this.To.SelectedItem = this.To.Items[0];
			}

			if(this.From.SelectedItem == this.From.Items[2])
			{
				this.To.SelectedItem = this.To.Items[1];
			}
		}

		private void To_SelectionChanged()
		{
			if((this.From.SelectedItem != this.From.Items[0]) && (this.To.SelectedItem == this.To.Items[0]))
			{
				this.From.SelectedItem = this.From.Items[1];
			}

			if((this.From.SelectedItem != this.From.Items[0]) && (this.To.SelectedItem == this.To.Items[1]))
			{
				this.From.SelectedItem = this.From.Items[2];
			}
		}
	}
}
