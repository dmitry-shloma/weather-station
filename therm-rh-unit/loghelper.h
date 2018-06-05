/****************************************************************************
**
** Copyright (C) 2017 Dmitry Shloma
** Contact: vk.com/dmitry.shloma
**
** This file is part of the weather-station project
****************************************************************************/

#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef LOG_FILE
#define LOG_FILE "DAQ.LOG"
#endif

// may be add this CHARLCD, GRAPHLCD, LED_INDICATOR ... and etc ?
enum LOG_DST {TO_SERIAL, TO_SDCARD};
//typedef int LOG_DST; // comment this if used avr-gcc

enum MSG_MODE {USUAL, NOTIFY, CRITICAL}; // msg, MSG, !!! MSG !!!
//typedef int MSG_MODE; // comment this if used avr-gcc

void str_to_upper(const char *in, char *out);

// msg max length is 20
void format_msg_to_msg_log(const char *msg, char *msg_log, MSG_MODE mode);

void to_log(LOG_DST dst, const char *msg, MSG_MODE mode);

void str_to_upper(const char *in, char *out)
{
    while ((*out++ = toupper(*in++)));
}

// msg max length is 20
void format_msg_to_msg_log(const char *msg, char *msg_log, MSG_MODE mode)
{
    // add decorate to msg
    char msg_mode[24] = ""; // TODO: may be is allocated dynamically?
    if (mode == USUAL) {
        strncpy(msg_mode, msg, sizeof(msg_mode));
    } else if (mode == NOTIFY) {
        str_to_upper(msg, msg_mode); // TODO: сделать ограничение кол-ву символов
    } else if (mode == CRITICAL) {
        str_to_upper(msg, msg_mode);
        char buf_temp[24] = ""; // TODO: may be is allocated dynamically?
        snprintf(buf_temp, sizeof(msg_mode), "! %s !", msg_mode);
        strncpy(msg_mode, buf_temp, sizeof(msg_mode));
    }

    // no RTC
    sprintf(msg_log, "[NO RTC] %s", msg_mode);
}

void to_log(LOG_DST dst, const char *msg, MSG_MODE mode)
{
    // TODO: if strlen(msg) > 247 :)

    char msg_log[47] = "";
    format_msg_to_msg_log(msg, msg_log, mode);

    // write to dst
    if (dst == TO_SERIAL) {
        Serial.println(msg_log);
    } else if (dst == TO_SDCARD) {
    }
}

#endif // LOGHELPER_H

