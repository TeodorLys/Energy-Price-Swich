#pragma once
#include <Arduino.h>

class _debugging{
private:
    static const byte MAX_MSG_SIZE;
    static String _http[];
    //static String buffer;

public:
    // Blink the LED
    static void blink(){
        digitalWrite(GPIO_NUM_20, 1);
        delay(50);
        digitalWrite(GPIO_NUM_20, 0);
    }
    // Blink the LED by the given parameter
    static void blink_times(byte count){
        for (byte a = 0; a < count; a++){
            blink();
            delay(150);
        }
    }
    static String get_http_messages();
    // TODO: change name to something else, not always error message.
    static void push_error_message(String _s);
};
