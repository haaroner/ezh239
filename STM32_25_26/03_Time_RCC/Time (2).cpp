
#include "Time.h"

// Определение статической переменной systemTime
volatile uint32_t TimeHandler::systemTime = 0;

void TimeHandler::init()
{
  RCC->CR |= RCC_CR_HSION; //enable HSI(nternal)
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_HSI; //choose HSI of all variants of clocks
  
  while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI); //wait for enabled status
  
  RCC->CR &= ~RCC_CR_PLLON; //disable PLL
	while((RCC->CR & RCC_CR_PLLRDY)) //wait until PII turning on
	RCC->CR |= RCC_CR_HSEON;
	while(RCC->CR & RCC_CR_HSERDY);
	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	RCC->CFGR &= ~RCC_CFGR_PPRE2;
	RCC->CFGR &= ~RCC_CFGR_HPRE;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; 
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; 
  //setting for PLL multiply and division
  RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
  RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
  RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;
  RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_4;
  RCC->PLLCFGR |= (RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7);
  RCC->PLLCFGR |= (RCC_PLLCFGR_PLLQ_2 | RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_0);
  RCC->CR |= RCC_CR_PLLON;
  while(!(RCC->CR & RCC_CR_PLLRDY));
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
  if(SysTick_Config(SystemCoreClock / 1000)) {
  	while(1);
	}
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

}
  uint32_t TimeHandler::getSysTimeMS()
{
	return TimeHandler::systemTime;
}
void TimeHandler::delay_ms(uint32_t ms)
{
	uint32_t start = TimeHandler::getSysTimeMS();
	while(getSysTimeMS() - start < ms);
}
extern "C" {
	void SysTick_Handler(void)
	{
		TimeHandler::systemTime++;
	}
}