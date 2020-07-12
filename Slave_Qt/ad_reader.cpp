#include"ad_reader.h"


void ad_reader::init()
{
    adc= "/dev/adc";
    if ((fd_adc = open(adc, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
	{
		printf("open ADC error\n");

	}

}

int ad_reader::ad()
{
    memset(buffer, 0, sizeof(buffer));
    int len=0;
    len = read(fd_adc, buffer, 10);
    if(len == 0)
    {
		printf("ADC read error \n");
                return 0;
    }
	else {
		r = atoi(buffer);
		r = (int)(r * 10000 / 4096);//传递过来的数据为浮点型 ，进行转换和去分辨率4096
		return r;
    }

}
