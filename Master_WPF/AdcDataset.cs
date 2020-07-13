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
    public class AdcDatasetItem
    {
        public string time;
        public string Time
        {
            get
            {
                return time;
            }
        }
        public int adcValue;
        public string AdcValue
        {
            get
            {
                return adcValue.ToString();
            }
        }
        public AdcDatasetItem() { }
        public AdcDatasetItem(string Time, int AdcValue)
        {
            time = Time;
            adcValue = AdcValue;
        }
    }
    [Serializable]
    public class AdcDataset : List<AdcDatasetItem>
    {
        public AdcDataset():base() { }
        public AdcDataset(AdcDataset oldDataset)
            :base(oldDataset){ }
        public void SaveTo(Uri uri)
        {
            SaveTo(uri.ToString());
        }
        /// <summary>
        /// 保存至uriString（即文件名）。
        /// </summary>
        /// <param name="uriString"></param>
        public void SaveTo(string uriString)
        {
            BinaryFormatter bf = new BinaryFormatter();
            FileStream fileStream = File.Create(uriString);
            bf.Serialize(fileStream, this);
            fileStream.Close();
        }
        public static AdcDataset ReadFrom(Uri uri)
        {
            return ReadFrom(uri.ToString());
        }
        public static AdcDataset ReadFrom(string uriString)
        {
            if (File.Exists(uriString))
            {
                BinaryFormatter bf = new BinaryFormatter();
                FileStream filestream = File.Open(uriString, FileMode.Open);
                AdcDataset ret =  bf.Deserialize(filestream) as AdcDataset;
                filestream.Close();
                return ret;
            }
            else
            {
                //Debug.Log("不存在存档");
                MessageBox.Show("不存在存档");
                return null;
            }
        }
        

    }
}
