#include"reader.h"
#include"qt1.h"
#include"camera.h"
#include"ad_reader.h"

void reader::ChangeSampleTime(int milliseconds){
    int milliseconds;
    ad_reader AD;
    AD.Initial();
    t5.start(milliseconds);
    AD.reader();
    camera Cam;
    Cam.fun_take_phtoto();

}