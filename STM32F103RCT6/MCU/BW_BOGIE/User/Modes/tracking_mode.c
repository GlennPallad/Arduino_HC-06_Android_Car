/**
 ******************************************************************************
 * @file 		tracking.c
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file provides all Tracking Mode functions.
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

#include "tracking_mode.h"
#include "mode_config.h"
#include "l298n.h"
#include "pwm.h"
#include "hc_sr04.h"
#include "wulfric_usart.h" 	/* For debug. */
#include "num_to_ascii.h" 	/* For debug. */
#include "wulfric_adc.h"

// #define 	TRACKING_MODE_DEBUG
int printed = 0;

/* The darker the color, the larger the value will be. */
uint32_t ADC1_CH10Value;
uint32_t ADC1_CH11Value;
uint32_t ADC1_CH12Value;
int pwm2LevelSigned;
int pwm3LevelSigned;
float ADC1_CH10ValueFloat;
float ADC1_CH11ValueFloat;
float ADC1_CH12ValueFloat;

void setTrackingMode(void)
{
#ifndef TRACKING_MODE_DEBUG
	switch(finalCmd) {
		case 0:
			/* Start. */
			if (distanceMillimeter > MIN_DISTANCE_MILLIMETER)
			{
				ADC1_CH10Value = (uint32_t)ADC_ConvertedValue[0];
				ADC1_CH11Value = (uint32_t)ADC_ConvertedValue[1];
				ADC1_CH12Value = (uint32_t)ADC_ConvertedValue[2];
				ADC1_CH10ValueFloat = (float)ADC1_CH10Value;
				ADC1_CH11ValueFloat = (float)ADC1_CH11Value;
				ADC1_CH12ValueFloat = (float)ADC1_CH12Value;
				if (ADC1_CH10Value ==  ADC1_CH11Value && ADC1_CH11Value == ADC1_CH12Value)
				{
					/* 3 infrared sensors' value all equal, this usually happens when no reflected
						infrared received. */
					setAStop();
					setBStop();
					pwm2Level = 0;
					pwm3Level = 0;
				} else if (ADC1_CH10Value > MIN_RECOG_THRESHOLD || ADC1_CH11Value > MIN_RECOG_THRESHOLD || ADC1_CH12Value > MIN_RECOG_THRESHOLD ) {
					/* Line captured. */
					if (ADC1_CH10Value < ADC1_CH11Value && ADC1_CH11Value > ADC1_CH12Value)
					{
						/* Normal. */
						setAForward();
						setBForward();
						pwm2Level = BASE_PWM_LEVEL;
						pwm3Level = BASE_PWM_LEVEL;
					} else if ((ADC1_CH10Value > ADC1_CH11Value || ADC1_CH10Value == ADC1_CH11Value) && (ADC1_CH10Value > ADC1_CH12Value)) {
						/* The bogie deviated to right. */
						setAForward();
						setBForward();
						pwm2Level = 0;
						pwm3Level = BASE_PWM_LEVEL;
					} else if ((ADC1_CH12Value > ADC1_CH11Value || ADC1_CH12Value == ADC1_CH11Value) && (ADC1_CH12Value > ADC1_CH10Value)) {
						/* The bogie deviated to left. */
						setAForward();
						setBForward();
						pwm2Level = BASE_PWM_LEVEL;
						pwm3Level = 0;
					} else {
						/* Unexpected situations. */
						setAStop();
						setBStop();
						pwm2Level = 0;
						pwm3Level = 0;
					}
				} else {
					/*!< No line captured. */
					setAStop();
					setBStop();
					pwm2Level = 0;
					pwm3Level = 0;
				}
			} else {
				/* Anticollision */
				setAStop();
				setBStop();
				pwm2Level = 0;
				pwm3Level = 0;
			}
			break;
		case 1:
			/* Stop. */
			setAStop();
			setBStop();
			pwm2Level = 0;
			pwm3Level = 0;
			break;
		case 2:
			/* Disconnect. */
			setAStop();
			setBStop();
			pwm2Level = 0;
			pwm3Level = 0;
			break;
		default:
			break;
	}
#else
	char ADC1_CH10ValueStr[36]; 	/* For debug. */
	char ADC1_CH11ValueStr[36]; 	/* For debug. */
	char ADC1_CH12ValueStr[36]; 	/* For debug. */
	uint32_t ADC1_CH10Value = (uint32_t)ADC_ConvertedValue[0]; 	/* For debug. */
	uint32_t ADC1_CH11Value = (uint32_t)ADC_ConvertedValue[1]; 	/* For debug. */
	uint32_t ADC1_CH12Value = (uint32_t)ADC_ConvertedValue[2]; 	/* For debug. */
	numberToASCIIDec(ADC1_CH10Value, ADC1_CH10ValueStr, sizeof(ADC1_CH10ValueStr)); 	 	/* For debug. */
	numberToASCIIDec(ADC1_CH11Value, ADC1_CH11ValueStr, sizeof(ADC1_CH11ValueStr)); 	 	/* For debug. */
	numberToASCIIDec(ADC1_CH12Value, ADC1_CH12ValueStr, sizeof(ADC1_CH12ValueStr)); 	 	/* For debug. */

	switch(finalCmd) {
		case 0:
			/* Start. */
			USART_SendString(USART3, "ADC1_CH10ValueStr:"); 	/* For debug. */
			USART_SendString(USART3, ADC1_CH10ValueStr); 		/* For debug. */
			USART_SendString(USART3, "ADC1_CH11ValueStr:"); 	/* For debug. */
			USART_SendString(USART3, ADC1_CH11ValueStr); 		/* For debug. */
			USART_SendString(USART3, "ADC1_CH12ValueStr:"); 	/* For debug. */
			USART_SendString(USART3, ADC1_CH12ValueStr); 		/* For debug. */
			USART_SendString(USART3, "\n"); 					/* For debug. */
			for (int i = 0; i < 500000; ++i) 	/* For debug. */
			{
			}
		case 1:
			/* Stop. */
			setAStop();
			setBStop();
			pwm2Level = 0;
			pwm3Level = 0;
			break;
		case 2:
			/* Disconnect. */
			setAStop();
			setBStop();
			pwm2Level = 0;
			pwm3Level = 0;
			break;
		default:
			break;
	}
#endif
}
