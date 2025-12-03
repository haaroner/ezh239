#pragma once
#include "project_config.h"


class GPIO
{
  public:
    GPIO(GPIO_TypeDef* GPIOx,
         uint32_t pinNumber,
         GPIOMode_TypeDef modeGPIO,
         GPIOSpeed_TypeDef speedGPIO,
         GPIOOType_TypeDef otypeGPIO, 
         GPIOPuPd_TypeDef  pupdGPIO,
         uint8_t pinSourceGPIO);
  void pwmInit(uint16_t prescaler, uint32_t period, uint8_t channel, TIM_TypeDef* TIMx);
  
  void write(bool data);
  void pwm(uint32_t pulse);
  
  private:
    GPIO_TypeDef* _GPIOx;
    TIM_TypeDef* _TIMx;
    uint32_t _pinNumber;
    GPIOMode_TypeDef _modeGPIO;
    GPIOSpeed_TypeDef _speedGPIO;
    GPIOOType_TypeDef _otypeGPIO;
    GPIOPuPd_TypeDef  _pupdGPIO;
    uint8_t _pinSourceGPIO;
    uint8_t _channel;
        
};