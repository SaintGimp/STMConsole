// https://github.com/eleciawhite/reusable

// Console IO is a wrapper between the actual in and output and the console code
// In an embedded system, this might interface to a UART driver.

#include <stdio.h>
#include "usart.h"
#include "ringbuffer.h"
#include "consoleIo.h"

ring_buffer_t ring_buffer;
char uart_receive_buffer;

eConsoleError ConsoleIoInit(void)
{
	ring_buffer_init(&ring_buffer);

	if (HAL_UART_Receive_IT(&huart3, (uint8_t *)&uart_receive_buffer, sizeof(uart_receive_buffer)) != HAL_OK)
	{
		Error_Handler();
	}

	// Prevent buffering of printf so everything gets sent immediately
	setbuf(stdout, NULL);

	return CONSOLE_SUCCESS;
}

eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength)
{
	uint8_t i = 0;
	char ch;
	
	while ((i < bufferLength) && (ring_buffer_dequeue(&ring_buffer, &ch)))
	{
		buffer[i] = (uint8_t) ch;
		i++;

		printf("%c", ch);
		if (ch == '\r')
		{
			printf("\n");
		}
	}
	*readLength = i;

	return CONSOLE_SUCCESS;
}

eConsoleError ConsoleIoSend(const uint8_t *buffer, const uint32_t bufferLength, uint32_t *sentLength)
{
	printf("%s", (char*)buffer);
	*sentLength = bufferLength;
	return CONSOLE_SUCCESS;
}

eConsoleError ConsoleIoSendString(const char *buffer)
{
	printf("%s", buffer);
	return CONSOLE_SUCCESS;
}

int _write(int file, char* outgoing, int len)
{
	// Could use DMA here but it's not clear to me how buffer
	// lifetime would be managed.
	HAL_UART_Transmit(&huart3, (uint8_t*)outgoing, len, 100);
	return len;
}

void customUart_HAL_UART_RxByteCallback(UART_HandleTypeDef* huart)
{
	ring_buffer_queue(&ring_buffer, uart_receive_buffer);
	// In this customized callback, no need to re-enable the interrupt.
	// The callback will fire for each byte and the most recent one
	// will overwrite the previous one in the buffer.
}
