/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : notify.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
	* BEEP TIM3 CHANNEL1 PWM Gerente
	* LED is TIM4 CH3 and CH4
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "fs.h"
#include "f_shell.h"
#include "state.h"
#include "main.h"
#include "cmsis_os.h"
#include "f_ops.h"
#include "string.h"
/************************************************************************
*                                                                       *
*               Generated C-File                                        *
*                                                                       *
*              (c) 1995 - 2018 SEGGER Microcontroller GmbH              *
*                                                                       *
*************************************************************************

Please note:

SEGGER Microcontroller has provided the feature of being able to save a file
in ANSI "C" format as convenience feature.
You may use this feature at your own risc, SEGGER assumes no liability
in any form.

Memory image: 0x8000000 - 0x8001037
*/

const unsigned char _act[0x1038] = {
  0x24, 0x4D, 0x3E, 0x03, 0x01, 0x00, 0x01, 0x0E, 0x0D, 0x24, 0x4D, 0x3E, 0x04, 0x02, 0x43, 0x4C,
  0x46, 0x4C, 0x03, 0x24, 0x4D, 0x3E, 0x03, 0x03, 0x01, 0x0B, 0x00, 0x0A, 0x24, 0x4D, 0x3E, 0x1A,
  0x05, 0x4E, 0x6F, 0x76, 0x20, 0x31, 0x39, 0x20, 0x32, 0x30, 0x31, 0x36, 0x30, 0x36, 0x3A, 0x35,
  0x35, 0x3A, 0x32, 0x32, 0x00, 0x0D, 0x0A, 0x45, 0x6E, 0x74, 0x65, 0x7E, 0x24, 0x4D, 0x3E, 0x06,
  0x04, 0x41, 0x46, 0x4E, 0x41, 0x02, 0x00, 0x08, 0x24, 0x4D, 0x3E, 0x0C, 0xA0, 0x56, 0xFF, 0x6C,
  0x06, 0x70, 0x86, 0x56, 0x51, 0x43, 0x22, 0x25, 0x67, 0xBD, 0x24, 0x4D, 0x3E, 0x04, 0xF0, 0x00,
  0x00, 0x00, 0x00, 0xF4, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0x9F, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04,
  0x00, 0x00, 0x00, 0x00, 0xFB, 0x24, 0x4D, 0x3E, 0x07, 0x64, 0xE7, 0x03, 0x00, 0x04, 0x00, 0x00,
  0x00, 0x83, 0x24, 0x4D, 0x3E, 0x10, 0x42, 0x03, 0x0A, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x5A, 0x00, 0x90, 0x01, 0x00, 0x00, 0x91, 0x24, 0x4D, 0x3E, 0x16, 0x72, 0xDC, 0x05, 0x7E,
  0x04, 0x3A, 0x07, 0xE8, 0x03, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x6E,
  0x21, 0x2B, 0x23, 0xBA, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xA1, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
  0xC5, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06,
  0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x0A, 0x00, 0x67,
  0x01, 0x00, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x0B, 0x65, 0xB6, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xD2, 0x24,
  0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09,
  0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09,
  0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B,
  0x65, 0xAE, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x24, 0x4D, 0x3E,
  0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06,
  0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06,
  0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x0B, 0x65, 0xAB, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCF, 0x24,
  0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09,
  0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09,
  0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B,
  0x65, 0xAC, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x24, 0x4D, 0x3E,
  0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06,
  0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06,
  0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x0B, 0x65, 0xA6, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xC2, 0x24,
  0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09,
  0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09,
  0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B,
  0x65, 0xB0, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xD4, 0x24, 0x4D, 0x3E,
  0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06,
  0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05,
  0x00, 0x0A, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xA9,
  0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCD, 0x24, 0x4D, 0x3E, 0x07, 0x6E,
  0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09,
  0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x67, 0x01, 0x00,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A,
  0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x09, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B,
  0x65, 0xC1, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xA5, 0x24, 0x4D, 0x3E,
  0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06,
  0x00, 0x0A, 0x00, 0x67, 0x01, 0x00, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05,
  0x00, 0x0A, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x05, 0x00, 0x0A, 0x00, 0x67, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xA5,
  0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xC1, 0x24, 0x4D, 0x3E, 0x07, 0x6E,
  0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x07, 0x00, 0x06,
  0x00, 0x67, 0x01, 0x0D, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x07, 0x00, 0xE8, 0xFF, 0x00, 0x00, 0x7A,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x07, 0x00, 0xC2, 0xFF, 0x00, 0x00, 0x50, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x0E, 0x00, 0xA7, 0xFF, 0x00, 0x00, 0x3C, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x1A, 0x00, 0x98,
  0xFF, 0x00, 0x00, 0x17, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x27, 0x00, 0x8E, 0xFF, 0x00, 0x00, 0x3C,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x33, 0x00, 0x85, 0xFF, 0x00, 0x00, 0x23, 0x24, 0x4D, 0x3E, 0x0B,
  0x65, 0xAF, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCB, 0x24, 0x4D, 0x3E,
  0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x39,
  0x00, 0x82, 0xFF, 0x67, 0x01, 0x48, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x34, 0x00, 0x86, 0xFF, 0x67,
  0x01, 0x41, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x20, 0x00, 0x90, 0xFF, 0x67, 0x01, 0x43, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x0A, 0x00, 0x9D, 0xFF, 0x67, 0x01, 0x64, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x0A,
  0x00, 0xC8, 0xFF, 0x66, 0x01, 0x30, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x09, 0x00, 0xEB, 0xFF, 0x66,
  0x01, 0x10, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0x08, 0x00, 0x66, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x66, 0x01, 0x00, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xB1,
  0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xD5, 0x24, 0x4D, 0x3E, 0x07, 0x6E,
  0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x03, 0x00, 0x0B,
  0x00, 0x66, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x08, 0x00, 0x67, 0x01, 0x01,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x12, 0x00, 0x00, 0x00, 0x67, 0x01, 0x1E, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x21, 0x00, 0xE5, 0xFF, 0x66, 0x01, 0x36, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x23, 0x00, 0xB7,
  0xFF, 0x67, 0x01, 0x67, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x27, 0x00, 0x87, 0xFF, 0x66, 0x01, 0x52,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x32, 0x00, 0x72, 0xFF, 0x66, 0x01, 0xB2, 0x24, 0x4D, 0x3E, 0x0B,
  0x65, 0xA6, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xC2, 0x24, 0x4D, 0x3E,
  0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x39,
  0x00, 0x6E, 0xFF, 0x67, 0x01, 0xA4, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x36, 0x00, 0x72, 0xFF, 0x67,
  0x01, 0xB7, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x2A, 0x00, 0x7F, 0xFF, 0x66, 0x01, 0xA7, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x1C, 0x00, 0x92, 0xFF, 0x66, 0x01, 0x7C, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x10,
  0x00, 0xAC, 0xFF, 0x65, 0x01, 0x4D, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x08, 0x00, 0xD1, 0xFF, 0x65,
  0x01, 0x28, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x09, 0x00, 0xF7, 0xFF, 0x65, 0x01, 0x0F, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x05, 0x00, 0x08, 0x00, 0x65, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xBC,
  0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x24, 0x4D, 0x3E, 0x07, 0x6E,
  0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x09, 0x00, 0x06,
  0x00, 0x64, 0x01, 0x00, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x13, 0x00, 0xFE, 0xFF, 0x64, 0x01, 0x1D,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x1C, 0x00, 0xF0, 0xFF, 0x64, 0x01, 0x1C, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x22, 0x00, 0xD1, 0xFF, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x23, 0x00, 0xAC,
  0xFF, 0x64, 0x01, 0x7F, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x23, 0x00, 0x83, 0xFF, 0x64, 0x01, 0x50,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x24, 0x00, 0x65, 0xFF, 0x65, 0x01, 0xB0, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x27, 0x00, 0x5C, 0xFF, 0x65, 0x01, 0x8A, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xB4, 0x0D, 0x00,
  0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00,
  0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x26, 0x00, 0x61, 0xFF, 0x65,
  0x01, 0xB6, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x1E, 0x00, 0x6D, 0xFF, 0x64, 0x01, 0x83, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x15, 0x00, 0x86, 0xFF, 0x64, 0x01, 0x63, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x15,
  0x00, 0xA6, 0xFF, 0x63, 0x01, 0x44, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x25, 0x00, 0xD5, 0xFF, 0x63,
  0x01, 0x07, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x42, 0x00, 0x01, 0x00, 0x62, 0x01, 0x4A, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x47, 0x00, 0x08, 0x00, 0x62, 0x01, 0x46, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xAB,
  0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCF, 0x24, 0x4D, 0x3E, 0x07, 0x6E,
  0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x4E, 0x00, 0x0A,
  0x00, 0x62, 0x01, 0x4D, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x59, 0x00, 0x0A, 0x00, 0x62, 0x01, 0x5A,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x63, 0x00, 0x09, 0x00, 0x62, 0x01, 0x63, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x57, 0x00, 0xF9, 0xFF, 0x63, 0x01, 0x59, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x1B, 0x00, 0xBA,
  0xFF, 0x65, 0x01, 0x50, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0xA3, 0xFF, 0x66, 0x01, 0x54,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0xA5, 0xFF, 0x66, 0x01, 0x51, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x06, 0x00, 0xA7, 0xFF, 0x66, 0x01, 0x53, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xAD, 0x0D, 0x00,
  0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00,
  0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x06, 0x00, 0xAC, 0xFF, 0x66,
  0x01, 0x58, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x07, 0x00, 0xB4, 0xFF, 0x66, 0x01, 0x41, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x17, 0x00, 0xCA, 0xFF, 0x66, 0x01, 0x2F, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x3D,
  0x00, 0xF4, 0xFF, 0x65, 0x01, 0x38, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x52, 0x00, 0x09, 0x00, 0x64,
  0x01, 0x54, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x4C, 0x00, 0x02, 0x00, 0x64, 0x01, 0x41, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x4E, 0x00, 0x02, 0x00, 0x64, 0x01, 0x43, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xB1,
  0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xD5, 0x24, 0x4D, 0x3E, 0x07, 0x6E,
  0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x51, 0x00, 0x02,
  0x00, 0x64, 0x01, 0x5C, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x48, 0x00, 0xF2, 0xFF, 0x65, 0x01, 0x4B,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x2C, 0x00, 0xCC, 0xFF, 0x66, 0x01, 0x12, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x06, 0x00, 0x9C, 0xFF, 0x66, 0x01, 0x68, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x09, 0x00, 0x9C,
  0xFF, 0x66, 0x01, 0x67, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x07, 0x00, 0x98, 0xFF, 0x66, 0x01, 0x6D,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x09, 0x00, 0x9F, 0xFF, 0x66, 0x01, 0x64, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x18, 0x00, 0xB5, 0xFF, 0x65, 0x01, 0x5C, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xAE, 0x0D, 0x00,
  0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00,
  0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x40, 0x00, 0xE4, 0xFF, 0x64,
  0x01, 0x54, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x5F, 0x00, 0x07, 0x00, 0x63, 0x01, 0x50, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x5D, 0x00, 0x06, 0x00, 0x63, 0x01, 0x53, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x5B,
  0x00, 0x02, 0x00, 0x63, 0x01, 0x51, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x4F, 0x00, 0xF3, 0xFF, 0x63,
  0x01, 0x4B, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x20, 0x00, 0xBE, 0xFF, 0x64, 0x01, 0x6E, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x08, 0x00, 0xA5, 0xFF, 0x64, 0x01, 0x5D, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xA5,
  0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xC1, 0x24, 0x4D, 0x3E, 0x07, 0x6E,
  0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x07, 0x00, 0xA5,
  0xFF, 0x64, 0x01, 0x52, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x07, 0x00, 0xA6, 0xFF, 0x64, 0x01, 0x51,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x14, 0x00, 0xBB, 0xFF, 0x64, 0x01, 0x5F, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x3C, 0x00, 0xE9, 0xFF, 0x63, 0x01, 0x22, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x5A, 0x00, 0x0A,
  0x00, 0x62, 0x01, 0x59, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x52, 0x00, 0x06, 0x00, 0x62, 0x01, 0x5D,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x52, 0x00, 0x08, 0x00, 0x62, 0x01, 0x53, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x48, 0x00, 0xFF, 0xFF, 0x62, 0x01, 0x41, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xA4, 0x0D, 0x00,
  0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00,
  0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x24, 0x00, 0xDA, 0xFF, 0x63,
  0x01, 0x09, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0xBE, 0xFF, 0x64, 0x01, 0x4B, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x09, 0x00, 0xCB, 0xFF, 0x64, 0x01, 0x32, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05,
  0x00, 0xD4, 0xFF, 0x64, 0x01, 0x21, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0xEF, 0xFF, 0x64,
  0x01, 0x1B, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x03, 0x00, 0x09, 0x00, 0x64, 0x01, 0x05, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x03, 0x00, 0x09, 0x00, 0x63, 0x01, 0x02, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x01,
  0x00, 0x0B, 0x00, 0x63, 0x01, 0x02, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xAF, 0x0D, 0x00, 0x00, 0x03,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCB, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF,
  0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x03, 0x00, 0x09, 0x00, 0x63, 0x01, 0x02,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x03, 0x00, 0x09, 0x00, 0x63, 0x01, 0x02, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x03, 0x00, 0x09, 0x00, 0x63, 0x01, 0x02, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09,
  0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xB8, 0x0D, 0x00,
  0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00,
  0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63,
  0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04,
  0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63,
  0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04,
  0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xAC, 0x0D, 0x00, 0x00, 0x03,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF,
  0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09,
  0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xBA, 0x0D, 0x00,
  0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xDE, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00,
  0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63,
  0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04,
  0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63,
  0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04,
  0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xC1, 0x0D, 0x00, 0x00, 0x03,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xA5, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF,
  0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09,
  0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xAC, 0x0D, 0x00,
  0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00,
  0x00, 0xFF, 0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63,
  0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04,
  0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63,
  0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x04, 0x00, 0x09, 0x00, 0x63, 0x01, 0x05, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05,
  0x00, 0x09, 0x00, 0x63, 0x01, 0x04, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xA9, 0x0D, 0x00, 0x00, 0x03,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCD, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF,
  0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09,
  0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09,
  0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0x9E, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04,
  0x00, 0x00, 0x00, 0x00, 0xFA, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00,
  0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05,
  0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05,
  0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xAE, 0x0D, 0x00, 0x00, 0x03,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF,
  0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09,
  0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09,
  0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xA3, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04,
  0x00, 0x00, 0x00, 0x00, 0xC7, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00,
  0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05,
  0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05,
  0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xB0, 0x0D, 0x00, 0x00, 0x03,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xD4, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF,
  0x01, 0x00, 0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09,
  0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09,
  0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xA7, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04,
  0x00, 0x00, 0x00, 0x00, 0xC3, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00,
  0x00, 0x24, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D,
  0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05,
  0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64,
  0x01, 0x03, 0x24, 0x4D, 0x3E, 0x0B, 0x65, 0xC5, 0x0D, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00,
  0x00, 0x00, 0xA1, 0x24, 0x4D, 0x3E, 0x07, 0x6E, 0xB3, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x24,
  0x24, 0x4D, 0x3E, 0x06, 0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03, 0x24, 0x4D, 0x3E, 0x06,
  0x6C, 0x05, 0x00, 0x09, 0x00, 0x64, 0x01, 0x03
};

