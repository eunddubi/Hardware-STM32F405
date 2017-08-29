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
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_it.h"
// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via ITM).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the ITM output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
TIM_HandleTypeDef TimHandle; // Ÿ�̸��� �ʱ�ȭ�� ����ü ������ ����
GPIO_InitTypeDef LD1; // GPIO�� �ʱ�ȭ�� ���� ����ü ������ ����

void TIMER_Config(void)
{
/*##-1- Enable peripherals and GPIO Clocks #################################*/
/* TIMx Peripheral clock enable */
__HAL_RCC_TIM2_CLK_ENABLE();
/* Set TIMx instance */
TimHandle.Instance = TIM2; // TIM2 ���
TimHandle.Init.Period = 10000 - 1; // ������Ʈ �̓X�� �߻��� ARR=9999�� ����
TimHandle.Init.Prescaler = 8400 - 1; // Prescaler = 8399�� ����
TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // division�� ������� ����
TimHandle.Init.CounterMode = TIM_COUNTERMODE_DOWN; // Down Counter ��� ����
HAL_TIM_Base_Init(&TimHandle);
/*##-2- Start the TIM Base generation in interrupt mode ####################*/
HAL_TIM_Base_Start_IT(&TimHandle); /* Start Channel1 */
/*##-3- Configure the NVIC for TIMx ########################################*/
HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0); /* Set Interrupt Group Priority */
HAL_NVIC_EnableIRQ(TIM2_IRQn); /* Enable the TIMx global Interrupt */
}

int main(int argc, char* argv[])
{
	/*##-1- Enable GPIOC Clock (to be able to program the configuration registers) */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	/*##-2- Configure PC2 IO in output push-pull mode to drive external LED ###*/
	LD1.Pin = GPIO_PIN_2|GPIO_PIN_3;
	LD1.Mode = GPIO_MODE_OUTPUT_PP;
	LD1.Pull = GPIO_NOPULL;
	LD1.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOC, &LD1);
	/* Configure TIMER */
	TIMER_Config();
	/*�ޟڷ�ƾ���� �ϴ� ��*/
	while(1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
