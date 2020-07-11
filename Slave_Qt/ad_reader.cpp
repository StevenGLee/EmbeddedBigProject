#include"ad_reader.h"

void ad_reader::init()
{
    if ((fd_adc = open(adc, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
	{
		printf("open ADC error\n");
		return 0;
	}

}
int ad_reader::ad_reader()
{
    int len=0;
    len = read(fd_adc, buffer, 10);
    if(len == 0)
    {
		printf("ADC read error \n");
		return;
    }
	else {
		r = atoi(buffer);
		r = (int)(r * 10000 / 4096);//传递过来的数据为浮点型 ，进行转换和去分辨率4096
		return r;
    }

}