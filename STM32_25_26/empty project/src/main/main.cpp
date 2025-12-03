#pragma once
#include "project_config.h"
#include "time.h"

int state = 0;

int main(void)
{
  ADC_InitTypeDef
  SystemInit();  
  time_service::init();
  EXTI_ClearITPendingBit(EXTI_Line0);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
  GPIO_MODER_MODER1;
  /*GPIO_InitTypeDef pin;//PA1
  pin.GPIO_Pin = GPIO_Pin_1;
  pin.GPIO_Speed = GPIO_Speed_100MHz;
  pin.GPIO_Mode = GPIO_Mode_OUT;
  pin.GPIO_OType = GPIO_OType_PP;
  pin.GPIO_PuPd = GPIO_PuPd_DOWN;
  
  GPIO_PinSource0;
  
  GPIO_Init(GPIOA, &pin);*/
  
  GPIO pin(/.../);
  pin.init();
  
  pin.pwmInit(RCC_APB2ENR_TIM5EN, 800, 4000, 2, TIM5);
  
  while(true)
  {
    GPIO_SetBits(GPIOA, GPIO_Pin_1); // set to 1
    time_service::delay_ms(1000);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1); // set to 0
    time_service::delay_ms(1000);
  }

}
