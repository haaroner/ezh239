#include "DMA.h"

DMA_Controller::DMA_Controller(DMA_Stream_TypeDef* stream, uint32_t channel) 
    : _stream(stream), _channel(channel) {
}

void DMA_Controller::EnableClock() {
    if ((uint32_t)_stream < (uint32_t)DMA2) {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    } else {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    }
}

void DMA_Controller::InitP2M(uint32_t periphAddr, uint32_t memAddr, uint32_t bufferSize) {
    EnableClock();
    DMA_DeInit(_stream);

    _dmaInit.DMA_Channel = _channel;
    _dmaInit.DMA_PeripheralBaseAddr = periphAddr;
    _dmaInit.DMA_Memory0BaseAddr = memAddr;
    _dmaInit.DMA_DIR = DMA_DIR_PeripheralToMemory;
    _dmaInit.DMA_BufferSize = bufferSize;
    _dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    _dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable;
    _dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    _dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    _dmaInit.DMA_Mode = DMA_Mode_Circular; // Циклический режим для постоянного приема
    _dmaInit.DMA_Priority = DMA_Priority_High;
    _dmaInit.DMA_FIFOMode = DMA_FIFOMode_Disable;
    _dmaInit.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    _dmaInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    _dmaInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    DMA_Init(_stream, &_dmaInit);
}

void DMA_Controller::InitM2P(uint32_t memAddr, uint32_t periphAddr, uint32_t bufferSize) {
    InitP2M(periphAddr, memAddr, bufferSize); // Переиспользуем базу
    _dmaInit.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    _dmaInit.DMA_Mode = DMA_Mode_Normal; // Для передачи обычно используется Normal
    DMA_Init(_stream, &_dmaInit);
}

void DMA_Controller::Start() {
    DMA_Cmd(_stream, ENABLE);
}

void DMA_Controller::Stop() {
    DMA_Cmd(_stream, DISABLE);
}

bool DMA_Controller::IsTransferComplete() {
    // Пример для потока 7 (нужно адаптировать под конкретный флаг или использовать прерывания)
    return DMA_GetFlagStatus(_stream, DMA_FLAG_TCIF7) != RESET;
}