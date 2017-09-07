/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
GPIO_InitTypeDef GPIO_Init_Struct;
UART_HandleTypeDef UartHandle;

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

// UART ����� ���� ����
#define TxBufferSize (countof(TxBuffer) - 1) // �ۚ� ���� ������ ����
#define RxBufferSize 0xFF // ���� ���� ����� 0xFF�� ����
#define countof(a) (sizeof(a) / sizeof(*(a))) // ������ ������
// UART ��߿� ���� ����
uint8_t TxBuffer[] = "UART Example 1 (Transmission Success!!)\n\r";
uint8_t RxBuffer[RxBufferSize];

// -- UART�� �ʱ⼳���� ���� �Լ�
void UART_config() { // USART2_TX(PA2), USART2_RX(PA3)
	// UART�� Ŭ���� �R��ȭ
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	__HAL_RCC_USART2_CLK_ENABLE();
//	// GPIO A��Ʈ 2�� ���� USART Tx, 3�� ���� USART Rx �� ����
//	GPIO_Init_Struct.Pin=GPIO_PIN_2|GPIO_PIN_3;
//	GPIO_Init_Struct.Mode=GPIO_MODE_AF_PP;
//	GPIO_Init_Struct.Pull=GPIO_NOPULL;
//	GPIO_Init_Struct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
//	GPIO_Init_Struct.Alternate=GPIO_AF7_USART2;
//	HAL_GPIO_Init(GPIOA,&GPIO_Init_Struct);
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_USART1_CLK_ENABLE()
	;
	GPIO_Init_Struct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_Init_Struct.Mode = GPIO_MODE_AF_PP;
	GPIO_Init_Struct.Pull = GPIO_NOPULL;
	GPIO_Init_Struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init_Struct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_Init_Struct);
//	// UART�� ���� ���� ����
//	UartHandle.Instance= USART2;
	UartHandle.Instance = USART1;
	UartHandle.Init.BaudRate = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;	//dataũ�� 8��Ʈ?
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	// UART ���������� UartHandle�� ������ ������ �ʱ�ȭ ��
	HAL_UART_Init(&UartHandle);
}

int main(int argc, char* argv[]) {
	UART_config();

	// TxBuffer�� ����Ǿ� �ִ� ������ PC�� ������.
	HAL_UART_Transmit(&UartHandle, (uint8_t*) TxBuffer, TxBufferSize, 0xFFFF);

	while (1) {
		// ���� �����͸� RxBuffer�� ��´�.
		if (HAL_UART_Receive(&UartHandle, (uint8_t*) RxBuffer, 3, 5)== HAL_OK) {
			RxBuffer[3] = '\n';
			RxBuffer[4] = '\r';
			// RxBuffer�� ����Ǿ� �ִ� ������ UART�� ������.
			HAL_UART_Transmit(&UartHandle, (uint8_t*) RxBuffer, 5, 5);
		}
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
