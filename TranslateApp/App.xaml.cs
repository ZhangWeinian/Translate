using System;

using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Navigation;

// To learn more about WinUI, the WinUI project structure, and more about our project templates,
// see: http://aka.ms/winui-project-info.

namespace TranslateApp
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	public partial class App : Application
	{
		/// <summary>
		/// Initializes the singleton application object. This is the first line of authored code
		/// executed, and as such is the logical equivalent of main() or WinMain().
		/// </summary>
		public App()
		{
			this.InitializeComponent();
		}

		/// <summary>
		/// Invoked when the application is launched.
		/// </summary>
		/// <param name="args">Details about the launch request and process.</param>
		protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
		{
			m_window = new View.MainView();

			//// 创建一个框架作为导航上下文并导航到第一页
			//Frame rootFrame = new();
			//rootFrame.NavigationFailed += OnNavigationFailed;

			//// 导航到第一页，通过将所需信息作为导航参数传递来配置新页面
			//rootFrame.Navigate(typeof(View.SetAPIView), args.Arguments);

			//// 将框架放置在当前窗口中
			//m_window.Content = rootFrame;

			// 确保主窗口处于活动状态
			m_window.Activate();
		}

		private void OnNavigationFailed(object sender, NavigationFailedEventArgs e) => throw new Exception("Failed to load Page " + e.SourcePageType.FullName);

		private Window m_window;
	}
}
