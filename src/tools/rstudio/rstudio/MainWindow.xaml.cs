using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;

namespace rstudio
{
    public class Rush
    {
        [DllImport("rboot_d.dll")]
        public static extern void RushInit(string cmdline);

        [DllImport("rboot_d.dll")]
        public static extern IntPtr RushGetBackBuffer();

        [DllImport("rboot_d.dll")]
        public static extern void RushUpdate();

        [DllImport("rboot_d.dll")]
        public static extern void RushResize(int w, int h);

        [DllImport("rboot_d.dll")]
        public static extern void RushShut();
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private D3DImage    RenderImage;
        private IntPtr      RenderSurface;

        public MainWindow()
        {
            RenderImage = new D3DImage();
            RenderImage.IsFrontBufferAvailableChanged
                += new DependencyPropertyChangedEventHandler(OnIsFrontBufferAvailableChanged);

            Resources["RenderSurface"] = new ImageBrush(RenderImage);

            string cmdLine = "--media=\"c:\\home\\proj\\rview\\media\\\"";
            Rush.RushInit(cmdLine);

            BeginRenderingScene();

            InitializeComponent();
        }

        private void OnIsFrontBufferAvailableChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            // if the front buffer is available, then WPF has just created a new
            // D3D device, so we need to start rendering our custom scene
            if (RenderImage.IsFrontBufferAvailable)
            {
                BeginRenderingScene();
            }
            else
            {
                // If the front buffer is no longer available, then WPF has lost its
                // D3D device so there is no reason to waste cycles rendering our
                // custom scene until a new device is created.
                StopRenderingScene();
            }
        }

        private void BeginRenderingScene()
        {
            RenderSurface = Rush.RushGetBackBuffer();
            if (RenderImage.IsFrontBufferAvailable && RenderSurface != IntPtr.Zero)
            {

                // set the back buffer using the new scene pointer
                RenderImage.Lock();
                RenderImage.SetBackBuffer(D3DResourceType.IDirect3DSurface9, RenderSurface);
                RenderImage.Unlock();

                // leverage the Rendering event of WPF's composition target to
                // update the custom D3D scene
                CompositionTarget.Rendering += OnRendering;
            }
        }

        private void StopRenderingScene()
        {
            // This method is called when WPF loses its D3D device.
            // In such a circumstance, it is very likely that we have lost 
            // our custom D3D device also, so we should just release the scene.
            // We will create a new scene when a D3D device becomes 
            // available again.
            CompositionTarget.Rendering -= OnRendering;
            Rush.RushShut();
            RenderSurface = IntPtr.Zero;
        }

        private void OnRendering(object sender, EventArgs e)
        {
            // when WPF's composition target is about to render, we update our 
            // custom render target so that it can be blended with the WPF target
            UpdateScene();
        }

        private void UpdateScene()
        {
            if (RenderImage.IsFrontBufferAvailable)
            {
                int w = System.Convert.ToInt32(RenderTarget.ActualWidth);
                int h = System.Convert.ToInt32(RenderTarget.ActualHeight);

                if (w == 0 || h == 0)
                {
                    return;
                }

                RenderImage.Lock();

                if (RenderImage.PixelWidth != w || RenderImage.PixelHeight != h)
                {
                    RenderImage.SetBackBuffer( D3DResourceType.IDirect3DSurface9, IntPtr.Zero );
                    Rush.RushResize(w, h);
                    RenderSurface = Rush.RushGetBackBuffer();
                    RenderImage.SetBackBuffer( D3DResourceType.IDirect3DSurface9, RenderSurface );
                }

                Rush.RushUpdate();
                
                if (RenderSurface != IntPtr.Zero)
                {
                    Int32Rect rect = new Int32Rect( 0, 0, w, h );
                    RenderImage.AddDirtyRect( rect );
                }
                RenderImage.Unlock();
            }
        }
    }
}
