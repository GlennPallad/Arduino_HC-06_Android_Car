/* Standard includes. */
#include <stdio.h>

/* STM standard library*/
#include "stm32f10x.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "queue.h"

#include "wulfric_rcc.h"
#include "wulfric_nvic.h"
#include "wulfric_gpio.h"
#include "wulfric_usart.h"
#include "wulfric_tim.h"
#include "wulfric_adc.h"
#include "wulfric_dma.h"

#include "hc_sr04.h"
#include "mode_config.h"
#include "mode_management.h"

void vTaskDistDetect( void *pvParameters );
void vTaskModeConfig( void *pvParameters );
void vTaskModeManagement( void *pvParameters );
TaskHandle_t xTaskModeManagementHandle;

/*
 * Configure the clocks and other peripherals as required by the demo.
 */
static void prvSetupHardware( void );

int main(int argc, char const *argv[])
{
	prvSetupHardware();

	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	USART_Configuration();
	TIM_Configuration();
	ADC_Configuration();
	DMA_Configuration();
	USART_SendString(USART3, "USART3 OK.\n");

	xTaskCreate( vTaskDistDetect, "vTaskDisDet", 1000, NULL, 4, NULL );
	xTaskCreate( vTaskModeConfig, "vTaskMoCon", 1000, NULL, 2, NULL );
	xTaskCreate( vTaskModeManagement, "vTaskMoMan", 1000, NULL, 1, &xTaskModeManagementHandle );

	/* Start the scheduler. */
	vTaskStartScheduler();

	while(1) {
		/* The program should never runs to here, if it does, an error has occurred. */
	}	
	return 0;
}

void vTaskDistDetect( void *pvParameters )
{
	const TickType_t xDelay100ms = pdMS_TO_TICKS( 100 );
	/* As per most tasks, this task is implemented in an infinite loop. */
	while(1) {
		calcDistance();
		vTaskDelay( xDelay100ms );
	}
}

void vTaskModeConfig( void *pvParameters )
{
	UBaseType_t uxPriority;
	/* As per most tasks, this task is implemented in an infinite loop. */
	while(1) {
		uxPriority = uxTaskPriorityGet(NULL);
		refreshData();
		vTaskPrioritySet(xTaskModeManagementHandle, (uxPriority + 1));
	}
}

void vTaskModeManagement( void *pvParameters )
{
	UBaseType_t uxPriority;
	/* As per most tasks, this task is implemented in an infinite loop. */
	while(1) {
		uxPriority = uxTaskPriorityGet(NULL);
		manageMode();
		vTaskPrioritySet(NULL, (uxPriority - 2));
	}
}


static void prvSetupHardware( void )
{
	/* Start with the clocks in their expected state. */
	RCC_DeInit();

	/* Enable HSE (high speed external clock). */
	RCC_HSEConfig( RCC_HSE_ON );

	/* Wait till HSE is ready. */
	while( RCC_GetFlagStatus( RCC_FLAG_HSERDY ) == RESET )
	{
	}

	/* 2 wait states required on the flash. */
	*( ( unsigned long * ) 0x40022000 ) = 0x02;

	/* HCLK = SYSCLK */
	RCC_HCLKConfig( RCC_SYSCLK_Div1 );

	/* PCLK2 = HCLK */
	RCC_PCLK2Config( RCC_HCLK_Div1 );

	/* PCLK1 = HCLK/2 */
	RCC_PCLK1Config( RCC_HCLK_Div2 );

	/* PLLCLK = 8MHz * 9 = 72 MHz. */
	RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_9 );

	/* Enable PLL. */
	RCC_PLLCmd( ENABLE );

	/* Wait till PLL is ready. */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	/* Select PLL as system clock source. */
	RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

	/* Wait till PLL is used as system clock source. */
	while( RCC_GetSYSCLKSource() != 0x08 )
	{
	}

	/* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
		| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );

	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );


	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

}
