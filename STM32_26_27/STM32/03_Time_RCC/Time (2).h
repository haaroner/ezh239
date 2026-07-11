#ifndef CLOCK_H
#define CLOCK_H

#include "project_config.h"

namespace TimeHandler
{
  extern volatile uint32_t systemTime;
  
  //system timer initialization
  void init();
  
  //basic delay function
  void delay_ms(uint32_t ms);
  
  //ststem time in ms
  uint32_t getSysTimeMS();
}

#endif
