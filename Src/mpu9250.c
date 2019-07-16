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
#include "main.h"
#include "mpu9250.h"
#include "filter.h"
#include "imu.h"
/* Private includes ----------------------------------------------------------*/
void IMUupdate( MPU9250_INS_DEF * ins , ATTITUDE_DEF * att);
/* USER CODE BEGIN Includes */
FS_INODE_REGISTER("mpu9250.d",mpu,mpu9250_heap_init,0);

extern SPI_HandleTypeDef hspi3;
SPI_HandleTypeDef * mpu_spi_handle;
MPU9250_INS_DEF mpu9250_s;

ATTITUDE_DEF attitude;

static unsigned short cali_ctrl;

static unsigned int imu_freq;

static float bias[3];

static float detlt[3];
static lpf2pData accLpf[3];
static lpf2pData gyroLpf[3];

void mpu5290_task(void * p)
{
	
	unsigned int lastWakeTime;
	
	while(1)
	{
		vTaskDelayUntil(&lastWakeTime, 1);
		
		mpu9250_read_sensor( &mpu9250_s );
		
		if( cali_ctrl < 1000 )
		{
			bias[0] += mpu9250_s.gyro[0] / 1000.f;
			bias[1] += mpu9250_s.gyro[1] / 1000.f;
			bias[2] += mpu9250_s.gyro[2] / 1000.f;
			
			cali_ctrl++;
		}
		else
		{
		   mpu9250_s.gyro[0] -= bias[0];
			 mpu9250_s.gyro[1] -= bias[1];
			 mpu9250_s.gyro[2] -= bias[2];

//			 mpu9250_s.gyro[0] = lpf2pApply(&gyroLpf[0], mpu9250_s.gyro[0] );
//			 mpu9250_s.gyro[1] = lpf2pApply(&gyroLpf[1], mpu9250_s.gyro[1] );
//			 mpu9250_s.gyro[2] = lpf2pApply(&gyroLpf[2], mpu9250_s.gyro[2] );
			 
			 detlt[0] += mpu9250_s.gyro[0] / 1000;
			 detlt[1] += mpu9250_s.gyro[1] / 1000;
			 detlt[2] += mpu9250_s.gyro[2] / 1000;			
			
			 mpu9250_s.gyro[0] *= DEG2RAD;
			 mpu9250_s.gyro[1] *= DEG2RAD;
			 mpu9250_s.gyro[2] *= DEG2RAD;
			
			 if( ( imu_freq ++ ) % 2 )
			 {
			   IMUupdate(&mpu9250_s,&attitude);
			 }
		}
	}
}

