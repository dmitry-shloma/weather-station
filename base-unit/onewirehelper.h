/****************************************************************************
**
** Copyright (C) 2017 Dmitry Shloma
** Contact: http://www.cpp-training.ru
**
** This file is part of the Mikhailo's project and wraps from 
** OneWire's library the following functions : LiquidCrystal(), begin(), 
** clear(), write() in use with createChar() , print(), createChar(). 
** To the other functions, you can access using the object lcd_
****************************************************************************/

#ifndef ONEWIREHELPER_H
#define ONEWIREHELPER_H

#include <OneWire.h>
#include <string.h>

#define ONE_SEC 1000

void onewire_enum_avaliable_devices(uint8_t *addrc, unsigned char addrv[][8]);
void onewire_addr_to_str(unsigned char *addr, char *str);

enum MODE {C, F};
float onewire_get_temperature(unsigned char *addr, MODE mode);

extern const uint8_t onewire_pin;
OneWire onewire(onewire_pin);

void onewire_enum_avaliable_devices(uint8_t *addrc, unsigned char addrv[][8])
{
    unsigned char addr[8] = {0};
    while (onewire.search(addr)) {
        // TODO: add crc check
        memcpy(addrv++, addr, 8);
        (*addrc)++;
    }
    
    // is it necessary?
    onewire.reset_search();
    delay(ONE_SEC / 4);
}

void onewire_addr_to_str(unsigned char *addr, char *str)
{
    for(uint8_t i = 0; i < 8; ++i) {
        sprintf(str, "%02X", addr[i]);
        str+=2;
    }    
}

float onewire_get_temperature(unsigned char *addr, MODE mode)
{
    // 0 - DS18B20, DS1822, 1 - DS1820, DS18S20
    // TODO: why a device is not a DS18x20 family device?
    unsigned char type_s = (addr[0] == 0x10) ? 1 : 0;

    onewire.reset();
    onewire.select(addr);
    onewire.write(0x44);

    delay(ONE_SEC); // maybe 750ms is enough?

    onewire.reset();
    onewire.select(addr);
    onewire.write(0xBE);

    unsigned char data[12] = {0};
    for (uint8_t i = 0; i < 9; ++i) {
        data[i] = onewire.read();
    }

    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
        raw = raw << 3;
        if (data[7] == 0x10) {
            raw = (raw & 0xFFF0) + 12 - data[6];
        }
    } else {
        unsigned char cfg = (data[4] & 0x60);
        if (cfg == 0x00) {
            raw = raw & ~7;
        } else if (cfg == 0x20) {
            raw = raw & ~3;
        } else if (cfg == 0x40) {
            raw = raw & ~1;
        }
    }

    // if the mode is not Celsius, then it is a Fahrenheit
    return (mode == C) ? raw / 16.0 : raw / 16.0 * 1.8 + 32.0;
}

#endif // ONEWIREHELPER_H
