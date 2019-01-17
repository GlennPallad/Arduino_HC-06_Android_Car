/**
 ******************************************************************************
 * @file 		avoidance.c
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file provides all Avoidance Mode functions.
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

#include "avoidance_mode.h"
#include "mode_config.h"
#include "l298n.h"
#include "pwm.h"
#include "hc_sr04.h"

void setAvoidanceMode(void)
{
	switch(finalCmd) {
		case 0:
			/* Start. */
			if (distanceMillimeter > MIN_DISTANCE_MILLIMETER)
			{
				if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0 && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
				{
					/* Both side aux anticollision, stop. */
					setAStop();
					setBStop();
					pwm2Level = 0;
					pwm3Level = 0;
				} else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0) {
					/* Left side aux anticollision, turns to right. */
					setAForward();
					setBBackward();
					pwm2Level = 520;
					pwm3Level = 580;
				} else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0) {
					/* Right side aux anticollision, turns to left. */
					setBForward();
					setABackward();
					pwm2Level = 580;
					pwm3Level = 520;
				} else {
					/* Normal. */
					setAForward();
					setBForward();
					pwm2Level = 420; 	/*!< 0 to 1023 */
					pwm3Level = 420; 	/*!< 0 to 1023 */
				}
			} else {
				/* Supersonic wave anticollision. */
				setAForward();
				setBBackward();
				pwm2Level = 520;
				pwm3Level = 580;
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
}
