#include <Arduino.h>

uint8_t pin_x1 = A0,
    pin_y1 = A1,
    pin_x2 = A2,
    pin_y2 = A3;

int16_t mid_x1 = 506,
        mid_y1 = 506,
        mid_x2 = 506,
        mid_y2 = 506;

uint8_t data[10] = {0};

void calibrate_josticks();

uint8_t map_jostick_value(int16_t value, int16_t mid_value);

void data_update_josticks();

void setup() {
    Serial.begin(9600);
}

void loop() {
    data_update_josticks();
    Serial.print( data[0] );
    Serial.print(" ");
    Serial.print( data[1] );
    Serial.print(" ");
    Serial.print( data[2] );
    Serial.print(" ");
    Serial.println( data[3] );
}

void calibrate_josticks() {
    int sum_x1 = 0,
        sum_y1 = 0,
        sum_x2 = 0,
        sum_y2 = 0;
    for(int i = 0; i < 1000; i++)
    {
        sum_x1 += analogRead(pin_x1);
        sum_y1 += analogRead(pin_y1);
        sum_x2 += analogRead(pin_x2);
        sum_y2 += analogRead(pin_y2);
    }
    mid_x1 = (int)(sum_x1 / 1000);
    mid_y1 = (int)(sum_y1 / 1000);
    mid_x2 = (int)(sum_x2 / 1000);
    mid_y2 = (int)(sum_y2 / 1000);

}

uint8_t map_jostick_value(int16_t value, int16_t mid_value) {
    uint8_t result;
    if( value < ( mid_value - 4 ) ) {
        result = value / ( mid_value / 127 );
    }
    else if( value > ( mid_value + 4 ) ) {
        result = ( value / ( ( 1023 - mid_value ) / 127 ) );
    }
    else {
        result = 128;
    }
    if(result > 255) result = 255;
    else if(result < 0) result = 0;
    return result;
}

void data_update_josticks()
{
    data[0] = map_jostick_value(analogRead(pin_x1), mid_x1);
    data[1] = map_jostick_value(analogRead(pin_y1), mid_y1);
    data[2] = map_jostick_value(analogRead(pin_x2), mid_x2);
    data[3] = map_jostick_value(analogRead(pin_y2), mid_y2);
}