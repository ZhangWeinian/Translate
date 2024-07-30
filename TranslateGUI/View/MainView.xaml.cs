using System;

using Microsoft.UI;
using Microsoft.UI.Xaml;

using TranslateGUI.ViewModel;

using WinRT.Interop;

// To learn more about WinUI, the WinUI project structure, and more about our project templates,
// see: http://aka.ms/winui-project-info.

using MainWindow = Microsoft.UI.Windowing.AppWindow;

namespace TranslateGUI.View
{
	/// <summary>
	/// An empty window that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainView : Window
	{
		public MainViewModel MainVM { get; set; } = new MainViewModel();

		private readonly MainWindow m_AppWindow;

		public MainView()
		{
			this.InitializeComponent();

			m_AppWindow = GetAppWindowForCurrentWindow();

			m_AppWindow.Resize(new(1300, 1500));

			this.Activated += OnFocusChanged;
		}

		private MainWindow GetAppWindowForCurrentWindow()
		{
			IntPtr hWnd = WindowNative.GetWindowHandle(this);

			WindowId myWndId = Win32Interop.GetWindowIdFromWindow(hWnd);

			return MainWindow.GetFromWindowId(myWndId);
		}

		private void OnFocusChanged(object sender, WindowActivatedEventArgs args)
		{
			var hasFocus = (args.WindowActivationState != WindowActivationState.Deactivated);

			MainVM.BorderBrushColor = hasFocus
				? new(Windows.UI.Color.FromArgb(255, 3, 150, 255))
				: new(Windows.UI.Color.FromArgb(255, 126, 201, 255));
		}
	}
}
