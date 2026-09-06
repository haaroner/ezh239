#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

void PWM_Init(uint16_t prescaler, uint16_t period)
{
    // 1. Включаем тактирование GPIOA и TIM2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. Настраиваем пин PA0 как альтернативный выход (TIM2_CH1)
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate Function Push-Pull
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    // 3. Настройка базового таймера (Time Base)
    TIM_TimeBaseInitTypeDef timer;
    TIM_TimeBaseStructInit(&timer); // Заполняем дефолтными значениями
    timer.TIM_Prescaler = prescaler - 1;
    timer.TIM_Period = period - 1;
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &timer);

    // 4. Настройка Канала 1 в режим ШИМ (PWM)
    TIM_OCInitTypeDef oc;
    TIM_OCStructInit(&oc); // Заполняем дефолтными значениями
    oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_Pulse = 0; // Начальная скважность 0%
    oc.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &oc);

    // 5. Запуск таймера
    TIM_Cmd(TIM2, ENABLE);
}

// Функция установки скважности
void PWM_SetDuty(uint16_t pulse)
{
    TIM_SetCompare1(TIM2, pulse);
}

int main(void)
{
    // Настраиваем TIM2:
    // Предделитель = 72 (при 72 МГц даст частоту счета 1 МГц)
    // Период = 1000 (дает частоту ШИМ = 1 кГц, т.е. 1 МГц / 1000)
    PWM_Init(72, 1000);

    // Устанавливаем скважность 50% (500 из 1000)
    PWM_SetDuty(500);

    while(1)
    {
        // Основной цикл
    }
}