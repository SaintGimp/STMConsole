#ifndef CUSTOMUART_H_
#define CUSTOMUART_H_

#include "stm32f4xx_hal.h"

__weak void customUart_HAL_UART_RxByteCallback(UART_HandleTypeDef *huart);
static HAL_StatusTypeDef customUart_UART_Receive_IT(UART_HandleTypeDef *huart);
void customUart_HAL_UART_IRQHandler(UART_HandleTypeDef *huart);

#endif /* CUSTOMUART_H_ */
