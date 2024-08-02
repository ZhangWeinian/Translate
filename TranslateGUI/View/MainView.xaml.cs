using Microsoft.UI;
using Microsoft.UI.Xaml;

using TranslateGUI.Model;
using TranslateGUI.ViewModel;

using WinRT.Interop;


namespace TranslateGUI.View
{
	using MainWindow = Microsoft.UI.Windowing.AppWindow;
	using WinUIColor = Windows.UI.Color;

	/// <summary>
	/// An empty window that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainView : Window
	{
		public MainViewModel MainVM { get; set; } = new MainViewModel();

		public MainModel MainM { get; set; } = new MainModel();

		private readonly MainWindow m_AppWindow;

		public MainView()
		{
			this.InitializeComponent();

			m_AppWindow = GetAppWindowForCurrentWindow();

			m_AppWindow.Resize(new(1600, 1350));

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

			MainVM.BorderBrushColor = hasFocus
				? new(WinUIColor.FromArgb(255, 3, 150, 255))
				: new(WinUIColor.FromArgb(255, 201, 222, 245));
		}

		private void ClickedTotranslate(object sender, RoutedEventArgs e)
		{
			MainVM.TranslateAns = "ok! " + MainVM.TranslateSource;
		}

		private void ClickedToExchanged(object sender, RoutedEventArgs e)
		{
		}
	}
}
