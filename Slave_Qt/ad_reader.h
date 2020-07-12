#ifndef AD_READER_H
#define AD_READER_H

#include <stdio.h>
//字符串操作函数头文件
#include <string.h>

#include <unistd.h>
//文件操作函数头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


class ad_reader
{
public:
    int fd_adc;
    char *adc;
    char buffer[512];
    int r;
    void init();
    int ad();
};
#endif AD_READER_H