int mpu9250_heap_init(void)
{
	mpu_spi_handle = &hspi3;
	mpu9250_init();
	
	xTaskCreate(mpu5290_task, "mpu5290_task", 150, NULL, 5, NULL);
	
	return FS_OK;
}
/* file & driver 's interface */
/* lowlevel */
static void mpu9250_delay(unsigned int t)
{
	while(t--);
}
/* mpu9250_write_reg */
static void mpu9250_write_reg(unsigned char reg,unsigned char data)
{
	unsigned char d[2] = { reg , data };
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_SPI_Transmit(mpu_spi_handle,d,2,0xffff);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}
/* mpu9250_read_reg */
static unsigned char mpu9250_read_reg(unsigned char reg)
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
/* write and check regs */
static int mpu9250_check_reg(unsigned char reg,unsigned char data)
{
	/* default */
	unsigned char tmp_reg;
	/* default try times */
	unsigned char timeout = 0x10;
	/* write and check */
	do
	{
		/* write */
		mpu9250_write_reg( reg , data );
		/* delay for a while */
    mpu9250_delay(0xffff);
		/* read */
		tmp_reg = mpu9250_read_reg(reg);
		/* delay for a while */
		mpu9250_delay(0xffff);
	}
	while( tmp_reg != data && timeout-- );//check
  /* check ok or not */
	if( timeout == 0xFF )
	{
		return FS_ERR; // timeout , oh no.we were failed
	}
  /* ok . great */
	return FS_OK;
}
/* spi read data */
static void mpu9250_read_sensor( MPU9250_INS_DEF * ins )
{
	/* write reg first */
  unsigned char d = MPU_ACCEL_XOUT_H | 0x80;
	unsigned char buffer[14];
	/* chipselect */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	/* write reg */
	HAL_SPI_Transmit(mpu_spi_handle,&d,1,0xffff);
	/* read */
	HAL_SPI_Receive(mpu_spi_handle,buffer,sizeof(buffer),0xffff);
	/* get */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);	
  /* change */
  ins->accel[0] = (short)((buffer[0] << 8) + buffer[1]) * ACCEL_SENSITIVITY;	
	ins->accel[1] = (short)((buffer[2] << 8) + buffer[3]) * ACCEL_SENSITIVITY;
	ins->accel[2] = (short)((buffer[4] << 8) + buffer[5]) * ACCEL_SENSITIVITY;
	/* get gyro */
	ins->gyro[0] = (short)((buffer[8]   << 8) + buffer[9])  * GYRO_SENSITIVITY;
	ins->gyro[1] = (short)((buffer[10]  << 8) + buffer[11]) * GYRO_SENSITIVITY;
	ins->gyro[2] = (short)((buffer[12]  << 8) + buffer[13]) * GYRO_SENSITIVITY;
	/* get temperature */
	ins->mpu9250_temperature = (short)((buffer[6] << 8) + buffer[7]) * TEMPERATURE_SENSITIVITY + 25;
	/* ok */
}
#if 0
/* mpu 9250 get mag */
static void mpu9250_read_mag( MPU9250_MAG_DEF * mag )
{
/* write reg first */
  unsigned char d = MPU_MAG_XOUT_L | 0x80;
	unsigned char buffer[6];
	/* chipselect */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	/* write reg */
	HAL_SPI_Transmit(mpu_spi_handle,&d,1,0xffff);
	/* read */
	HAL_SPI_Receive(mpu_spi_handle,buffer,sizeof(buffer),0xffff);
	/* get */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);	
  /* change */	
	mag->mag[0] = (short)((buffer[0] << 8) + buffer[1]);	
	mag->mag[1] = (short)((buffer[2] << 8) + buffer[3]);	
	mag->mag[2] = (short)((buffer[4] << 8) + buffer[5]);	
	/* return */
}
#endif
/* mpu9250 init */
static int mpu9250_init(void)
{
	/* some default */
	unsigned char ctrl0 = 0x80;
	unsigned char ctrl1 = BIT_I2C_IF_DIS;
	unsigned char ctrl2 = OUT_DATA_RATE;
	unsigned char ctrl3 = GYRO_BADNWIDTH;
	unsigned char ctrl4 = (GYRO_SCALE<<3) | ENABLE_GDLPF;
	unsigned char ctrl5 = ACCEL_SCALE<<3;
	unsigned char ctrl6 = ACCEL_BADNWIDTH | (ENABLE_ADLPF<<3);
  /* decode */
	unsigned char ICM_REG[7] = {MPU_PWR_MGMT_1,MPU_USER_CTRL,MPU_SMPLRT_DIV,MPU_CONFIG,MPU_GYRO_CONFIG,MPU_ACCEL_CONFIG,MPU_ACCEL_CONFIG2};
	unsigned char CTRL_RE[7] = {0x01,ctrl1,ctrl2,ctrl3,ctrl4,ctrl5,ctrl6};
	/* reset first */
	mpu9250_write_reg(MPU_PWR_MGMT_1, ctrl0);
  /* delay for some time */
  mpu9250_delay(0xffff0);
  /* for loop to config */
	for( int i = 0 ; i < 7 ; i++ )
	{
		/* start check */
		if( mpu9250_check_reg(ICM_REG[i],CTRL_RE[i]) != 0 )
		{
       return FS_ERR; //failed
		}
		/* delay for some time */
		mpu9250_delay(0xffff);
	}
	/* init  */
	for ( int i = 0 ; i < 3 ; i++ )// 初始化加速计和陀螺二阶低通滤波
	{
		lpf2pInit(&gyroLpf[i], 1000, GYRO_LPF_CUTOFF_FREQ);
		lpf2pInit(&accLpf[i],  1000, ACCEL_LPF_CUTOFF_FREQ);
	}
	/* ok . great */
  return FS_OK;
}

/* USER CODE END */

































