/****************************************************************************
**
** Copyright (C) 2017 Dmitry Shloma
** Contact: vk.com/dmitry.shloma
**
** This file is part of the weather-station project
****************************************************************************/

#ifndef HIH4000HELPER_H
#define HIH4000HELPER_H

float get_humidity(uint8_t hih4000_pin, float t);

float get_humidity(uint8_t hih4000_pin, float t)
{
//    uint16_t sensorValue = analogRead(hih4000_pin);
    // (1 = 0.0049V) 0-1023 range
//    float voltage = sensorValue * 0.0049;
  // Relative humidity(RH) (These are the values for my sensor, yours may differ a bit):
  // 0% = about 163
  // 100% = about 795
  // With roughly linear response.
  // 795 - 163 = 632 (points in the sensor's range)
  // 6.32 points = 1% RH

    float rh = ((analogRead(hih4000_pin) / 1023.0) * 5.0 / 5.0 - 0.16) * 161.29 / 1.0546 - (0.00216 * t);
//    float rh = (analogRead(hih4000_pin) - 163) / 6.32;
//    if (rh < -5) {
//        return ERROR;
//    }
    return rh;
}

#endif // HIH4000HELPER_H
