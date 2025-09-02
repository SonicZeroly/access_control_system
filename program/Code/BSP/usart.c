#include "main.h"
#include <stdio.h>
#include "usart.h"

static void bsp_SetUartParam(USART_TypeDef *Instance, uint32_t BaudRate, uint32_t Parity, uint32_t Mode);
static void USART_SetBaud();

UART_HandleTypeDef huart1;

extern unsigned char uartRxData;

void UART1_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* ʹ�� GPIO TX/RX ʱ�� */
	USART1_TX_GPIO_CLK_ENABLE();
	USART1_RX_GPIO_CLK_ENABLE();
	
	/* ʹ�� USARTx ʱ�� */
	USART1_CLK_ENABLE();	

	/* ����TX���� */
	GPIO_InitStruct.Pin       = USART1_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = USART1_TX_AF;
	HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);	
	
	/* ����RX���� */
	GPIO_InitStruct.Pin = USART1_RX_PIN;
	GPIO_InitStruct.Alternate = USART1_RX_AF;
	HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);

	/* ����NVIC the NVIC for UART */   
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	/* ���ò����ʡ���żУ�� */
	bsp_SetUartParam(USART1,  115200, UART_PARITY_NONE, UART_MODE_TX_RX);

	CLEAR_BIT(USART1->SR, USART_SR_TC);   /* ���TC������ɱ�־ */
    CLEAR_BIT(USART1->SR, USART_SR_RXNE); /* ���RXNE���ձ�־ */
//	SET_BIT(USART1->CR1, USART_CR1_RXNEIE);	/* ʹ��RX�����ж� */

	HAL_UART_Receive_IT(&huart1, &uartRxData, 1);	//�ڲ���������
}

static void bsp_SetUartParam(USART_TypeDef *Instance, uint32_t BaudRate, uint32_t Parity, uint32_t Mode){
	huart1.Instance        = Instance;
	huart1.Init.BaudRate   = BaudRate;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits   = UART_STOPBITS_1;
	huart1.Init.Parity     = Parity;
	huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	huart1.Init.Mode       = Mode;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}

}

int fputc(int ch, FILE *f)
{
	/* ����������ʽ����ÿ���ַ�,�ȴ����ݷ������ */
	/* дһ���ֽڵ�USART1 */
	USART1->DR = ch;
	
	/* �ȴ����ͽ��� */
	while((USART1->SR & USART_SR_TC) == 0)
	{}
	
	return ch;
}

