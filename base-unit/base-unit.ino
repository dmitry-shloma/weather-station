#include <iarduino_RF433_Receiver.h>
#include <SPI.h>
#include <SD.h>

#include "onewirehelper.h"
#include "hih4000helper.h"
#include "charlcdhelper_i2c.h"
#include "loghelper.h"

#define HIH4000_PIN A0

const uint8_t onewire_pin = 2;
#define ONEWIRE_SENSORS_MAX 3

const uint8_t charlcd_i2c_addr = 0x3F;

#define RECEIVER_PIN 3

#define ONE_SEC 1000

#define CHIP_SELECT 10

// LCD size's
#define LCD_COLS 16
#define LCD_ROWS 2

iarduino_RF433_Receiver radioRX(RECEIVER_PIN);

void setup()
{
    Serial.begin(9600);
    lcd_init(LCD_COLS, LCD_ROWS);
    lcd_clear();

    if (!SD.begin(CHIP_SELECT)) {
        // TODO: error
        Serial.println("Card failed, or not present");
    }

    radioRX.begin(1000);
    radioRX.openReadingPipe(5);
    radioRX.startListening();
}

unsigned long ms0 = 0;

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

    char buffer[12] = "";
    uint8_t k;
    if(radioRX.available(&k)){                            // Если в буфере имеются данные принятые приёмником, то получаем номер трубы в переменную k и ...
       radioRX.read(&buffer, 12);                       // Читаем данные в массив j и указываем сколько байт читать
       Serial.print(buffer);                                   // Выводим полученные данные на монитор
       Serial.println((String)" (Pipe="+k+")");           // Выводим номер трубы, по которой эти данные получены. Так можно определить, от кокого передатчика они получены
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

        File daqlog = SD.open("daq.log", FILE_WRITE);
        if (!daqlog) {
            // TODO: error
            Serial.println("error open file");
        }
        char buf2[255] = "";
        sprintf(buf2, "temp: %.1f C, rh: %.1f %", t, rh);
        Serial.println(buf2);
        daqlog.println(buf2);
        daqlog.close();
        
        delay(2 * ONE_SEC);
    }        
}
