#include "onewirehelper.h"
#include "hih4000helper.h"
#include "charlcdhelper.h"

#define HIH4000_PIN A0

const uint8_t onewire_pin = 2;
#define ONEWIRE_SENSORS_MAX 3

#define ONE_SEC 1000

// LCD size's
#define LCD_COLS 16
#define LCD_ROWS 2

void setup()
{
    lcd_init(LCD_COLS, LCD_ROWS);
    lcd_clear();
}

unsigned long ms2 = 0;

void loop()
{
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
                
        lcd_clear();
        
        float t = onewire_get_temperature(addrv[i], C);
//        lcd_out_value(str, t, 4, 1, "DSC", 0, 0);
        lcd_out_value("", t, 4, 1, "DSC", 0, 0);

        unsigned long ms = millis();
        if (ms - ms2 > 5 * ONE_SEC) {
            float rh = get_humidity(HIH4000_PIN, t);
            lcd_out_value("RH: ", rh, 4, 1, "RH", 0, 1);
        }
        
        delay(2 * ONE_SEC);
    }
        
}
