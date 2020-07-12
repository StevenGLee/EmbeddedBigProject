#ifndef READER_H
#define READER_H
#include <QTimer>

class reader
{
public:
int ad_zhi;
QTimer t5;
char buffer_send[MAXSIZE];
void ChangeSampleTime(int milliseconds);

};
#endif READER_H
