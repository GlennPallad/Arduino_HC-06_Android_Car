/**
 ******************************************************************************
 * @file 		free_mode.c
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file provides all Free Mode functions.
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

#include <math.h>

#include "free_mode.h"
#include "mode_config.h"
#include "l298n.h"
#include "pwm.h"
#include "hc_sr04.h"

void setFreeMode(void)
{
	switch(finalCmd) {
		case 2:
			/* Disconnect. */
			setAStop();
			setBStop();
			pwm2Level = 0;
			pwm3Level = 0;
			break;
		case 3:
			/* Set pwmLevels. */
			if (finalFactorX == 0 && finalFactorY == 0) {
				/* Origin. */
				setAStop();
				setBStop();
				pwm2Level = 0;
				pwm3Level = 0;
			} else if (finalFactorX > 0 && finalFactorY == 0) {
				/* + X-axis. */
				if (distanceMillimeter > MIN_DISTANCE_MILLIMETER)
				{
					setAForward();
					setBStop();
					pwm2Level = (int)floorf(finalFactorX * 1023);
					pwm3Level = 0;
				} else {
					/* Anticollision */
					setAStop();
					setBStop();
					pwm2Level = 0;
					pwm3Level = 0;
				}
			} else if (finalFactorX > 0 && finalFactorY > 0) {
				/* 1st quadrant. */
				if (distanceMillimeter > MIN_DISTANCE_MILLIMETER)
				{
					setAForward();
					setBForward();
					pwm2Level = (int)floorf(sqrt(pow(finalFactorX, 2) + pow(finalFactorY, 2)) * 1023);
					pwm3Level = (int)floorf(finalFactorY * 1023);
				} else {
					/* Anticollision */
					setAStop();
					setBStop();
					pwm2Level = 0;
					pwm3Level = 0;
				}
			} else if (finalFactorX == 0 && finalFactorY > 0) {
				/* + Y-axis. */
				if (distanceMillimeter > MIN_DISTANCE_MILLIMETER)
				{
					setAForward();
					setBForward();
					pwm2Level = (int)floorf(finalFactorY * 1023);
					pwm3Level = (int)floorf(finalFactorY * 1023);
				} else {
					/* Anticollision */
					setAStop();
					setBStop();
					pwm2Level = 0;
					pwm3Level = 0;
				}
			} else if (finalFactorX < 0 && finalFactorY > 0) {
				/* 2nd quadrant. */
				if (distanceMillimeter > MIN_DISTANCE_MILLIMETER)
				{
					setAForward();
					setBForward();
					pwm2Level = (int)floorf(finalFactorY * 1023);
					pwm3Level = (int)floorf(sqrt(pow(finalFactorX, 2) + pow(finalFactorY, 2)) * 1023);
				} else {
					/* Anticollision */
					setAStop();
					setBStop();
					pwm2Level = 0;
					pwm3Level = 0;
				}
			} else if (finalFactorX < 0 && finalFactorY == 0) {
				/* - X-axis. */
				if (distanceMillimeter > MIN_DISTANCE_MILLIMETER)
				{
					setAStop();
					setBForward();
					pwm2Level = 0;
					pwm3Level = (int)floorf((-finalFactorX) * 1023);
				} else {
					/* Anticollision */
					setAStop();
					setBStop();
					pwm2Level = 0;
					pwm3Level = 0;
				}
			} else if (finalFactorX < 0 && finalFactorY < 0) {
				/* 3rd quadrant. */
				setABackward();
				setBBackward();
				pwm2Level = (int)floorf((-finalFactorY) * 1023);
				pwm3Level = (int)floorf(sqrt(pow(finalFactorX, 2) + pow(finalFactorY, 2)) * 1023);
			} else if (finalFactorX == 0 && finalFactorY < 0) {
				/* - Y-axis. */
				setABackward();
				setBBackward();
				pwm2Level = (int)floorf((-finalFactorY) * 1023);
				pwm3Level = (int)floorf((-finalFactorY) * 1023);
			} else {
				/* 4th quadrant. */
				setABackward();
				setBBackward();
				pwm2Level = (int)floorf(sqrt(pow(finalFactorX, 2) + pow(finalFactorY, 2)) * 1023);
				pwm3Level = (int)floorf((-finalFactorY) * 1023);
			}
			break;
		default:
			/* An error occurred, do nothing... */
			return;
	}
}
