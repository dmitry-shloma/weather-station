#include <iarduino_RF433_Transmitter.h>

#include "onewirehelper.h"
#include "hih4000helper.h"

#define TRANSMITTER_PIN 4
#define LED_PIN 3

#define HIH4000_PIN A0

const uint8_t onewire_pin = 2;
#define ONEWIRE_SENSORS_MAX 3

#define ONE_SEC 1000

#define MODULE_ID 1

iarduino_RF433_Transmitter radioTX(TRANSMITTER_PIN);

void setup()
{
    Serial.begin(9600);
    radioTX.begin(1000);
    radioTX.openWritingPipe(5);
    pinMode(LED_PIN, OUTPUT);
}

unsigned long ms0 = 0;

void loop()
{
    unsigned char addrv[ONEWIRE_SENSORS_MAX][8] = {{0}};
    uint8_t addrc = 0;
    
    onewire_enum_avaliable_devices(&addrc, addrv);
    if (addrc == 0) { // temperature sensors not found
        // TODO: send error
        Serial.println("temperature sensors not found");
        delay(ONE_SEC);
        return;
    }
    
//    for (int i = 0; i < addrc; ++i) {
//        char str[19] = "";  // 0...15 byte for addr, 16 byte - ':', 17 byte - ' ', 18 byte - '\0'
//        onewire_addr_to_str(addrv[i], str);
//        str[16] = ':';
//        str[17] = ' ';
        // str[18] is already '\0';

        // memcpy(&str[16], ": ", 2); // don't use this, because it requires 7942, instead of 7934 bytes
                
        float t = onewire_get_temperature(addrv[0], C);
        unsigned long ms1 = millis();
        if (ms1 - ms0 > 5 * ONE_SEC) { // запрос отн. влажности каждые пять секунд
            float rh = get_humidity(HIH4000_PIN, t);
            
            char packet[12] = "";
            sprintf(packet, "%.1f/%.1f", t, rh);
            digitalWrite(LED_PIN, HIGH);
            char buf[] = "ADS";
            radioTX.write(&buf, 3);
            delay(200);
            digitalWrite(LED_PIN, LOW);
            ms0 = ms1;
        }
        delay(2 * ONE_SEC);
//    }
}
