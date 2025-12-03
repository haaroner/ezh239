#include "Time.h"

namespace time_service
{
  volatile uint32_t SystemTime;
  void init()
  {
    RCC->CFGR &= ~RCC_CFGR_SW;//reset sw bits
    RCC->CFGR |= RCC_CFGR_SW_HSI;//set hsi as system clock
    
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);//wait untill
    
    RCC->CR &= ~RCC_CR_PLLON;//turn PLL off to setup it 
    while((RCC->CR & RCC_CR_PLLRDY)); // wait untill pll is off
      
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY));
    
    RCC->CFGR &= ~RCC_CFGR_HPRE;						//CLEAR
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    RCC->CFGR &= ~RCC_CFGR_PPRE2;
    
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;				//AHB = SYSCLK / 1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;				//APB1 = HCLK / 4
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;				//APB2 = HCLK / 2
    
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ;
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
    
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_2;
    RCC->PLLCFGR |= (RCC_PLLCFGR_PLLQ_2 | RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_0);
    RCC->PLLCFGR |= (RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7);
    
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));
    
    RCC->CFGR &= ~ RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    
    if(SysTick_Config(SystemCoreClock/1000))
    {
      while(true);
    }
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  }
  
  uint32_t get_cur_time()
  {
    return SystemTime;
  }
  
  void delay_ms(uint32_t delta)
  {
    uint32_t cur_time = get_cur_time();
    
    while(get_cur_time() - cur_time < delta);
  }
}

extern "C"
{
  void SysTick_Handler(void)
  {
    time_service::SystemTime++;
  }
}

