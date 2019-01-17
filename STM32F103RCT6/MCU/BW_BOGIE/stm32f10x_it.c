/**
 ******************************************************************************
 * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and 
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "wulfric_usart.h"
#include "mode_config.h"
#include "hc_sr04.h"
#include "pwm.h"
#include "num_to_ascii.h"

static char c;
static char temp[20] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
static int RxCounter = 0;
static int error = 0;

int Capture_RiseFlag = 1;

/** @addtogroup STM32F10x_StdPeriph_Template
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
}


void USART3_IRQHandler(void) 	/* For printing debug info to PC. */
{
	uint16_t temp;

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != 0)
	{
		temp = USART_ReceiveData(USART3);
		USART_SendData(USART3, temp);
	}
}

void USART2_IRQHandler(void){
	uint16_t rxByte; 	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != 0)
	{
		/* Read a new byte. */
		rxByte = USART_ReceiveData(USART2);
		/* Send back. */
		// USART_SendData(USART2, rxByte); 	/*!< For debug. */
		c = (char)rxByte;
		if (error == 0)
		{
			/* Store the new char into temp[]. */
			if (c != '\n')
			{
				temp[RxCounter] = c;
				RxCounter++;
				if (RxCounter == 19)
				{
					/* Under the situation of maximum length, temp[19] should be '\n', temp[20] should be '\0',
							if not, an error has occurred, reset RxCounter, drop all following
							new chars(drop the whole message) until meeting a '\n'. */
					RxCounter = 0;
					error = 1;
				}
			} else {
				/* A message ends. */
				temp[RxCounter] = c;
				/* Copy message from temp[] to rawMsg[]. */
				for (int i = 0; i < 20; ++i)
				{
					rawMsg[i] = temp[i];
				}
				/* Reset temp[] and RxCounter. */
				for (int i = 0; i < 20; ++i)
				{
					temp[i] = '\0';
				}
				RxCounter = 0;
			}
		} else {
			if (c != '\n')
			{
				/* Do nothing... drop this new char until meeting a '\n' */
			} else {
				error = 0;
			}
		}
	}
}

void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		if (pwm2Level > 1023)
		{
			pwm2Level = 1023;
		}
		if (pwm2Level < 0)
		{
			pwm2Level = 0;
		}
		TIM2->CCR1 = pwm2Level;
		TIM_ClearITPendingBit (TIM2, TIM_IT_Update);
	}
}

void TIM3_IRQHandler(void){
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		if (pwm3Level > 1023)
		{
			pwm3Level = 1023;
		}
		if (pwm3Level < 0)
		{
			pwm3Level = 0;
		}
		TIM3->CCR1 = pwm3Level;
		TIM_ClearITPendingBit (TIM3, TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		/* Don't do any time consuming operations here! */
		TIM_Cmd(TIM4, DISABLE);
		setTrigLow();
		TIM_Cmd(TIM1, ENABLE);
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
	}
}

void TIM1_CC_IRQHandler(void)
{
	uint32_t time_microsecond;
	float dist_millimeter_float;
	int dist_millimeter_int;
	char str[36];
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		if (Capture_RiseFlag)
		{
			TIM_SetCounter(TIM1, 0);
			TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Falling);
			Capture_RiseFlag = 0;
		} else {
			time_microsecond = TIM_GetCapture1(TIM1);
			dist_millimeter_float = (0.34 * time_microsecond)/2;
			dist_millimeter_int = (int)dist_millimeter_float;
			distanceMillimeter = dist_millimeter_int;
			numberToASCIIDec(dist_millimeter_int, str, sizeof(str));
			// USART_SendString(USART3, str); 	/*!< For debug. */
			TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Rising);
			Capture_RiseFlag = 1;
			hcsr04FinishedFlag = 1;
			TIM_Cmd(TIM1, DISABLE);
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	}
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		hcsr04FinishedFlag = 1;
		// Capture_RiseFlag = 1;
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_Cmd(TIM1, DISABLE);
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
	* @brief  This function handles PPP interrupt request.
	* @param  None
	* @retval None
	*/
/*void PPP_IRQHandler(void)
{
}*/

/**
	* @}
	*/ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
