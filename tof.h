#ifndef TOF_H
#define TOF_H
#include <main.h>

#define TOF_THRESHOLD 400
#define TOO_FAR 600
#define TOF_FILTER_N 8
class TOF_CLASS
{
public:
    float alpha = 0.5;
    int _last = TOO_FAR;
    int _lastlast = TOO_FAR;
    int _Tofget = TOO_FAR;
    int filter_buf[TOF_FILTER_N + 1];
    int Filter_avg = TOO_FAR;
    int lowFir = TOO_FAR;
    bool tof_fsm(HardwareSerial &A);
};

void Filter(TOF_CLASS *tof);
extern TOF_CLASS tof;
void get_tof(HardwareSerial &A, TOF_CLASS *tof);
#endif