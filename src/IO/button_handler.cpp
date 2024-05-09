#include "button_handler.h"
#include "debugging/debugging.h"

button_handler::press_state button_handler::press(){
  long timer = millis();
  if(!read_pin()){
    return press_state::NO_PRESS;
  }
  byte presses = 1;
  //Checks for double presses, within 500ms
  while(millis() - timer < 500){
    if(read_pin()){
      presses += 1;
    }
  }
  if(presses >= 5){
    return press_state::RESET_PRESS;
  }
  else if(presses == 2){
    return press_state::DOUBLE_PRESS;
  }
  else if(presses == 1){
    return press_state::PRESS;
  }
  else{
    return press_state::NO_PRESS;
  }
}