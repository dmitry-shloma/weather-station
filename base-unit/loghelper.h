#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "rtchelper.h"
#include "sdcardhelper.h"

#ifndef LOG_FILE
#define LOG_FILE DAQ.LOG
#endif

// may be add this CHARLCD, GRAPHLCD, LED_INDICATOR ... and etc ?
enum LOG_DST {TO_SERIAL, TO_SDCARD};
//typedef int LOG_DST; // comment this if used avr-gcc

enum MSG_MODE {USUAL, NOTIFY, CRITICAL}; // msg, MSG, !!! MSG !!!
//typedef int MSG_MODE; // comment this if used avr-gcc

void str_to_upper(const char *in, char *out);
void format_msg_to_msg_log(const char *msg, char *msg_log, MSG_MODE mode);

void to_log(LOG_DST dst, const char *msg, MSG_MODE mode);

void str_to_upper(const char *in, char *out)
{
    while ((*out++ = toupper(*in++)));
}

void format_msg_to_msg_log(const char *msg, char *msg_log, MSG_MODE mode)
{
    // add text decorate
    char msg_mode[70] = ""; // TODO: may be is allocated dynamically?
    if (mode == USUAL) {
        strcpy(msg_mode, msg);
    } else if (mode == NOTIFY) {
        str_to_upper(msg, msg_mode);
    } else if (mode == CRITICAL) {
        str_to_upper(msg, msg_mode);
        char buf_temp[70] = ""; // TODO: may be is allocated dynamically?
        sprintf(buf_temp, "!!! %s !!!", msg_mode);
        strcpy(msg_mode, buf_temp);
    }

    // add datetime
    struct tm t;
    rtctime(&t);
    char buf_datetime[70] = "";
    strftime(buf_datetime, sizeof(buf_datetime), "%Y-%m-%d, %H:%M:%S", &t);
    sprintf(msg_log, "[%s] %s", buf_datetime, msg_mode);
}

void to_log(LOG_DST dst, const char *msg, MSG_MODE mode)
{
    // TODO: if strlen(msg) > 247 :)

    char msg_log[70] = "";
    format_msg_to_msg_log(msg, msg_log, mode);

    // write to dst
    if (dst == TO_SERIAL) {
        Serial.println(msg_log);
    } else if (dst == TO_SDCARD) {
        int error = append_str_to_file(LOG_FILE, msg_log);
        char msg[70] = "";
        switch (error) {
        case SDCARD_NO_ERROR:
            sprintf(msg, "in file %s, write %s", LOG_FILE, msg_log);
            to_log(TO_SERIAL, msg, USUAL);
            break;
        case SDCARD_NOT_PRESENT:
            to_log(TO_SERIAL, "sd-card failed, or not present", NOTIFY);
            break;
        case SDCARD_OPEN_FILE_ERROR:
            sprintf(msg, "error open file %s", LOG_FILE);
            to_log(TO_SERIAL, msg, NOTIFY);
            break;
        default:
            sprintf(msg, "sdcard, unexpected error %d", error);
            to_log(TO_SERIAL, msg, NOTIFY);
        }
    }
}

#endif // LOGHELPER_H

