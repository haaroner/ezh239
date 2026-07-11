#include "usart6.h"

extern "C" void USART6_IRQHandler(void)
{
  if(USART_GetITStatus(USART6, USART_IT_TXE) == SET) //Transmit part. Условие по флагу IT_TXE - Interruption TX Empty
  {
    if(usart6::_bytesToSend > 0) //Проверка, сколько осталось отправить байт
    {
      --usart6::_bytesToSend; //Уменьшили их количество на 1
      USART_SendData(USART6, usart6::tx[usart6::_sendCnt++]); // Отправили и сдвинули счетчик отправлителя в буфере на 1
      if(usart6::_sendCnt == 30) //Сдвигаем счетчик в буфере в 0, если уперлись в лимит
        usart6::_sendCnt = 0;
    }
    else
    {
       USART_ITConfig(USART6, USART_IT_TXE, DISABLE); // Если нечего отправлять - выключить прерывание на отправку.
    }
  }
  
  if(USART_GetITStatus(USART6, USART_IT_RXNE) == SET) //Receive part - Приемник не пуст, флаг IT_RXNE - Interruptiob RX Not Empty
  {
    if((USART6->SR & (USART_FLAG_NE || USART_FLAG_FE || USART_FLAG_PE || USART_FLAG_ORE)) != 0) // Проверка корректности приема байта
    {
      USART_ReceiveData(USART6);//Если ошибка - просто пропусти байт
    }
    else
    {
      usart6::rx[usart6::_rxCnt] = USART_ReceiveData(USART6);//Иначе прочитай байт и смести указатель в массиве
      usart6::_rxCnt++;
      if(usart6::_rxCnt == 30)
        usart6::_rxCnt = 0;
    }
  }
}

namespace usart6
{
  volatile uint8_t tx[30];
  volatile uint8_t rx[30];
  volatile uint16_t _rxCnt;
  volatile uint16_t _txCnt;
  volatile bool flag;
  volatile uint16_t _readCnt;
  volatile uint16_t _sendCnt;
  volatile uint8_t _bytesToSend;
  volatile uint32_t _tets;
  volatile bool _receiver_buffer_overflow_warning;
  
  
    void usart6Init(uint32_t speed, uint8_t word_length, float stop_bits)
    {
    flag = 1;
    _txCnt = 0;
    _rxCnt = 0;
    _readCnt = 0;
    _sendCnt = 0;
    _bytesToSend = 0;
    _receiver_buffer_overflow_warning = false;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
    USART_InitTypeDef u;
    u.USART_BaudRate = speed;
    
    if(word_length == 9) u.USART_WordLength = USART_WordLength_9b;
    else u.USART_WordLength = USART_WordLength_8b;

    if(stop_bits == 2) u.USART_StopBits = USART_StopBits_2;
    else if(stop_bits == 0.5) u.USART_StopBits = USART_StopBits_0_5;
    else if(stop_bits == 1.5) u.USART_StopBits = USART_StopBits_1_5;
    else u.USART_StopBits = USART_StopBits_1;
    u.USART_WordLength = USART_WordLength_8b;
    u.USART_StopBits = USART_StopBits_1;
    u.USART_Parity = USART_Parity_No;
    u.USART_Mode =  USART_Mode_Rx | USART_Mode_Tx;
    u.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART6, &u);

    //Настройка прерываний
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ???; //если у 2 прерываний одинаковый приоритет - порядок выполнения не определен (а.к.а. случайный) 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 

    NVIC_Init(&NVIC_InitStructure);
    USART_Cmd(USART6, ENABLE);
    USART_ITConfig(USART6, USART_IT_TXE, ENABLE);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
  
  }  
     
  uint16_t read()
  {  
    uint16_t dt;
    ENTER_CRITICAL_SECTION();
    dt = rx[_readCnt];
    _readCnt++;
    if(_readCnt == 30)
    {
     _readCnt = 0;
    }
    EXIT_CRITICAL_SECTION();
    return dt;
  }  
  
  uint16_t look()
  {  
    uint16_t dt;
    ENTER_CRITICAL_SECTION();
    dt = rx[_readCnt];
    EXIT_CRITICAL_SECTION();
    return dt;
  }  
     
  uint16_t available()
  {  
    uint16_t size;
    ENTER_CRITICAL_SECTION();


    size = _rxCnt - _readCnt;
    EXIT_CRITICAL_SECTION();
    return size;
  }  
     
  void write(uint8_t _byte)
  {  
    ENTER_CRITICAL_SECTION();
    if(_bytesToSend < 29)
    {
      USART_ITConfig(USART6, USART_IT_TXE, ENABLE);
     tx[_txCnt] = _byte;
     _txCnt++;
      _bytesToSend += 1;
     if(_txCnt == 30)
     {
     _txCnt = 0;
     }
   }
    EXIT_CRITICAL_SECTION();
  }
}
