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
#include "stm32f4xx_hal.h" //���� ���������� �ּ� ����
#include "stm32f4xx_it.h" //���ͷ�Ʈ ��뿡 �ʿ��� ����ğ�
#include "string.h"
GPIO_InitTypeDef GPIO_Init_Struct,LD,MOTOR,PWM;// GPIO�� �ʱ�ȭ�� ���� ����ü���� ������ ����
UART_HandleTypeDef UartHandle; // UART�� �ʱ�ȭ�� ���� ����ü���� ������ ����

// ----- main() ---------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

// UART ����� ���� ����
#define TxBufferSize (countof(TxBuffer) - 1) // �ۚ� ���� ������ ����
#define RxBufferSize 0xFF // ���� ���� ����� 0xFF�� ����
#define countof(a) (sizeof(a) / sizeof(*(a))) // ������ ������

// UART ��߿� ���� ����
uint8_t TxBuffer[] = "UART Example 1 (Transmission Success !!)\n\r";
uint8_t RxBuffer[RxBufferSize];
uint8_t ErBuffer[30];  //uint8_t->char   str warning deleted
// -- UART�� �ʱ⼳���� ���� �Լ�
void UART_config(void) // USART2_TX(PA2), USART2_RX(PA3)
{
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_USART2_CLK_ENABLE()
	;

	GPIO_Init_Struct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_Init_Struct.Mode = GPIO_MODE_AF_PP;
	GPIO_Init_Struct.Pull = GPIO_NOPULL;
	GPIO_Init_Struct.Speed = GPIO_SPEED_LOW;
	GPIO_Init_Struct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_Init_Struct);

	// UART�� ���� ���� ����
	UartHandle.Instance = USART2;
	UartHandle.Init.BaudRate = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;//������? ������? �������� �������̶�� stop��Ʈ�� 1��Ʈ ���� 2��Ʈ����
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	// UART ���������� UartHandle�� ������ ������ �ʱ�ȭ ��
	HAL_UART_Init(&UartHandle);
	 //UART ���ͷ�Ʈ �켱 ���� ���� �� �R��ȭ
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}
void LD_Config(){
	__HAL_RCC_GPIOC_CLK_ENABLE();
	LD.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
	LD.Mode=GPIO_MODE_OUTPUT_PP;
	LD.Pull=GPIO_NOPULL;
	LD.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOC,&LD);
}
void MOTOR_Config(){
	__HAL_RCC_GPIOB_CLK_ENABLE();
	MOTOR.Pin=GPIO_PIN_6|GPIO_PIN_7;//PIN_6=MOTOR_Positive, PIN_7=MOTOR_Negative
	MOTOR.Mode=GPIO_MODE_OUTPUT_PP;
	MOTOR.Pull=GPIO_NOPULL;
	MOTOR.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB,&MOTOR);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	PWM.Pin=GPIO_PIN_6;
	PWM.Mode=GPIO_MODE_OUTPUT_PP;
	PWM.Pull=GPIO_NOPULL;
	PWM.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA,&PWM);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,1);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,0);
}
int main(int argc, char* argv[]) {
	/* Configure UART */
	UART_config();
	LD_Config();
	MOTOR_Config();
	// TxBuffer�� ����Ǿ� �ִ� ������ PC�� ������.
	HAL_UART_Transmit(&UartHandle, (uint8_t*) TxBuffer, TxBufferSize, 0xFFFF);
	HAL_UART_Receive_IT(&UartHandle, (uint8_t*) RxBuffer, 2);
	while (1) {
	}

}

/* UART ���ͷ�Ʈ Callback �Լ� */ // Interrupt ��� ������ �Ϸ�Ǹ� ȣ��Ǵ� callback �Լ�
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
// ������ �Ϸ�Ǹ� ���ߵ� �����͸� �״�� �ۚ�
	RxBuffer[2] = '\n';
	RxBuffer[3] = '\r';
	HAL_UART_Transmit(huart, (uint8_t*) RxBuffer, 4, 0xFFFF);
	if (RxBuffer[0] == 'L') {
		switch (RxBuffer[1]) {
		case '1':
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
			break;
		case '2':
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
			break;
		case '3':
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);
			break;
		case '4':
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5);
			break;
		default:
			strcpy(ErBuffer, "LD NUM ERROR \n\r");
			HAL_UART_Transmit(&UartHandle, (uint8_t*) ErBuffer,
					countof(ErBuffer) - 1, 0xFFFF);
			break;
		}
	} else if (RxBuffer[0] == 'M') {
		if (RxBuffer[1] == '1') {
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
		} else
			strcpy(ErBuffer, "MOTOR NUM ERROR \n\r");
		HAL_UART_Transmit(&UartHandle, (uint8_t*) ErBuffer,
				countof(ErBuffer) - 1, 0xFFFF);
	} else {
		strcpy(ErBuffer, "Input ERROR \n\r");
		HAL_UART_Transmit(&UartHandle, (uint8_t*) ErBuffer,
				countof(ErBuffer) - 1, 0xFFFF);
	}

	HAL_UART_Receive_IT(&UartHandle, (uint8_t*) RxBuffer, 2);
}
// ----------------------------------------------------------------------------
