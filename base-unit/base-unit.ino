#define PROJECT_NAME WEATHER-STATION
#define TO_STR(str) #str

#define PCF8574_ADDR 0x3F
#include "charlcdhelper_i2c.h"

#include "onewirehelper.h"
#include "hih4000helper.h"

#define LOG_FILE "WEATHER.LOG" // only 8.3
#include "loghelper.h"

#define HIH4000_PIN A0

const uint8_t onewire_pin = 2;
#define ONEWIRE_SENSORS_MAX 3

#define ONE_SEC 1000

// LCD size's
#define LCD_COLS 16
#define LCD_ROWS 2

void setup()
{
    Serial.begin(9600);
  
    to_log(TO_SERIAL, "is started", USUAL);
    to_log(TO_SERIAL, "is started", NOTIFY);
    to_log(TO_SERIAL, "is started", CRITICAL);

    to_log(TO_SDCARD, "is started", USUAL);
    to_log(TO_SDCARD, "is started", NOTIFY);
    to_log(TO_SDCARD, "is started", CRITICAL);
        
//    lcd_init(LCD_COLS, LCD_ROWS);
//    lcd_clear();
}

unsigned long ms0 = 0;

void loop()
{
    return;
    unsigned char addrv[ONEWIRE_SENSORS_MAX][8] = {{0}};
    uint8_t addrc = 0;
    
    onewire_enum_avaliable_devices(&addrc, addrv);
    if (addrc == 0) {
        const char *info[] = {
            "Temperature sensors",
            "not found!"
        };
        lcd_clear();
        lcd_out_text(info[0], (LCD_COLS - strlen(info[0])) / 2, 0);
        lcd_out_text(info[1], (LCD_COLS - strlen(info[1])) / 2, 1);
        delay(ONE_SEC);
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
//        lcd_out_value(str, t, 4, 1, "DSC", 0, 0);
        lcd_out_value("T: ", t, 4, 1, "DSC", 0, 0);

        unsigned long ms1 = millis();
        float rh = 0.0;
        if (ms1 - ms0 > 5 * ONE_SEC) {
            rh = get_humidity(HIH4000_PIN, t);
            lcd_out_value("RH: ", rh, 4, 1, "RH", 0, 1);
            ms0 = ms1;
        }

//        Serial.println(t);
//        Serial.println(rh);

//        char msg[255] = "";
//        sprintf(msg, "temp: %.1f C, rh: %.1f %", t, rh);
//        Serial.println(msg);
//        to_log(SDCARD, msg, INFO);
        
        delay(2 * ONE_SEC);
    }        
}

