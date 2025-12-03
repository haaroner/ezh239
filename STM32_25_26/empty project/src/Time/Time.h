#pragma once

#include "project_config.h"

namespace time_service
{
  extern volatile uint32_t SystemTime;//in clock tics
  void init();
  void delay_ms(uint32_t delta);
  uint32_t get_cur_time();//in ms
}
