#include <stdbool.h>
#include "main.h"
#include "gpio.h"
#include "console.h"
#include "app.h"

#define HALF_SECOND 500    // in millisecond ticks

uint32_t lastRun = 0U;
uint32_t nowTime;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    HAL_GPIO_TogglePin( LD3_GPIO_Port, LD3_Pin);
	printf("Button pressed\r\n");
}

void ApplicationLoop()
{
	ConsoleInit();

	while (1)
	{
		ConsoleProcess();

		nowTime = HAL_GetTick();
		if ((nowTime - lastRun) >= HALF_SECOND * 2)
		{
			HAL_GPIO_TogglePin( LD4_GPIO_Port, LD4_Pin);
			lastRun = nowTime;
		}
	}
}
