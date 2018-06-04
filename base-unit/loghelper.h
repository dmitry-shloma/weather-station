#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <stdlib.h>
#include <string.h>

#include "rtchelper.h"

// [2018.06.04, 19:42:00] msg

enum LOG_MODE {USUAL, UNUSUAL};

void str_to_upper(char *str);
void log(char *buf, const char *msg, int mode);

void str_to_upper(char *str)
{
    while (*str++ = toupper(*str));
}

void log(char *buf, const char *msg, int mode)
{
    const char *curr_date = "2018.06.04";
    const char *curr_time = "19:42:00";

    char buf2[255] = "";
    strcpy(buf2, msg);
    if (mode == UNUSUAL) {
        str_to_upper(buf2);
    }
    sprintf(buf, "[%s, %s] %s", curr_date, curr_time, buf2);
}

#endif // LOGHELPER_H
