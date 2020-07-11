using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Master_WPF
{
    /// <summary>
    /// 高能进度条单个数据
    /// </summary>
    [Serializable]
    public class HotspotData : IComparable
    {
        public double TimeStamp, Hotness;
        /// <summary>
        /// 高能进度条单个数据构造函数
        /// </summary>
        /// <param name="timeStamp">时间戳，为[0,1]的double类型数据，即该时间戳的时间点t在总时长T中的位置，即t/T的值</param>
        /// <param name="hotness">热度值，为[0,1]的double类型数据</param>
        public HotspotData(double timeStamp, double hotness)
        {
            TimeStamp = timeStamp;
            Hotness = hotness;
        }
        public int CompareTo(object obj)
        {
            HotspotData data = obj as HotspotData;
            return TimeStamp.CompareTo(data.TimeStamp);
        }
    }

    [Serializable]
    public class HotSpotDatas : List<HotspotData>
    {
        /// <summary>
        /// 高能进度条数据集默认构造函数
        /// </summary>
        public HotSpotDatas()
        {
            Add(new HotspotData(0, 0));
        }
        /// <summary>
        /// 复制构造函数
        /// </summary>
        /// <param name="hotSpotDatas"></param>
        public HotSpotDatas(HotSpotDatas hotSpotDatas)
        {
            foreach (HotspotData hotspotData in hotSpotDatas)
                Add(hotspotData);
        }
        public void SaveTo(Uri uri)
        {
            SaveTo(uri.ToString());
        }
        /// <summary>
        /// 保存至uriString（即文件名）。建议存储格式为课程名-章节名.hst
        /// </summary>
        /// <param name="uriString"></param>
        public void SaveTo(string uriString)
        {
            BinaryFormatter bf = new BinaryFormatter();
            FileStream fileStream = File.Create(uriString);
            bf.Serialize(fileStream, this);
            fileStream.Close();
        }
        public void ReadFrom(Uri uri)
        {
            ReadFrom(uri.ToString());
        }
        public void ReadFrom(string uriString)
        {
            if (File.Exists(uriString))
            {
                BinaryFormatter bf = new BinaryFormatter();
                FileStream filestream = File.Open(uriString, FileMode.Open);
                Clear();
                foreach(HotspotData hotspotData in (HotSpotDatas)bf.Deserialize(filestream))
                    Add(hotspotData);
                filestream.Close();
            }
            else
            {
                //Debug.Log("不存在存档");
                MessageBox.Show("不存在存档");
            }
        }
    }
}
