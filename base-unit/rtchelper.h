#ifndef RTCHELPER_H
#define RTCHELPER_H

#include <time.h>

void rtctime(struct tm *t);

void rtctime(struct tm *t)
{
    // TODO: get it from RTC
    t->tm_year = 2018 - 1900;
    t->tm_mon = 06;
    t->tm_mday = 05;
    t->tm_hour = 10;
    t->tm_min = 20;
    t->tm_sec = 40;
}

#endif // RTCHELPER_H
