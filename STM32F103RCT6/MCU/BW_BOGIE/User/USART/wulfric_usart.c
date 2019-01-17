/**
 ******************************************************************************
 * @file 		wulfric_usart.c
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file provides all functions for configuring USARTx in 
 * 				STM32F10x and sending strings.
 ******************************************************************************
 * @attention
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * <h2><center>COPYRIGHT &copy; 2018 WULFRIC LEE </center></h2>
 ******************************************************************************
 */

#include "wulfric_usart.h"

/**
 * @brief Configure mode of USART.
 * @param None
 * @retval None
 */
void USART_Configuration(void){
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART2 */
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	/* USART3 */
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
}

/**
 * @brief Send a string. This only works at 8-bit mode(7 bits data + 1 bit parity).
 *   Note that this function will occupies the processor until transmission finished.
 * @param USARTx: Select the USART or the UART peripheral. 
 *   This parameter can be one of the following values:
 *   USART1, USART2, USART3, UART4 or UART5.
 * @param str: Address of the string.
 * @retval None
 */
void USART_SendString(USART_TypeDef* USARTx, char* str){
	unsigned int i = 0;
	while(* (str + i) != '\0'){
		USART_SendData(USARTx, * (str + i));
		i++;
		while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE)){
			// Wait until TDR empty.
		}
	}
}
