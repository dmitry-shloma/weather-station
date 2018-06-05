#define PCF8574_ADDR 0x3F // find it using i2c scanner
#include "charlcdhelper_i2c.h"

#include "onewirehelper.h"
#include "hih4000helper.h"

#define LOG_FILE "WEATHER.LOG" // only 8.3
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

// LCD size's
#define LCD_COLS 16
#define LCD_ROWS 2

#define LED_RX_PIN 7

RH_ASK driver;

void setup()
{
    Serial.begin(9600);

    to_log(TO_SERIAL, "ws is started", USUAL);

    if (!driver.init()) {
        to_log(TO_SERIAL, "rf433 init fail", NOTIFY);
    }

    pinMode(LED_RX_PIN, OUTPUT);
     
    int error = lcd_init(LCD_COLS, LCD_ROWS);
    if (error != CHAR_LCD_NO_ERROR) {
        char msg[20] = "";
        sprintf(msg, "lcd init, error %d", error);
        to_log(TO_SERIAL, msg, CRITICAL);
//        to_log(TO_SDCARD, msg, CRITICAL);
        STOP;
    }
    to_log(TO_SERIAL, "lcd init success", USUAL);
//    to_log(TO_SDCARD, "lcd init success", USUAL);
    lcd_clear();

    const char *title[] = {
        "WEATHER",
        "STATION"
    };
    lcd_out_text(title[0], (LCD_COLS - strlen(title[0])) / 2, 0);
    lcd_out_text(title[1], (LCD_COLS - strlen(title[0])) / 2, 1);
    
    delay(ONE_SEC * 5);
    lcd_clear();
}

unsigned long ms0 = 0;
bool is_need_update_ = false;

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
//        to_log(TO_SDCARD, "t sensors not found", CRITICAL);
        
        lcd_clear();
        lcd_out_text(info[0], (LCD_COLS - strlen(info[0])) / 2, 0);
        lcd_out_text(info[1], (LCD_COLS - strlen(info[1])) / 2, 1);
        is_need_update_ = true;
        return;
    }

    if (is_need_update_) {
        lcd_clear();
        is_need_update_ = false;
    }
    for (int i = 0; i < addrc; ++i) {
        char str[19] = "";  // 0...15 byte for addr, 16 byte - ':', 17 byte - ' ', 18 byte - '\0'
        onewire_addr_to_str(addrv[i], str);
        str[16] = ':';
        str[17] = ' ';
        // str[18] is already '\0';

        // memcpy(&str[16], ": ", 2); // don't use this, because it requires 7942, instead of 7934 bytes
                
        float t1 = onewire_get_temperature(addrv[i], C);
//        lcd_out_value(str, t1, 4, 1, "DSC", 0, 0);
        lcd_out_value("T, C: 1:", t1, 2, 0, "NONE", 0, 0);
        const unsigned char degree[8] = {0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00}; // degree symbol
        lcd_out_custom_char(degree, 0, 2, 0);

        //FIXME: Ширина поля сделана что бы цифры были ровно,
        // однако если цифра отрицательная, она начинает скакать потому что с минусом ширина больше
        // если сделать ее меньше то остается часть от прежней цифры

        int t_rh[2] = {0};
        int t_rh_i = 0;
        uint8_t buf[8] = "";
        uint8_t buflen = sizeof(buf);
        digitalWrite(LED_RX_PIN, HIGH);
        if (driver.recv(buf, &buflen)) { // неблокирующая функция
            to_log(SERIAL, (char*)buf, USUAL);
            char *ch = strtok((char*)buf, "/");
            while (ch != NULL) {
                t_rh[t_rh_i] = atoi(ch);
                ch = strtok(NULL, "/");
                ++t_rh_i;
            }
        }
        digitalWrite(LED_RX_PIN, LOW);
        if (t_rh[0] != 0) { // BUG: 0 градусов пролетает :)
            lcd_out_value("2:", t_rh[0], 2, 0, "NONE", 11, 0); // t2
        }

        unsigned long ms1 = millis();
        if (ms1 - ms0 > 5 * ONE_SEC) {
            float rh1 = get_humidity(HIH4000_PIN, t1);
            lcd_out_value("RH,%: 1:", rh1, 2, 0, "NONE", 0, 1);
//            to_log(TO_SERIAL, 12, USUAL);
        if (t_rh[1] != 0) { // BUG: с влажностью не так страшно :)
            lcd_out_value("2:", t_rh[1], 2, 0, "NONE", 11, 1); // rh2
        }
//            to_log(TO_SERIAL, rh2, USUAL);
            ms0 = ms1;
        }
    }        
}

