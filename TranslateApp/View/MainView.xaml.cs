using Microsoft.UI;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

using TranslateApp.ViewModel;

using WinRT.Interop;

// To learn more about WinUI, the WinUI project structure, and more about our project templates,
// see: http://aka.ms/winui-project-info.

namespace TranslateApp.View
{
	using MainWindow = Microsoft.UI.Windowing.AppWindow;
	using WinUIColor = Windows.UI.Color;

	/// <summary>
	/// An empty window that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainView : Window
	{
		public HomeViewModel HomeVM { get; set; } = new();

		private readonly MainWindow m_AppWindow;

		public MainView()
		{
			this.InitializeComponent();

			ExtendsContentIntoTitleBar = true;

			m_AppWindow = GetAppWindowForCurrentWindow();

			m_AppWindow.Resize(new(1000, 1250));

			this.RootNavView.SelectedItem = this.RootNavView.MenuItems[0];

			this.ContentFrame.Navigate(typeof(HomeView));

			this.Activated += OnFocusChanged;
		}

		private MainWindow GetAppWindowForCurrentWindow()
		{
			System.IntPtr hWnd = WindowNative.GetWindowHandle(this);

			WindowId myWndId = Win32Interop.GetWindowIdFromWindow(hWnd);

			return MainWindow.GetFromWindowId(myWndId);
		}

		private void OnFocusChanged(object sender, WindowActivatedEventArgs args)
		{
			var hasFocus = (args.WindowActivationState != WindowActivationState.Deactivated);

			HomeVM.BorderBrushColor = hasFocus
				? new(WinUIColor.FromArgb(255, 3, 150, 255))
				: new(WinUIColor.FromArgb(255, 201, 222, 245));
		}

		private void PageInvoked(NavigationView sender, NavigationViewItemInvokedEventArgs args)
		{
			if(args.InvokedItemContainer is NavigationViewItem item)
			{
				switch(item.Tag as string)
				{
					case "DefaultPage":
						this.ContentFrame.Navigate(typeof(HomeView));
						break;

					case "SetAPI":
						this.ContentFrame.Navigate(typeof(SetAPIView));
						break;

					case "LookingLog":
						this.ContentFrame.Navigate(typeof(LogView));
						break;

					case "LookingMore":
						this.ContentFrame.Navigate(typeof(ErrinfoView));
						break;

					default:
						break;
				}
			}
		}
	}
}
