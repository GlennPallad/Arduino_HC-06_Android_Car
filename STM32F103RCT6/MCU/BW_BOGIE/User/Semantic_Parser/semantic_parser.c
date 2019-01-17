/**
 ******************************************************************************
 * @file 		semantic_parser.c
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file is for semantic parsing of messages received by
 * 				Bluetooth SPP devices(e.g. HC-0x).
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

#include "semantic_parser.h"

static int isNumber(char c);

/**
 * @brief Parsing messages received by Bluetooth SPP,
 * 		The message format is composed of 14 bytes at most, 3 bytes at least as shown below:
 * 				Most:
 * 					|M|C|[-]|X|[.]|[X]|[X]|,|[-]|Y|[.]|[Y]|[Y]|"\n"|
 * 				Least:
 * 					|M|C|"\n"|
 * 		Where:
 * 		M = Mode Code, which can be
 * 				0 = Tracking Mode
 * 				1 = Avoidance Mode
 * 				2 = Free Mode
 * 		C = Command Code, which can be
 * 				0 = start
 * 				1 = stop
 * 				2 = disconnect
 * 				3 = factor
 * 		[-] = Minus sign, which only used under Free Mode(|M|C| = |2|3|).
 * 		, = Separator, separate X factor and Y factor.
 * 		X[.XX] and Y[.YY] are factors which only used under Free Mode(|M|C| = |2|3|),
 * 			the range of them is from -1.00 to 1.00.
 * 				X.XX = X factor
 * 				Y.YY = Y factor
 * @param msg: The message received by Bluetooth SPP.
 * @param mode: The mode that BW-BOGIE should run on.
 * @param cmd: The command that BW-BOGIE should execute.
 * @param factorX: The X-axis factor of joystick, commonly for PWM control, equal to 0 by default.
 * @param factorY: The Y-axis factor of joystick, commonly for PWM control, equal to 0 by default.
 * @retval If the return value not equals to 0, an error has occurred.
 */
