EXTI_InitTypeDef exti;
  
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);
  EXTI_ClearITPendingBit(EXTI_Linex);
  exti.EXTI_Line = EXTI_Linex;
  exti.EXTI_LineCmd = ;
  exti.EXTI_Mode = ;
  exti.EXTI_Trigger = ;
  
  EXTI_Init(&exti);
  
  NVIC_InitTypeDef nvic;
  nvic.NVIC_IRQChannel = EXTIx_IRQn;
  nvic.NVIC_IRQChannelCmd = ;
  nvic.NVIC_IRQChannelPreemptionPriority = ;
  nvic.NVIC_IRQChannelSubPriority = ;
  
  NVIC_Init(&nvic);
