using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Master_WPF
{
    /// <summary>
    /// HotspotStrip.xaml 的交互逻辑
    /// </summary>
    /// 

    public partial class HotspotStrip : UserControl
    {
        public double VerticalPosition
        {
            get
            {
                return (double)GetValue(VerticalPositionProperty);
            }
            set
            {
                SetValue(VerticalPositionProperty, value);
            }
        }

        public readonly static DependencyProperty VerticalPositionProperty = DependencyProperty.Register("VerticalPosition", typeof(double),
            typeof(HotspotStrip), new PropertyMetadata((double)0, new PropertyChangedCallback(OnVerticalPositionChange)));
        static void OnVerticalPositionChange(DependencyObject sender, DependencyPropertyChangedEventArgs args)
        {
            HotspotStrip source = sender as HotspotStrip;
            TranslateTransform transform = new TranslateTransform(0, (double)args.NewValue);
            source.RenderTransform = transform;
        }
        private HotSpotDatas hotspotDatas;
        public HotSpotDatas HotspotDatas
        {
            set 
            {
                if (value.Count <= 0)
                    hotspotDatas = new HotSpotDatas();
                else
                    hotspotDatas = value;
                hotspotDatas.Sort();
                GrayPolygon.Points.Clear();
                GrayPolygon.Points.Add(new Point(0, CanvasBase.ActualHeight));
                GrayPolygon.Points.Add(new Point(0, CanvasBase.ActualHeight * (1-hotspotDatas[0].Hotness)));
                foreach (HotspotData data in hotspotDatas)
                    GrayPolygon.Points.Insert(GrayPolygon.Points.Count, new Point(CanvasBase.ActualWidth * data.TimeStamp, CanvasBase.ActualHeight * (1-data.Hotness)));
                GrayPolygon.Points.Add(new Point(GrayPolygon.ActualWidth, CanvasBase.ActualHeight * (1-hotspotDatas[hotspotDatas.Count - 1].Hotness)));
                GrayPolygon.Points.Add(new Point(GrayPolygon.ActualWidth, CanvasBase.ActualHeight));
            }
            get { return hotspotDatas; }
        }

        public HotspotStrip()
        {
            InitializeComponent();
            if (hotspotDatas == null)
                hotspotDatas = new HotSpotDatas();
        }

        public void CanvasBase_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (hotspotDatas.Count <= 0)
                hotspotDatas = new HotSpotDatas();
            hotspotDatas.Sort();
            GrayPolygon.Points.Clear();
            GrayPolygon.Points.Add(new Point(0, CanvasBase.ActualHeight));
            GrayPolygon.Points.Add(new Point(0, CanvasBase.ActualHeight * (1 - hotspotDatas[0].Hotness)));
            foreach (HotspotData data in hotspotDatas)
                GrayPolygon.Points.Insert(GrayPolygon.Points.Count, new Point(CanvasBase.ActualWidth * data.TimeStamp, CanvasBase.ActualHeight * (1 - data.Hotness)));
            GrayPolygon.Points.Add(new Point(GrayPolygon.ActualWidth, CanvasBase.ActualHeight * (1 - hotspotDatas[hotspotDatas.Count - 1].Hotness)));
            GrayPolygon.Points.Add(new Point(GrayPolygon.ActualWidth, CanvasBase.ActualHeight));
        }
    }
}
 