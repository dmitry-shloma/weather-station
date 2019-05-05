#include <string.h>

#include "charlcdhelper.h"
#include "randhelper.h"

#define ONE_SEC 1000
#define UNCONNECTED_PIN 5

// LCD size's
#define LCD_COLS 16
#define LCD_ROWS 2

void setup()
{
    init_rand(analogRead(UNCONNECTED_PIN));
    lcd_init(LCD_COLS, LCD_ROWS);
    
    lcd_clear();
    const char *banner = "AQUARIUM";
    lcd_out_text(banner, (LCD_COLS - strlen(banner)) / 2, 0);
    delay(5 * ONE_SEC);
}

void loop()
{
    float t[6] = {0};
    for (uint8_t i = 0; i < 6; ++i) {
        t[i] = frrand(0.0, 99.0);
    };

    lcd_clear();
    lcd_out_value("1: ", t[0], 2, 0, "DSC", 0, 0);
    lcd_out_value("2: ", t[1], 2, 0, "DSF", 8, 0);
    lcd_out_value("3: ", t[2], 2, 0, "RH", 16, 0);
    lcd_out_value("4: ", t[3], 2, 0, "RH", 0, 1);
    lcd_out_value("5: ", t[4], 2, 0, "DS", 8, 1);
    lcd_out_value("6: ", t[5], 2, 0, "NONE", 16, 1);
    
    delay(ONE_SEC);
}
