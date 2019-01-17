/**
 ******************************************************************************
 * @file 		wulfric_tim.c
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 30, 2018
 * @brief 		This file is TIM driver.
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

#include "wulfric_tim.h"

void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/*!< TIM2 & TIM3 configuration */
	TIM_TimeBaseStructure.TIM_Period = (1024-1);
	TIM_TimeBaseStructure.TIM_Prescaler = (200-1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		/* TIM2 */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		/* TIM3 */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	/*!< PWM mode configuration */
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		/* TIM2 */
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
		/* TIM3 */
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/*!< TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	/*!< TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	/* TIM4(Trig turn off) and TIM1(Echo input capture) */
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/* TIM4 */
	TIM_TimeBaseStructure.TIM_Period = (20 - 1); 	/*!< 20 microseconds. As per HC-SR04 
	 													Datasheet, at least 10 microseconds needed
	 													to trigger a emitting of sonic wave. */
	// TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, DISABLE); 	/*!< Set ENABLE to start. */
	/* TIM1 */
	TIM_TimeBaseStructure.TIM_Period = (35294 - 1); 	/*!< As per HC-SR04 Datasheet, Max Range = 4m,
															which is 8m back and forth. Consider the time 
															margin, set 6m as maximun detectable range, 
															which is 12m back and forth, the time for the sonic 
															wave to travel in the air for 12m is (1/340)*12 s,
															that is, 35,294 microseconds approximately. */
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	/* TIM1 input capture */
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM1, TIM_FLAG_Update | TIM_IT_CC1);
	TIM_ITConfig(TIM1, TIM_IT_Update | TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM1, DISABLE); 	/*!< Set ENABLE to start. */
}
