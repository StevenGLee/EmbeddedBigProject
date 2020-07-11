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

class ad_reader
{
private:
    int fd_adc;
    char *adc = "/dev/adc";
    char buffer[512];
public:
    void init();
    int ad_reader();
};
#endif AD_READER_H
