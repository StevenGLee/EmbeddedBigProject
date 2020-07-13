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
    [Serializable]
    public class SequenceData: HotSpotDatas
    {
        public int maxCount;
        public int seqCount 
        {
            get;
            private set;
        }
        List<int> buffer;
        int maximum, minimum;

        public SequenceData(int MaxCount, int Maximum, int Minimum)
        {
            Clear();
            maxCount = MaxCount;
            maximum = Maximum;
            minimum = Minimum;
            buffer = new List<int>();
            AddSequenceData(minimum);
        }
        public void AddSequenceData(int data)
        {
            buffer.Add(data);
            if (seqCount > maxCount)
            {
                while (buffer.Count > maxCount)
                    buffer.RemoveAt(0);
                seqCount = maxCount;
            }
            else if(seqCount == maxCount)
                buffer.RemoveAt(0);
            else
                seqCount++;
            Clear();
            for(int i=0;i<seqCount;i++)
                Add(new HotspotData(((double)i)/(seqCount-1), ((double)(buffer[i]-minimum))/(maximum-minimum)));
        }
        public new void SaveTo(Uri uri)
        {
            SaveTo(uri.ToString());
        }
        /// <summary>
        /// 保存至uriString（即文件名）。
        /// </summary>
        /// <param name="uriString"></param>
        public new void SaveTo(string uriString)
        {
            BinaryFormatter bf = new BinaryFormatter();
            FileStream fileStream = File.Create(uriString);
            bf.Serialize(fileStream, this);
            fileStream.Close();
        }
        public new void ReadFrom(Uri uri)
        {
            ReadFrom(uri.ToString());
        }
        public new void ReadFrom(string uriString)
        {
            if (File.Exists(uriString))
            {
                BinaryFormatter bf = new BinaryFormatter();
                FileStream filestream = File.Open(uriString, FileMode.Open);
                SequenceData temp = (SequenceData)bf.Deserialize(filestream);
                Clear();
                buffer.Clear();
                foreach (HotspotData hotspotData in temp)
                    Add(hotspotData);
                maxCount = temp.maxCount;
                maximum = temp.maximum;
                minimum = temp.minimum;
                buffer = temp.buffer;
                seqCount = temp.seqCount;
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
