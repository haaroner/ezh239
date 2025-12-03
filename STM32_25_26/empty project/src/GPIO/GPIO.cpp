#include "GPIO.h"

GPIO::GPIO(GPIO_TypeDef* GPIOx,
         uint32_t pinNumber,
         GPIOMode_TypeDef modeGPIO,
         GPIOSpeed_TypeDef speedGPIO,
         GPIOOType_TypeDef otypeGPIO, 
         GPIOPuPd_TypeDef  pupdGPIO,
         uint8_t   pinsource,
         uint8_t   AF_GPIO)
{
//...   _...=...
  
 //init struct
 
 //GPIO_INIT
  if(_modeGPIO == GPIO_Mode_AF)
    GPIO_PinAFConfig(_GPIOx, pinsource, AF_GPIO);
}

void GPIO::pwmInit(uint8_t RCC_TIMx,uint16_t prescaler, uint32_t period, uint8_t channel, TIM_TypeDef* TIMx)
{
  _channel = channel;
  if(TIMx == TIM1 || TIMx == TIM8 || TIMx == TIM9 || TIMx == TIM10 || TIMx == TIM11)
    RCC_APB2PeriphClockCmd(RCC_TIMx, ENABLE);
  else
    RCC_APB1PeriphClockCmd(RCC_TIMx, ENABLE);
  
  TIM_TimeBaseInitTypeDef timer;
  timer.TIM_ClockDivision = TIM_CKD_DIV1;
  timer.TIM_CounterMode = TIM_CounterMode_Up;
  timer.TIM_Prescaler = prescaler;
  timer.TIM_Period = period;
  TIM_TimeBaseInit(TIMx, &timer);
  TIM_Cmd(TIMx, ENABLE);
  
  TIM_OCInitTypeDef ch; //ch ~ channel
  
 //...//
  
  //add OCNpolarity... = ...LOW
  
  
  if(channel == 1)
    TIM_OC1Init(TIMx, &ch);
  if(channel == 2)
    TIM_OC2Init(TIMx, &ch);
  //if(channel == 3) ...
  //channel == 4 ...
}
GPIO_AF_TIMx //x = 1\2\3\4\5..14
void GPIO::pwm(uint32_t pulse)
{
 if(_channel == 1)
    TIM_SetCompare1(_TIMx, pulse);
 if(_channel == 2)
    TIM_SetCompare2(_TIMx, pulse);
 //_channel == 3
 //_channel == 4
}
