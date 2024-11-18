using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Input;

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
			this.AppWindow.Resize(new Windows.Graphics.SizeInt32 { Width = 860, Height = 1100 });

			// 隐藏原生标题栏
			this.ExtendsContentIntoTitleBar = true;

			// 设置自定义标题栏
			this.SetTitleBar(this.AppTitleBar);

			// 源语言类型默认为 "自动检测"
			this.From.SelectedItem = this.From.Items[0];

			// 目标语言类型默认为 "中文"
			this.To.SelectedItem = this.To.Items[0];

			// 添加键盘事件处理程序
			this.Content.KeyDown += MainView_KeyDown; // Change this line
		}

		private void MainView_KeyDown(object sender, KeyRoutedEventArgs e)
		{
			// 监听键盘，当检测到 Ctrl + Enter 时触发翻译
			if(e.Key == Windows.System.VirtualKey.Enter &&
			Microsoft.UI.Input.InputKeyboardSource.GetKeyStateForCurrentThread(Windows.System.VirtualKey.Control).HasFlag(Windows.UI.Core.CoreVirtualKeyStates.Down))
			{
				this.Translate_Click();
			}
		}

		private void Switch_Click()
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

		private void Translate_Click()
		{
			string query = this.InputTextBox.Text;

			if(string.IsNullOrEmpty(query))
			{
				return;
			}

			string form = "";
			string to = "";

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

			_ = ViewModel.Translate(query, form, to);

			this.InputTextBox.Text = "";
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
