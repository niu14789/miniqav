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
#include "fs.h"
#include "f_shell.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu9250.h"

const float gyro_sensitivity  = 1 / 32.8f / 180.0f * 3.1415926f;
const float accel_sensitivity = 1 / 4096.0f * 9.788f;

unsigned char test[5];

int ocm_heap_init(void);

FS_INODE_REGISTER("/ocm.d",ocm0,ocm_heap_init,0);
FS_INODE_REGISTER("/ocm.d",ocm1,ocm_heap_init,4);
FS_INODE_REGISTER("/ocm.d",ocm2,ocm_heap_init,3);
FS_INODE_REGISTER("/ocm.d",ocm3,ocm_heap_init,1);
FS_INODE_REGISTER("/ocm.d",ocm4,ocm_heap_init,2);

FS_SHELL_STATIC(test1,test,0x010001,_CB_RT_);
FS_SHELL_STATIC(test2,test,0x010001,_CB_RT_);
FS_SHELL_STATIC(test3,test,0x010001,_CB_RT_);
FS_SHELL_STATIC(test4,test,0x010001,_CB_RT_);
FS_SHELL_STATIC(test5,test,0x010001,_CB_RT_);

int ocm_heap_init(void)
{
	return FS_OK;
}

/* USER CODE END Includes */
SPI_HandleTypeDef * mpu_spi_handle;
/* USER CODE BEGIN */
/* lowlevel */
void mpu9250_write_reg(unsigned char reg,unsigned char data)
{
	unsigned char d[2] = { reg , data };
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_SPI_Transmit(mpu_spi_handle,d,2,0xffff);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}
/* get */
unsigned char mpu9250_read_reg(unsigned char reg)
{
	unsigned char d = reg | 0x80;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_SPI_Transmit(mpu_spi_handle,&d,1,0xffff);
	/* read */
	HAL_SPI_Receive(mpu_spi_handle,&d,1,0xffff);
	/* get */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);	
	/* return */
	return d;
}
unsigned char mpu_chip_id[3];

int mpu9250_Init( void * dev )
{
	mpu_spi_handle = dev;
	
	mpu9250_write_reg(PWR_MGMT_1, 0x00);
	
	mpu_chip_id[0] = mpu9250_read_reg(0);
	
  mpu9250_write_reg(PWR_MGMT_1, 0x00);	//解除休眠状态
	mpu9250_write_reg(CONFIG, 0x07);      //低通滤波频率，典型值：0x07(3600Hz)此寄存器内决定Internal_Sample_Rate==8K
	
/**********************Init SLV0 i2c**********************************/	
//Use SPI-bus read slave0
	mpu9250_write_reg(INT_PIN_CFG ,0x30);// INT Pin / Bypass Enable Configuration  
	mpu9250_write_reg(I2C_MST_CTRL,0x4d);//I2C MAster mode and Speed 400 kHz
	mpu9250_write_reg(USER_CTRL ,0x20); // I2C_MST _EN 
	mpu9250_write_reg(I2C_MST_DELAY_CTRL ,0x01);//延时使能I2C_SLV0 _DLY_ enable 	
	mpu9250_write_reg(I2C_SLV0_CTRL ,0x81); //enable IIC	and EXT_SENS_DATA==1 Byte
	
/*******************Init GYRO and ACCEL******************************/	
	mpu9250_write_reg(SMPLRT_DIV, 0x07);  //陀螺仪采样率，典型值：0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
	mpu9250_write_reg(GYRO_CONFIG, (2<<3) | 0); //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	mpu9250_write_reg(ACCEL_CONFIG, 2<<3);//加速计自检、测量范围及高通滤波频率，典型值：0x18(不自检，16G)
	mpu9250_write_reg(ACCEL_CONFIG_2, 2 | (0<<3));//加速计高通滤波频率 典型值 ：0x08  （1.13kHz）	
 
  mpu_chip_id[0] = mpu9250_read_reg(ACCEL_XOUT_L); 
  mpu_chip_id[1] = mpu9250_read_reg(ACCEL_XOUT_H);

	mpu_chip_id[2] = mpu9250_read_reg(0); 
	
	return 0;
}


float gyro[3];
float acce[3];



void READ_MPU9250_ACCEL(float * ax,float *ay,float *az)//
{ 
   unsigned char BUF[6];
	 short Accel[3];
   BUF[0]=mpu9250_read_reg(ACCEL_XOUT_L); 
   BUF[1]=mpu9250_read_reg(ACCEL_XOUT_H);
   Accel[0]=	(BUF[1]<<8)|BUF[0];
   acce[0] = Accel[0] * accel_sensitivity; 
   BUF[2]=mpu9250_read_reg(ACCEL_YOUT_L);
   BUF[3]=mpu9250_read_reg(ACCEL_YOUT_H);
   Accel[1]=	(BUF[3]<<8)|BUF[2];
   acce[1] = Accel[1] * accel_sensitivity;
   BUF[4]=mpu9250_read_reg(ACCEL_ZOUT_L); 
   BUF[5]=mpu9250_read_reg(ACCEL_ZOUT_H);
   Accel[2]=  (BUF[5]<<8)|BUF[4];
   acce[2] = Accel[2] * accel_sensitivity;
	 /*-------------------*/
	 *ax = acce[0];
	 *ay = acce[1];
	 *az = acce[2];
	 /*-------------------*/
}
/**********************陀螺仪读取*****************************/
void READ_MPU9250_GYRO(float * gx,float *gy,float *gz)
{ 
		unsigned char BUF[6];
		short Gyro[3];
		BUF[0]=mpu9250_read_reg(GYRO_XOUT_L); 
		BUF[1]=mpu9250_read_reg(GYRO_XOUT_H);
		Gyro[0]=	(BUF[1]<<8)|BUF[0];
		gyro[0] = Gyro[0] * gyro_sensitivity;
		BUF[2]=mpu9250_read_reg(GYRO_YOUT_L);
		BUF[3]=mpu9250_read_reg(GYRO_YOUT_H);
		Gyro[1]=	(BUF[3]<<8)|BUF[2];
		gyro[1] = Gyro[1] * gyro_sensitivity;
		BUF[4]=mpu9250_read_reg(GYRO_ZOUT_L);
		BUF[5]=mpu9250_read_reg(GYRO_ZOUT_H);
		Gyro[2]=	(BUF[5]<<8)|BUF[4];
		gyro[2] = Gyro[2] * gyro_sensitivity;
	  /*----------------*/
		*gx = gyro[0];
	  *gy = gyro[1];
	  *gz = gyro[2];
	  /*----------------*/
}

/* USER CODE END */



