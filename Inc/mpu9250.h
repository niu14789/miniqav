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

#ifndef __MPU5250_H__
#define __MPU5250_H__

#include "state.h"

#define MPU_SELF_TEST_X_GYRO	0x00
#define MPU_SELF_TEST_Y_GYRO	0x01
#define MPU_SELF_TEST_Z_GYRO	0x02
#define MPU_SELF_TEST_X_ACCEL	0x0D
#define MPU_SELF_TEST_Y_ACCEL	0x0E
#define MPU_SELF_TEST_Z_ACCEL	0x0F
#define MPU_XG_OFFS_USRH		0x13
#define MPU_XG_OFFS_USRL		0x14
#define MPU_YG_OFFS_USRH		0x15
#define MPU_YG_OFFS_USRL		0x16
#define MPU_ZG_OFFS_USRH		0x17
#define MPU_ZG_OFFS_USRL		0x18
#define MPU_SMPLRT_DIV			0x19
#define MPU_CONFIG				  0x1A
#define MPU_GYRO_CONFIG			0x1B
#define MPU_ACCEL_CONFIG		0x1C
#define MPU_ACCEL_CONFIG2		0x1D
#define MPU_LP_MODE_CFG			0x1E
#define MPU_ACCEL_WOM_THR		0x1F
#define MPU_FIFO_EN				  0x23
#define MPU_FSYNC_INT			  0x36
#define MPU_INT_PIN_CFG			0x37
#define MPU_INT_ENABLE			0x38
#define MPU_INT_STATUS			0x3A
#define MPU_ACCEL_XOUT_H		0x3B
#define MPU_ACCEL_XOUT_L		0x3C
#define MPU_ACCEL_YOUT_H		0x3D
#define MPU_ACCEL_YOUT_L		0x3E
#define MPU_ACCEL_ZOUT_H		0x3F
#define MPU_ACCEL_ZOUT_L		0x40
#define MPU_TEMP_OUT_H			0x41
#define MPU_TEMP_OUT_L			0x42
#define MPU_GYRO_XOUT_H			0x43
#define MPU_GYRO_XOUT_L			0x44
#define MPU_GYRO_YOUT_H			0x45
#define MPU_GYRO_YOUT_L			0x46
#define MPU_GYRO_ZOUT_H			0x47
#define MPU_GYRO_ZOUT_L			0x48
#define MPU_SIGNAL_PATH_RESET	0x68
#define MPU_ACCEL_INTEL_CTRL	0x69
#define MPU_USER_CTRL			  0x6A
#define MPU_PWR_MGMT_1			0x6B
#define MPU_PWR_MGMT_2			0x6C
#define MPU_FIFO_COUNTH			0x72
#define MPU_FIFO_COUNTL			0x73
#define MPU_FIFO_R_W			  0x74
#define MPU_WHO_AM_I			  0x75
#define MPU_XA_OFFSET_H			0x77
#define MPU_XA_OFFSET_L			0x78
#define MPU_YA_OFFSET_H			0x7A
#define MPU_YA_OFFSET_L			0x7B
#define MPU_ZA_OFFSET_H			0x7D
#define MPU_ZA_OFFSET_L			0x7E
/* mag */
#define MPU_MAG_XOUT_L		  0x03
#define MPU_MAG_XOUT_H		  0x04
#define MPU_MAG_YOUT_L		  0x05
#define MPU_MAG_YOUT_H		  0x06
#define MPU_MAG_ZOUT_L		  0x07
#define MPU_MAG_ZOUT_H		  0x08
/* Variables -----------------------------------------------------------------*/
enum Odr { // gyro Output Data Rate
  ODR_1kHz = 0,
  ODR_500Hz,	// default
  ODR_333Hz,
  ODR_250Hz,
  ODR_200Hz,
  ODR_167Hz,
  ODR_142Hz,
  ODR_125Hz,
  ODR_111Hz		//0x08
};

enum Ascale {
  AFS_2G = 0,
  AFS_4G,
  AFS_8G,
  AFS_16G
};

enum Abw { // accel bandwidth
  ABW_218_1 = 1,
  ABW_99_0,
  ABW_44_8,
  ABW_21_2,
  ABW_10_2,
  ABW_5_1,
  ABW_420 = 7 //0x07
};

enum Gscale {  // gyro full scale
  GFS_250DPS = 0,
  GFS_500DPS,
  GFS_1000DPS,		// default
  GFS_2000DPS         // 0x03
};

enum Gbw { // gyro bandwidth
  GBW_250Hz = 0,
  GBW_176Hz,
  GBW_92Hz,
  GBW_41Hz,
  GBW_20Hz,
  GBW_10Hz,
  GBW_5Hz,
  GBW_3281Hz	// 0x07
};
/*----------------------------*/
#define BIT_H_RESET					  0x80
#define BIT_RAW_RDY_EN				0x01
#define BIT_I2C_IF_DIS				0x10
#define BIT_RAW_RDY_EN				0x01
#define BIT_INT_ANYRD_2CLEAR	0x10

#define OUT_DATA_RATE				  ODR_1kHz
#define ACCEL_SCALE					  AFS_8G
#define ACCEL_BADNWIDTH				ABW_44_8
#define GYRO_SCALE					  GFS_1000DPS
#define GYRO_BADNWIDTH				GBW_92Hz
#define ENABLE_ADLPF				  0
#define ENABLE_GDLPF				  0
#define DISABLE_ADLPF				  1
#define DISABLE_GDLPF				  3
/* sensitivity */
#define GYRO_SENSITIVITY               ((float)( 2000.f / 65536.f ))
#define ACCEL_SENSITIVITY              ((float)( 16.f / 65536.f   ))
#define TEMPERATURE_SENSITIVITY        ((float)( 1 / 326.8f    ))
	
/* config */

#define USED_LPF              0  /* enable / disable LPF ( low pass filter ) */
#define USED_MAG              0  /* as default . disable the mag sensor */

/* some decleare */

static int mpu9250_init(void);
int mpu9250_heap_init(void);
static void mpu9250_read_sensor( MPU9250_INS_DEF * ins );
static unsigned char mpu9250_read_reg(unsigned char reg);
static void mpu9250_write_reg(unsigned char reg,unsigned char data);
static int mpu9250_check_reg(unsigned char reg,unsigned char data);
static int mpu9250_init(void);
/*---------------*/
#endif




















