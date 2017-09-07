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

GPIO_InitTypeDef GPIO_Init_Struct; // GPIO�� �ʱ�ȭ�� ���� ����ü���� ������ ����
ADC_HandleTypeDef AdcHandler; // ADC�� �ʱ�ȭ�� ���� ����ü���� ������ ����
ADC_ChannelConfTypeDef sConfig;
int adc_value;// ADC�� ���� ����

/*������ƾ*/
static void ms_delay_int_count(volatile unsigned int nTime) // ms ����
{
nTime = (nTime * 14000);
for(; nTime > 0; nTime--);
}
static void us_delay_int_count(volatile unsigned int nTime) // us ����
{
nTime = (nTime * 12);
for(; nTime > 0; nTime--);
}
void LED_Config() {
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	GPIO_Init_Struct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_Init_Struct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init_Struct.Pull = GPIO_NOPULL;
	GPIO_Init_Struct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_Init_Struct);
}
void ADC_Config(){
//	__HAL_RCC_ADC1_CLK_ENABLE();
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	GPIO_Init_Struct.Pin=GPIO_PIN_4;
//	GPIO_Init_Struct.Mode=GPIO_MODE_ANALOG;
//	HAL_GPIO_Init(GPIOA,&GPIO_Init_Struct);
//	AdcHandler.Instance=ADC1;
//	AdcHandler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV2;
//	AdcHandler.Init.Resolution=ADC_RESOLUTION_12B;
//	AdcHandler.Init.DataAlign=ADC_DATAALIGN_RIGHT;
//	AdcHandler.Init.ScanConvMode=DISABLE;
//	AdcHandler.Init.ContinuousConvMode=ENABLE;
//	AdcHandler.Init.NbrOfConversion=1;
//	AdcHandler.Init.ExternalTrigConv=ADC_INJECTED_SOFTWARE_START;
//	HAL_ADC_Init(&AdcHandler);
//
//	sConfig.Channel=ADC_CHANNEL_4;
//	sConfig.Rank=1;
//	sConfig.SamplingTime=ADC_SAMPLETIME_3CYCLES;
//	HAL_ADC_ConfigChannel(&AdcHandler,&sConfig);
//
//	HAL_NVIC_SetPriority(ADC_IRQn,0,0);
//	HAL_NVIC_EnableIRQ(ADC_IRQn);

	__HAL_RCC_ADC1_CLK_ENABLE(); // ADC Ŭ�� �R��ȭ
	__HAL_RCC_GPIOA_CLK_ENABLE(); // ADC ������ ����� GPIOx �R��ȭ(VR:PA4)
	GPIO_Init_Struct.Pin = GPIO_PIN_4; // GPIO���� ����� PIN ����
	GPIO_Init_Struct.Mode = GPIO_MODE_ANALOG; // Input Analog Mode ���
	HAL_GPIO_Init(GPIOA, &GPIO_Init_Struct);
	AdcHandler.Instance = ADC1; // ADC1 ����
	AdcHandler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2; // ADC clock prescaler
	AdcHandler.Init.Resolution = ADC_RESOLUTION_12B; // ADC resolution
	AdcHandler.Init.DataAlign = ADC_DATAALIGN_RIGHT; // ADC data alignment
	AdcHandler.Init.ScanConvMode = DISABLE; // ADC scan ��� ���R��ȭ
	AdcHandler.Init.ContinuousConvMode = ENABLE; // ADC ���� ��� �R��ȭ
	AdcHandler.Init.NbrOfConversion = 1; // ADC ���Q ���� ����
	AdcHandler.Init.ExternalTrigConv = ADC_SOFTWARE_START; // ADC �ܺ� Ʈ���� OFF
	HAL_ADC_Init(&AdcHandler); // ADC�� ������ ������ �ʱ�ȭ
	sConfig.Channel = ADC_CHANNEL_4; // ADC ä�� ����(PA�� ä�� 4��)
	sConfig.Rank = 1; // ADC ä�� ���� ����
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES; // ADC ���ø� Ÿ�� ����(3Ŭ��)
	HAL_ADC_ConfigChannel(&AdcHandler, &sConfig); // ä�� ����
	/* NVIC configuration */
	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);
}
int
main(int argc, char* argv[])
{
//	LED_Config();
//	ADC_Config();
//	HAL_ADC_Start_IT(&AdcHandler);
	/* Configure LED_1(PA2), LED_2(PA3) */
	LED_Config();
	/* Configure ADC */
	ADC_Config();
	// Interrupt ������� ADC ���Q ����
	HAL_ADC_Start_IT(&AdcHandler); // ADC�� ���۽�Ŵ, ���ͷ�Ʈ �R��ȭ
  // At this stage the system clock should have already been configured
  // at high speed.

  // Infinite loop
  while (1)
    {
       // Add your code here.


    }
}
/*���ͷ�Ʈ ��ƾ*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandler)
{
	  adc_value = HAL_ADC_GetValue(AdcHandler); // ADC ���Q ��� ���� ����
	  // ���Q ������� �̿��Ͽ� LED�� �ֱ⸦ ����
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,1);
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,1);
		  us_delay_int_count(adc_value);
		  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,0);
		  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,0);
		  us_delay_int_count(3000);
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
