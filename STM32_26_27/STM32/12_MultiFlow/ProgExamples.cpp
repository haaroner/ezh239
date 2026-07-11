//-----NOTIFICATIONS--------
TaskHandle_t xHandler;

xTaskNotifyGive(xHandler);

ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//---------------------/

//-------StreamBuffer-------

#include "FreeRTOS.h"
#include "stream_buffer.h"

// Хендл буфера
StreamBufferHandle_t xUartTxBuffer;

// Эмуляция функции отправки байта в UART (зависит от вашего железа)
void UART_SendByte_Polling(uint8_t data) {
    // В реальном чипе это выглядит так:
    // while(!(UART->SR & UART_FLAG_TXE)); // Ждем готовности передатчика
    // UART->DR = data;                   // Пишем байт
}

// ЗАДАЧА 1: Источник данных (Писатель)
void vTaskSensorProducer(void *pvParameters) {
    char *msg = "Data Log: OK\n";
    
    while(1) {
        // Просто кидаем строку в буфер. 
        // Если буфер полон, задача "уснет" и не будет тормозить процессор пустым циклом.
        xStreamBufferSend(xUartTxBuffer, (void *)msg, strlen(msg), portMAX_DELAY);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // Генерируем данные раз в секунду
    }
}

// ЗАДАЧА 2: Драйвер UART (Читатель-передатчик)
void vTaskUartDriver(void *pvParameters) {
    uint8_t txByte;
    
    while(1) {
        // Читаем из буфера по 1 байту. 
        // Если буфер пуст, задача спит (Состояние Blocked) и не тратит CPU.
        // Trigger Level = 1, поэтому просыпаемся сразу при появлении хотя бы 1 байта.
        size_t received = xStreamBufferReceive(xUartTxBuffer, &txByte, 1, portMAX_DELAY);
        
        if(received > 0) {
            // Отправляем байт в физический UART (опрос регистра готовности внутри функции)
            UART_SendByte_Polling(txByte);
        }
    }
}

int main(void) {
    // Создаем буфер. Размер 64 байта, просыпаться при наличии 1 байта.
    xUartTxBuffer = xStreamBufferCreate(64, 1);

    if (xUartTxBuffer != NULL) {
        xTaskCreate(vTaskSensorProducer, "Producer", 128, NULL, 1, NULL);
        xTaskCreate(vTaskUartDriver,   "UART_Drv", 128, NULL, 2, NULL); // Приоритет выше!

        vTaskStartScheduler();
    }
    return 0;
}

//------Interrupts-------
// Обработчик прерывания (например, по нажатию кнопки или готовности UART)
void EXTI0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; // Изначально - нет нужды переключать

    // Даем семафор из прерывания
    xSemaphoreGiveFromISR(xGlobalSemaphore, &xHigherPriorityTaskWoken);

    // Вызываем принудительное переключение контекста, если xHigherPriorityTaskWoken стал pdTRUE
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

//---------queue-----------------

void vTaskSensorSampler(void *pvParameters) {
    SensorData_t xDataToSend;
    xDataToSend.DeviceID = 1;

    while(1) {
        // 1. Имитируем получение данных
        xDataToSend.Temperature = 25.5f + (rand() % 10); 
        xDataToSend.Timestamp = xTaskGetTickCount();

        // 2. Отправляем структуру в очередь
        // &xDataToSend - адрес локальной переменной
        // 100 - ждем 100 тиков, если очередь полна, прежде чем сдаться
        BaseType_t xStatus = xQueueSend(xSensorQueue, &xDataToSend, 100);

        if(xStatus != pdPASS) {
            // Очередь полна, данные не ушли
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Ждем 1 секунду
    }
}


void vTaskDataProcessor(void *pvParameters) {
    SensorData_t xReceivedData;

    while(1) {
        // 1. Ждем данные из очереди
        // portMAX_DELAY - задача будет "спать" вечно, пока не придут данные
        if(xQueueReceive(xSensorQueue, &xReceivedData, portMAX_DELAY) == pdPASS) {
            // 2. Данные успешно скопированы в xReceivedData
            // Теперь можно их обработать 
            displayData("ID: %d, Temp: %.2f\n", xReceivedData.DeviceID, xReceivedData.Temperature);
        }
    }
}
//-------------Semaphor-----------
#include "FreeRTOS.h"
#include "semphr.h"

SemaphoreHandle_t xBinarySemaphore;

// Задача-отправитель (Событие)
void vTaskProducer(void *pvParameters) {
    while(1) {
        // Делаем что-то полезное...
        vTaskDelay(pdMS_TO_TICKS(2000)); 

        // Сигнализируем, что данные готовы
        xSemaphoreGive(xBinarySemaphore); 
        printf("Producer: Сигнал отправлен\n");
    }
}

// Задача-потребитель (Обработчик)
void vTaskConsumer(void *pvParameters) {
    while(1) {
        // Ждем сигнала бесконечно
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdPASS) {
            // Если мы здесь — мы получили "разрешение"
            printf("Consumer: Сигнал получен, обрабатываю...\n");
        }
    }
}

int main(void) {
    // Важно: БИНАРНЫЙ семафор после создания пуст (равен 0)!
    xBinarySemaphore = xSemaphoreCreateBinary();

    if (xBinarySemaphore != NULL) {
        xTaskCreate(vTaskProducer, "Producer", 128, NULL, 1, NULL);
        xTaskCreate(vTaskConsumer, "Consumer", 128, NULL, 2, NULL);
        vTaskStartScheduler();
    }
    return 0;
}
//-------------groups
#include "FreeRTOS.h"
#include "event_groups.h"

// Определяем биты событий
#define BIT_SENSOR_READY (1 << 0)
#define BIT_WIFI_READY   (1 << 1)

EventGroupHandle_t xSystemEvents;

// Задача 1: Инициализация датчика
void vSensorInitTask(void *pv) {
    vTaskDelay(pdMS_TO_TICKS(1000)); // Имитация долгой инициализации
    xEventGroupSetBits(xSystemEvents, BIT_SENSOR_READY);
    vTaskDelete(NULL); 
}

// Задача 2: Инициализация Wi-Fi
void vWifiInitTask(void *pv) {
    vTaskDelay(pdMS_TO_TICKS(2000)); // Wi-Fi грузится еще дольше
    xEventGroupSetBits(xSystemEvents, BIT_WIFI_READY);
    vTaskDelete(NULL);
}

// Главная задача
void vMainAppTask(void *pv) {
    // Ждем установки ОБОИХ битов
    // pdTRUE - сбросить биты при выходе (чтобы не сработали повторно)
    // pdTRUE - ждать ВСЕ указанные биты (Логическое И)
    EventBits_t uxBits = xEventGroupWaitBits(
                            xSystemEvents, 
                            BIT_SENSOR_READY | BIT_WIFI_READY, 
                            pdTRUE, pdTRUE, portMAX_DELAY);

    if ((uxBits & (BIT_SENSOR_READY | BIT_WIFI_READY)) == (BIT_SENSOR_READY | BIT_WIFI_READY)) {
        printf("Все системы готовы. Запуск приложения...\n");
    }
}

int main(void) {
    xSystemEvents = xEventGroupCreate();

    if (xSystemEvents != NULL) {
        xTaskCreate(vSensorInitTask, "SensInit", 128, NULL, 1, NULL);
        xTaskCreate(vWifiInitTask, "WifiInit", 128, NULL, 1, NULL);
        xTaskCreate(vMainAppTask, "MainApp", 256, NULL, 2, NULL);

        vTaskStartScheduler();
    }
    return 0;
}