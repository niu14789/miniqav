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

#define VL53L0X_ADDR 				0x52

unsigned short vl_read_16bit(unsigned short reg);

I2C_HandleTypeDef *iic_dev;
/* USER CODE BEGIN Includes */
int vl53lxx_Init( void * dev )
{
	iic_dev = dev;
  /* read ID */
	unsigned short vl_chipID = vl_read_16bit(0xC0);
  /*---------*/
	return 0;
}
unsigned short vl_read_16bit(unsigned short reg)
{
	unsigned char buffer[2];
	
	buffer[0] = reg;
	buffer[1] = VL53L0X_ADDR | 0x1;
	/* send */
  HAL_I2C_Master_Transmit(iic_dev,VL53L0X_ADDR,buffer,2,0xffff);
	/* read */
	HAL_I2C_Master_Receive(iic_dev,VL53L0X_ADDR,buffer,2,0xffff);
	/*---*/
	return buffer[0] << 8 | buffer[1];
}