/*** end of file ***/

/* some marcos */
#define GS_HEAD1 (0x24)
#define GS_HEAD2 (0x4D)
#define GS_HEAD3 (0x3E)
/* functions declerae */
static int gs_heap_init(void);
static int gs_config(void);
static void gs_send_euqler( const state_def * state );
static void gs_thread( void * p );
static void gs_set_up(void);
static void gs_send_battery( const state_def * state );
static void gs_send_unknow(const state_def * state );
/* uart default */
extern UART_HandleTypeDef huart1;;
/* USER CODE BEGIN Includes */
FS_INODE_REGISTER("gs.a",gs,gs_heap_init,0);
/* default some data */
static state_def * state_p;
/* gs heap init */
static int gs_heap_init(void)
{
  /* init as default */
	gs.flip.f_inode = &gs;
	/* config */
	gs.config = gs_config;
	/* file interface  */
	return FS_OK;
}
/* default config for creating a task */
static int gs_config(void)
{
	/* default some data */
	struct shell_cmd * sp;
	/* open mpu */
	if( ( sp = shell_find("state")) == 0 )
	{
		/* can not find the state dev */
		return FS_ERR;
	}
	/* copy data */
	state_p = sp->enter;
	/* create a task run as 1ms per second */
	xTaskCreate(gs_thread, "gs_thread", 128 /* stack size */, 0 , 4 /* priority */ , 0 );
	/* return ok */
	return FS_OK;
}
/* ground station task */
static void gs_thread( void * p )
{
	/* Ignore compiler warnings */
	(void)p;
	/* init something */
	unsigned int lasttime;
	unsigned int freq_ctrl = 0;
	/* set up linker */
	gs_set_up();
	/* loop */
	while(1)
	{
		 vTaskDelayUntil(&lasttime, 10 /* 1ms */ );
		 /* send euqler angle */
		 gs_send_euqler(state_p);
		 /* send rc */
//		 if( ! ( freq_ctrl ++ % 8 ) )
//		 {
//			   /* send battery msg */
//		    gs_send_battery(state_p);
//			  /* send unknow msg */
//			  gs_send_unknow(state_p);
//		 }
		 
		 //HAL_UART_Transmit(&huart1,(unsigned char *)&_act[freq_ctrl],1,1);
//		
//		//USART1->DR = _act[freq_ctrl];
//		
//		if( freq_ctrl < sizeof(_act) )
//		{
//			freq_ctrl ++;
//		}
	}
}
/* create send data */
static int gs_create(unsigned char * buf , unsigned int buflen , unsigned int plen , unsigned int pid , const void * payout )
{
	/* jugde */
	if( buflen < plen + 6 )
	{
		return 0;//can not supply this format
	}	
	/* create */
	buf[0] = GS_HEAD1;
	buf[1] = GS_HEAD2;
	buf[2] = GS_HEAD3;
	/* LEN */
	buf[3] = plen;
	/* pid */
	buf[4] = pid;
	/* payout */
	memcpy( buf + 5 , payout , plen );
  /* check xor*/
  unsigned char oxr = 0;
  /* check */
  for( int i = 0 ; i < plen + 2 ; i ++ )
	{
		oxr ^= buf[ 3 + i ];
	}		
	/* copy */
	buf[ plen + 5 ] =  oxr;
	/* return */
	return plen + 6;
}	
/* create euqler buffer */
static void gs_send_euqler( const state_def * state )
{
	/* equler data buffer */
	short equler[3];
	unsigned char buffer[16];
	/* create buffer */
	equler[0] = (short)(state->att.roll  * 10 );
	equler[1] = (short)(state->att.pitch * 10 );
	equler[2] = (short)(state->att.yaw   * 10 );
	/* create buffer */
	int len = gs_create(buffer,sizeof(buffer),sizeof(equler),0x6C,equler);
	/* send */
	HAL_UART_Transmit(&huart1,buffer,len,0xffffffff);
}
/* create battery buffer */
static void gs_send_battery( const state_def * state )
{
	/* const battery data */
	const unsigned char bat[] = {0xB3,0x00,0x00,0xFF,0x01,0x00,0x00};
	/* buffer */
	unsigned char buffer[16];
	/* create buffer and send */
	int len = gs_create(buffer,sizeof(buffer),sizeof(bat),0x6E,bat);
	/* send */
	HAL_UART_Transmit(&huart1,buffer,len,0xffffffff);	
}
/* create 0x0b 0x65 */
static void gs_send_unknow(const state_def * state )
{
	/* const battery data */
	static unsigned char unknow[] = {0xAE,0x0D,0x00,0x00,0x03,0x00,0x04,0x00,0x00,0x00,0x00};
	/* buffer */
	unsigned char buffer[32];
	
	unknow[0] += 16;
	
	/* create buffer and send */
	int len = gs_create(buffer,sizeof(buffer),sizeof(unknow),0x65,unknow);
	/* send */
	HAL_UART_Transmit(&huart1,buffer,len,0xffffffff);	
}
/* create set up buf */
static void gs_set_up(void)
{
	unsigned char buffer[64]; 
	int len = 0;
	/* len , id , data */
	const unsigned char set_up_tab[14][32] = 
  {
		{0x03 ,0x01 ,0x00,0x01,0x0E},
		{0x04 ,0x02 ,0x43,0x4C,0x46,0x4C},
		{0x03 ,0x03 ,0x01,0x0B,0x00},
		{0x1A ,0x05 ,0x4E,0x6F,0x76,0x20,0x31,0x39,0x20,0x32,0x30,0x31,0x36,0x30,0x36,0x3A,0x35,0x35,0x3A,0x32,0x32,0x00,0x0D,0x0A,0x45,0x6E,0x74,0x65},
		{0x06 ,0x04 ,0x41,0x46,0x4E,0x41,0x02,0x00},
		{0x0C ,0xA0 ,0x56,0xFF,0x6C,0x06,0x70,0x86,0x56,0x51,0x43,0x22,0x25,0x67},
		{0x04 ,0xF0 ,0x00,0x00,0x00,0x00},
		{0x0B ,0x65 ,0xAE,0x0D,0x00,0x00,0x03,0x00,0x04,0x00,0x00,0x00,0x00},
		{0x07 ,0x64 ,0xE7,0x03,0x00,0x04,0x00,0x00,0x00},
		{0x10 ,0x42 ,0x03,0x0A,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5A,0x00,0x90,0x01,0x00,0x00},
		{0x16 ,0x72 ,0xDC,0x05,0x7E,0x04,0x3A,0x07,0xE8,0x03,0xE8,0x03,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x6E,0x21,0x2B,0x23},
		{0x06 ,0x6C ,0x03,0x00,0x0A,0x00,0x05,0x00},
		{0x07 ,0x6E ,0xB3,0x00,0x00,0xFF,0x01,0x00,0x00}
  };
	/* create buffer */
  for( int i = 0 ; i < 14 ; i ++ )
	{
		len = gs_create(buffer,sizeof(buffer),set_up_tab[i][0],set_up_tab[i][1],&set_up_tab[i][2]);
		/* send */
		HAL_UART_Transmit(&huart1,buffer,len,0xffffffff);		
	}
}
































