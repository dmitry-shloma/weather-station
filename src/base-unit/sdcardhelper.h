/****************************************************************************
**
** Copyright (C) 2017 Dmitry Shloma
** Contact: vk.com/dmitry.shloma
**
** This file is part of the weather-station project
****************************************************************************/

#ifndef SDCARDHELPER_H
#define SDCARDHELPER_H

#include <SPI.h>
#include <SD.h>

#define CS_PIN 10 // chip select pin

#define SDCARD_NO_ERROR 0
#define SDCARD_NOT_PRESENT 1
#define SDCARD_OPEN_FILE_ERROR 2

int append_str_to_file(const char *fname, const char *str);

int append_str_to_file(const char *fname, const char *str)
{
    if (!SD.begin(CS_PIN)) {
        return SDCARD_NOT_PRESENT;
    }
    
    File file = SD.open(fname, FILE_WRITE);
    if (!file) {
        return SDCARD_OPEN_FILE_ERROR;
    }
    file.println(str);
    file.close();

    return SDCARD_NO_ERROR;
}

#endif // SDCARDHELPER_H
