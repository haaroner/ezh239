// Функция отправки одного байта (поллинг)
namespace UART2
{
   void write(uint8_t byte)
   {
      // Ждем освобождения регистра передачи
      while (USART_GetFlagStatus(xxxx, USART_FLAG_TXE) == RESET);
      // Отправляем байт
      USART_SendData(xxxx, Byte);
      // Ждем завершения передачи
      while (USART_GetFlagStatus(xxx, USART_FLAG_TC) == RESET);
   }

   // Функция приема одного байта (поллинг)
   uint8_t read()
   {
      // Ждем поступления данных
      while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
      // Возвращаем принятый байт
      return USART_ReceiveData(USART2);
   }

   void UART_Init(uint32_t speed, uint8_t word_length, float stop_bits)
   {
      USART_InitTypeDef USART_InitStructure;
      RCC_PeriphClockCmd(RCC_PERIPH_xxx, ENABLE);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSourcexxxx, GPIO_AF_xxxx);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSourcexxxx, GPIO_AF_xxxx);
      
      // Настройка USART2
      USART_InitStructure.USART_BaudRate = ;
      USART_InitStructure.USART_WordLength = ;
      USART_InitStructure.USART_StopBits = ;
      USART_InitStructure.USART_Parity = ;
      USART_InitStructure.USART_HardwareFlowControl = ;
      USART_InitStructure.USART_Mode = ;
      USART_Init(xxx, &xxx);
      
      // Включаем UART
      USART_Cmd(xxxx, xxx);
   }
}