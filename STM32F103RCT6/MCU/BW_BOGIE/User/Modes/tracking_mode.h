/**
 ******************************************************************************
 * @file 		tracking.h
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file is header of tracking.c.
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

#ifndef TRACKING_H
#define TRACKING_H

#include "stm32f10x.h"

#define 	BASE_PWM_LEVEL 			430 	/*!< From 0 to 1023, set this value too small may cause
												the motors underpowered, but too large may cause fails
												since speed amendment can not follow the travel speed. */
#define 	MIN_RECOG_THRESHOLD 	190 	/*!< Min recognizable ADC value threshold, for white paper material,
												and under normal room temperature, ADC values will not over
												200 commonly. */
// #define 	BASE_PWM_OFFSET 90 				/*!< Even at the same PWM Level, the left and right motors 
// 												have a speed difference. Use this BASE_PWM_OFFSET to decelerate 
// 												the side with the higher speed to achieve balance. */
// #define 	SENS_COEF 		1 				/*!< Sensitivity coefficient, from 0 to 1,
// 												the larger the value, the more sensitive, vice versa. */

extern uint32_t ADC1_CH10Value;
extern uint32_t ADC1_CH11Value;
extern uint32_t ADC1_CH12Value;
extern int pwm2LevelSigned;
extern int pwm3LevelSigned;
extern float ADC1_CH10ValueFloat;
extern float ADC1_CH11ValueFloat;
extern float ADC1_CH12ValueFloat;

void setTrackingMode(void);

#endif
