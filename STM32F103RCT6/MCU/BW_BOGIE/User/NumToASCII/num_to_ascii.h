/**
 ******************************************************************************
 * @file 		num_to_ascii.h
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 25, 2018
 * @brief 		This file provides all functions for converting numbers to 
 * 				chars.
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

#ifndef NUM_TO_ASCII_H
#define NUM_TO_ASCII_H

/* ------------------------ Configuration ------------------------------------*/
/** Choose your platforms(e.g. GCC, Arduino, STM32...),
 * In STM32, uint32_t was already supported in official libs.
 * Uncomment this to enable your platform, all commented by default.
 */
// #define GCC_PLATFORMS
#define STM32_PLATFORMS

#ifdef GCC_PLATFORMS
#include <stdio.h>
typedef __uint32_t uint32_t;
#endif

#ifdef STM32_PLATFORMS
#include "stm32f10x.h"
#endif


#define ASCII_OFFSET	48

void numberToASCIIBin(uint32_t number, char * str, int sizeof_str);
void numberToASCIIDec(uint32_t number, char * str, int sizeof_str);
// void numberToASCIIHex(uint32_t number, char * str, int sizeof_str);

#endif
