
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include <string.h>
GPIO_InitTypeDef GPIO_Init_Struct,LD,MOTOR,PWM;
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
uint8_t TxBuffer[] = "UART Example 1 (Transmission Success!!)\n\r";//uint8_t Redefine
uint8_t RxBuffer[RxBufferSize];
uint8_t ErBuffer[30];  //uint8_t->char   str warning deleted

// -- UART�� �ʱ⼳���� ���� �Լ�
void UART_config() {
	// USART2_TX(PA2), USART2_RX(PA3)
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
	GPIO_Init_Struct.Pin = GPIO_PIN_9 | GPIO_PIN_10;//PIN_6=MOTOR PWM,PIN_9=TX,PIN_10=RX
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
	UART_config();
	LD_Config();
	MOTOR_Config();
	// TxBuffer�� ����Ǿ� �ִ� ������ PC�� ������.
	HAL_UART_Transmit(&UartHandle, (uint8_t*) TxBuffer, TxBufferSize, 0xFFFF);

	while (1) {
		// ���� �����͸� RxBuffer�� ��´�.
		if (HAL_UART_Receive(&UartHandle, (uint8_t*) RxBuffer, 2, 200)== HAL_OK) {
			RxBuffer[2] = '\n';
			RxBuffer[3] = '\r';
			// RxBuffer�� ����Ǿ� �ִ� ������ UART�� ������.
			HAL_UART_Transmit(&UartHandle, (uint8_t*) RxBuffer, 4, 5);
			if(RxBuffer[0]=='L'){
				switch (RxBuffer[1]) {
					case '1':
						HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_2);
						break;
					case '2':
						HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_3);
						break;
					case '3':
						HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_4);
						break;
					case '4':
						HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_5);
						break;
					default:
						strcpy(ErBuffer,"LD NUM ERROR \n\r");
						HAL_UART_Transmit(&UartHandle,(uint8_t*)ErBuffer,countof(ErBuffer)-1,0xFFFF);
						break;
				}
			}
			else if(RxBuffer[0]=='M'){
				if(RxBuffer[1]=='1'){
					HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_6);
				}
				else
					strcpy(ErBuffer,"MOTOR NUM ERROR \n\r");
					HAL_UART_Transmit(&UartHandle,(uint8_t*)ErBuffer,countof(ErBuffer)-1,0xFFFF);
			}
			else{
				strcpy(ErBuffer,"Input ERROR \n\r");
				HAL_UART_Transmit(&UartHandle,(uint8_t*)ErBuffer,countof(ErBuffer)-1,0xFFFF);
			}
		}
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