int semanticParse(char* msg, int* mode, int* cmd, float* factorX, float* factorY)
{
	int factorX_Sign = 1; /*!< 1 = positive, -1 = negative */
	int factorY_Sign = 1; /*!< 1 = positive, -1 = negative */
	int factorX_EndIndex = 0;
	int factorY_StartIndex = 0;
	float factorX_Temp = 0.f;
	float factorY_Temp = 0.f;

	switch (msg[0]) {
		case '0':
			*mode = 0;
			switch (msg[1]) {
				case '0':
					*cmd = 0;
					*factorX = 0.f;
					*factorY = 0.f;
					break;
				case '1':
					*cmd = 1;
					*factorX = 0.f;
					*factorY = 0.f;
					break;
				case '2':
					*cmd = 2;
					*factorX = 0.f;
					*factorY = 0.f;
					break;
				default:
					break;
			}
			break;
		case '1':
			*mode = 1;
			switch (msg[1]) {				
				case '0':
					*cmd = 0;
					*factorX = 0.f;
					*factorY = 0.f;
					break;
				case '1':
					*cmd = 1;
					*factorX = 0.f;
					*factorY = 0.f;
					break;
				case '2':
					*cmd = 2;
					*factorX = 0.f;
					*factorY = 0.f;
					break;
				default:
					break;
			}
			break;
		case '2':
			*mode = 2;
			switch (msg[1]) {
				case '0':
					/*!< Illegal message */
					*cmd = 2;
					*factorX = 0.f;
					*factorY = 0.f;
					break;
				case '1':
					/*!< Illegal message */
					*cmd = 2;
					*factorX = 0.f;
					*factorY = 0.f;
					break;
				case '2':
					*cmd = 2;
					*factorX = 0.f;
					*factorY = 0.f;
					break;
				case '3':
					*cmd = 3;
					/* Extract factorX from msg[]. */
					if (msg[2] == '-')
					{
						/*!< factorX is negative. */
						factorX_Sign = -1;
						for (int i = 3; msg[i] != ','; ++i)
						{
							if (i == 3)
							{
								if (isNumber(msg[i]))
								{
									factorX_Temp = (int)msg[i] - 48;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == 4) {
								/*!< msg[i] is '.'. */
								if (msg[i] == '.')
								{
									continue;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == 5) {
								if (isNumber(msg[i]))
								{
									factorX_Temp = factorX_Temp + ((int)msg[i] - 48) * 0.1;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == 6) {
								if (isNumber(msg[i]))
								{
									factorX_Temp = factorX_Temp + ((int)msg[i] - 48) * 0.01;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else {
								/*!< Something's going wrong if the programme runs to here. */
								return -1;
							}
							factorX_EndIndex = i;
						}
						*factorX = factorX_Temp * factorX_Sign;
					} else {
						/*!< factorX is positive or 0. */
						factorX_Sign = 1;
						for (int i = 2; msg[i] != ','; ++i)
						{
							if (i == 2)
							{
								if (isNumber(msg[i]))
								{
									factorX_Temp = (int)msg[i] - 48;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == 3) {
								/*!< msg[i] is '.'. */
								if (msg[i] == '.')
								{
									continue;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == 4) {
								if (isNumber(msg[i]))
								{
									factorX_Temp = factorX_Temp + ((int)msg[i] - 48) * 0.1;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == 5) {
								if (isNumber(msg[i]))
								{
									factorX_Temp = factorX_Temp + ((int)msg[i] - 48) * 0.01;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else {
								/*!< Something's going wrong if the programme runs to here. */
								return -1;
							}
							factorX_EndIndex = i;
						}
						*factorX = factorX_Temp * factorX_Sign;
					}
					/* Extract factorY from msg[]. */
					factorY_StartIndex = factorX_EndIndex + 2;
					if (msg[factorY_StartIndex] == '-')
					{
						/*!< factorY is negative. */
						factorY_Sign = -1;
						for (int i = factorY_StartIndex + 1; msg[i] != '\n' && msg[i] != '\r'; ++i)
						{
							if (i == factorY_StartIndex + 1)
							{
								if (isNumber(msg[i]))
								{
									factorY_Temp = (int)msg[i] - 48;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == factorY_StartIndex + 2) {
								/*!< msg[i] is '.'. */
								if (msg[i] == '.')
								{
									continue;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == factorY_StartIndex + 3) {
								if (isNumber(msg[i]))
								{
									factorY_Temp = factorY_Temp + ((int)msg[i] - 48) * 0.1;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == factorY_StartIndex + 4) {
								if (isNumber(msg[i]))
								{
									factorY_Temp = factorY_Temp + ((int)msg[i] - 48) * 0.01;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else {
								/*!< Something's going wrong if the programme runs to here. */
								return -1;
							}
						}
						*factorY = factorY_Temp * factorY_Sign;
					} else {
						/*!< factorY is positive or 0. */
						factorY_Sign = 1;
						for (int i = factorY_StartIndex; msg[i] != '\n' && msg[i] != '\r'; ++i)
						{
							if (i == factorY_StartIndex)
							{
								if (isNumber(msg[i]))
								{
									factorY_Temp = (int)msg[i] - 48;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == factorY_StartIndex + 1) {
								/*!< msg[i] is '.'. */
								if (msg[i] == '.')
								{
									continue;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == factorY_StartIndex + 2) {
								if (isNumber(msg[i]))
								{
									factorY_Temp = factorY_Temp + ((int)msg[i] - 48) * 0.1;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else if (i == factorY_StartIndex + 3) {
								if (isNumber(msg[i]))
								{
									factorY_Temp = factorY_Temp + ((int)msg[i] - 48) * 0.01;
								} else {
									/*!< Illegal char. */
									return -1;
								}
							} else {
								/*!< Something's going wrong if the programme runs to here. */
								return -1;
							}
						}
						*factorY = factorY_Temp * factorY_Sign;
					}
					/*!< Reset variables. */
					factorX_Temp = 0.f;
					factorY_Temp = 0.f;
					factorX_EndIndex = 0;
					factorY_StartIndex = 0;
					factorX_Sign = 1;
					factorY_Sign = 1;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return 0;
}

/**
 * @brief Check if a char is a number.
 * 		e.g. '1' is a number, '!' isn't.
 * @param c: The char to check.
 * @retval int: 1 is a number, 0 isn't.
 */
int isNumber(char c)
{
	if ((-1 < ((int)c - 48)) && (((int)c - 48) < 10))
	{
		return 1;
	} else {
		return 0;
	}
}
