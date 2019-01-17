/**
 ******************************************************************************
 * @file 		mode_config.c
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file is mainly used to set variables related to control 
 * 				in real time.
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

#include "mode_config.h"
#include "semantic_parser.h"

static int errorFlag = 0;

char rawMsg[20];
char finalMsg[MAX_MSG_LEN];
int tempMode = -1; 	/*!< Initialize to none of any mode. */
int finalMode = -1; 	/*!< Initialize to none of any mode. */
int tempCmd;
int finalCmd;
float tempFactorX;
float finalFactorX;
float tempFactorY;
float finalFactorY;

/**
 * @brief Fetches data from rawMsg[] to:
 * 		finalMode
 * 		finalCmd
 * 		finalFactorX
 * 		finalFactorY
 * @param None
 * @retval None
 */
void refreshData(void)
{
	int messageLength = 0;
	for (int i = 0; i < MAX_MSG_LEN; ++i)
	{
		if (rawMsg[i] != '\n') {
			continue;
		} else {
			messageLength = i + 1;
		}
	}
	if (messageLength > 14)
	{
		/* Illegal message length, do not refresh. */
		return;
	} else {
		/* Legal length, save message to finalMsg. */
		for (int i = 0; i < MAX_MSG_LEN; ++i)
		{
			finalMsg[i] = rawMsg[i];
		}
		errorFlag = semanticParse(finalMsg, &tempMode, &tempCmd, &tempFactorX, &tempFactorY);
		if (errorFlag != 0)
		{
			/* An error has occorred, do not refresh. */
			return;
		} else {
			finalMode = tempMode;
			finalCmd = tempCmd;
			finalFactorX = tempFactorX;
			finalFactorY = tempFactorY;
		}
	}
}
