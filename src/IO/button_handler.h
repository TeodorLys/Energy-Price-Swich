#pragma once
#include <Arduino.h>
class button_handler {
private:
  byte _pin;
  byte last_state = 0;
public:
  enum press_state {
      NO_PRESS,
      PRESS,
      DOUBLE_PRESS,
      RESET_PRESS   // Did not work...
  };

  /*
  Sets the pin to use the internal pullup,
  reads the current state and saves it.
  */
  button_handler(byte pin) : _pin(pin) {
    pinMode(pin, INPUT_PULLUP);
    last_state = digitalRead(pin);
  }

  press_state press();
private:  
/*
  Compares the last read state to the current state, if they
  dont match it is pressed/released.
  Includes a timer so you cant press it more than once within 100ms
*/
  bool read_pin() {
    static long _timer = millis();
    byte state = digitalRead(_pin);
      if(last_state != state && millis() - _timer > 100){
        last_state = state;
        _timer = millis();
        return !state;
      }
      return false;
  }
};