#include "onewirehelper.h"
#include "hih4000helper.h"
#include "loghelper.h"

#include <RH_ASK.h>
#include <SPI.h>

#include <string.h>

#define HIH4000_PIN A0

const uint8_t onewire_pin = 3;
#define ONEWIRE_SENSORS_MAX 1

#define ONE_SEC 1000

#define STOP \
    while(1) {};

RH_ASK driver;

void setup()
{
    Serial.begin(9600);

    to_log(TO_SERIAL, "ws module 1 is started", USUAL);

    if (!driver.init()) {
        to_log(TO_SERIAL, "rf433 init fail", NOTIFY);
    }
}

unsigned long ms0 = 0;

void loop()
{    
    unsigned char addrv[ONEWIRE_SENSORS_MAX][8] = {{0}};
    uint8_t addrc = 0;
    
    onewire_enum_avaliable_devices(&addrc, addrv);
    if (addrc == 0) {
        const char *info[] = {
            "Temper sensors",
            "not found!"
        };
        to_log(TO_SERIAL, "t sensors not found", CRITICAL);
        return;
    }

    for (int i = 0; i < addrc; ++i) {
        char str[19] = "";  // 0...15 byte for addr, 16 byte - ':', 17 byte - ' ', 18 byte - '\0'
        onewire_addr_to_str(addrv[i], str);
        str[16] = ':';
        str[17] = ' ';
        // str[18] is already '\0';

        // memcpy(&str[16], ": ", 2); // don't use this, because it requires 7942, instead of 7934 bytes
                
        float t = onewire_get_temperature(addrv[i], C);
        float rh = 0.0;
        unsigned long ms1 = millis();
        if (ms1 - ms0 > 5 * ONE_SEC) {
            rh = get_humidity(HIH4000_PIN, t);
//            to_log(TO_SERIAL, 12, USUAL);
//            to_log(TO_SERIAL, rh2, USUAL);
            ms0 = ms1;
        }
        char packet[8] = "";
        sprintf(packet, "%d/%d", (int)ceil(t), (int)ceil(rh));
        to_log(TO_SERIAL, packet, USUAL);
        driver.send((uint8_t *)packet, strlen(packet));
        driver.waitPacketSent();
        delay(ONE_SEC * 2);        
    }        
}

