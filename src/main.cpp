#include "ext/RF24/nRF24L01.h"
#include "ext/RF24/RF24.h"

// // // set before use
// board pins mapping
uint8_t pin_x1 = A0,
    pin_y1 = A1,
    pin_x2 = A2,
    pin_y2 = A3,
    pin_CE = 7,
    pin_CSN = 8,
    pin_power_on = 2,
    pin_radio_connected = 3;

const byte pipeOut[6] = "00001";

int16_t mid_x1, mid_y1, mid_x2, mid_y2;

uint8_t data[10] = {143};

RF24 radio(pin_CE, pin_CSN);

void calibrate_joysticks();

uint8_t map_joystick_value(int16_t value, int16_t mid_value);
bool res;

void data_update_joysticks();

void setup() {
    Serial.begin(9600);
    calibrate_joysticks();
    data_update_joysticks();
    radio.begin();
    radio.openWritingPipe(pipeOut);
    radio.stopListening();
    pinMode(pin_power_on,OUTPUT);
    digitalWrite(pin_power_on, LOW);
    pinMode(pin_radio_connected, OUTPUT);
    digitalWrite(pin_radio_connected, LOW);
}

void loop() {
    digitalWrite(pin_power_on, HIGH);
    data_update_joysticks();
    bool radioRes = radio.write(&data, sizeof(data));
    // joysticks output
    Serial.print("js: ");
    Serial.print( data[0] );
    Serial.print(" ");
    Serial.print( data[1] );
    Serial.print(" ");
    Serial.print( data[2] );
    Serial.print(" ");
    Serial.print( data[3] );
    // is radio connection established
    if(radioRes) {
        Serial.print(" +");
        digitalWrite(pin_radio_connected, HIGH);
    }
    else {
        Serial.print(" -");
        digitalWrite(pin_radio_connected, LOW);
    }
    Serial.println( " " );
    radio.printDetails();
    delay(300);
}

void calibrate_joysticks() {
    int32_t sum_x1 = 0,
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

uint8_t map_joystick_value(int16_t value, int16_t mid_value) {
    // joystick row values range: 0 to 1023
    // map to range 0 to 255
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

void data_update_joysticks()
{
    data[0] = map_joystick_value(analogRead(pin_x1), mid_x1);
    data[1] = map_joystick_value(analogRead(pin_y1), mid_y1);
    data[2] = map_joystick_value(analogRead(pin_x2), mid_x2);
    data[3] = map_joystick_value(analogRead(pin_y2), mid_y2);
}