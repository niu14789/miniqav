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
#include "dev.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/* USER CODE END Includes */
SPI_HandleTypeDef * spi_handle;
/* USER CODE BEGIN */
unsigned char pmw3901_read_reg(unsigned char addr)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	
	unsigned char d = (addr & 0x7F);
	
	HAL_SPI_Transmit(spi_handle,&d,1,0xffff);

	HAL_SPI_Receive(spi_handle,&d,1,0xffff);
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	
	return d;	
}
/* lowlevel */
unsigned char pmw3901_write_reg(unsigned char addr)
{
	return 0;
}

unsigned char chip_id[3];

int pwm3901_Init( void * dev )
{
	spi_handle = dev;
	
	chip_id[0] = pmw3901_read_reg(0);
	chip_id[1] = pmw3901_read_reg(1);
	chip_id[2] = pmw3901_read_reg(0x5F);
	
	return 0;
}
/* USER CODE END */

















