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
#include "state.h"
/* Private includes ----------------------------------------------------------*/
static unsigned int mpu9250_fread(FAR struct file *filp, FAR void * buffer, unsigned int buflen);
static struct file * mpu9250_fopen (FAR struct file *filp);
static int mpu9250_ioctrl(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data);
/* USER CODE BEGIN Includes */
FS_INODE_REGISTER("mpu9250.d",mpu,mpu9250_heap_init,0);
/* USER SPI HANDLER */
extern SPI_HandleTypeDef hspi3;
/* USER SPI HANDLER */
SPI_HandleTypeDef * mpu_spi_handle;
/* LPF */
#if USED_LPF
/* define */
static lpf2pData accLpf[3];
static lpf2pData gyroLpf[3];
/* end */
#endif
/* defined functions */
static int mpu9250_heap_init(void)
{
	/* init as default */
	mpu.flip.f_inode = &mpu;
	/* file interface  */
	mpu.ops.read  = mpu9250_fread;
	mpu.ops.open  = mpu9250_fopen;
	mpu.ops.ioctl = mpu9250_ioctrl;
	/* spi handle */
	mpu_spi_handle = &hspi3;
	/* return mpu9250 result */
	return mpu9250_init();
}
/* file & driver 's interface */
static struct file * mpu9250_fopen (FAR struct file *filp)
{
	/* return flip data */
	return &mpu.flip;
}
/* data read */
static unsigned int mpu9250_fread(FAR struct file *filp, FAR void * buffer, unsigned int buflen)
{
	/* ignore the complier warring */
	(void)filp;
	/* read */
	if( buflen != sizeof(MPU9250_INS_DEF) || buffer == NULL )
	{
		/* can not supply this format */
		return 0;
	}
	/* read data */
	mpu9250_read_sensor(buffer);
	/* return lens */
	return buflen;
}
/* mpu9250 ioctrl */
static int mpu9250_ioctrl(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data)
{
	/* nothing diffrent */
	int ret = FS_OK;
	/* temp */
	unsigned char * io_tmp;
	/* select a cmd */
	switch(cmd)
	{
		case 0:
			/* mpu9250_write_reg */
		  io_tmp = pri_data;
		  /* write */
		  mpu9250_write_reg(io_tmp[0],io_tmp[1]);
		  /* end of cmd */
			break;
		case 1:
			/* mpu9250_read_reg */
		  ret = mpu9250_read_reg((unsigned char)arg);
		  /* end of cmd */
			break;
		case 2:
			/* mpu9250_check_reg */
		  io_tmp = pri_data;
		  /* read and check */
			ret = mpu9250_check_reg(io_tmp[0],io_tmp[1]);
		  /* end of cmd */
			break;
		case 3:
			/* mpu9250_read_sensor */
		  if( arg != sizeof(MPU9250_INS_DEF) )
			{
				return FS_ERR;//can not supply this format
			}
			/* read */
			mpu9250_read_sensor(pri_data);
			/* end of cmd */
		case 4:
			/* mpu9250_init */
		  ret = mpu9250_init();
		  /* end of cmd */
		default:
			break;
	}
	/* return */
	return ret;
}
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
	ins->gyro[0] = (short)((buffer[8]   << 8) + buffer[9])  * GYRO_SENSITIVITY * DEG2RAD ; // transfer to rad
	ins->gyro[1] = (short)((buffer[10]  << 8) + buffer[11]) * GYRO_SENSITIVITY * DEG2RAD ; // transfer to rad
	ins->gyro[2] = (short)((buffer[12]  << 8) + buffer[13]) * GYRO_SENSITIVITY * DEG2RAD ; // transfer to rad
	/* get temperature */
	ins->mpu9250_temperature = (short)((buffer[6] << 8) + buffer[7]) * TEMPERATURE_SENSITIVITY + 25;
	/* ok */
#if USED_LPF
  for( int i = 0 ; i < 3 ; i ++ ) 
	{
		/* apply lpf2 */
    ins->accel[i] = lpf2pApply(&accLpf[i],ins->accel[i]);
	  ins->gyro[i]  = lpf2pApply(&gyroLpf[i] , ins->gyro[i]);
	}
#endif
}
/* disable mag sensor */
#if USED_MAG
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
	unsigned char ICM_REG[7] = {MPU_PWR_MGMT_1,MPU_USER_CTRL,MPU_SMPLRT_DIV,MPU_CONFIG,
	                            MPU_GYRO_CONFIG,MPU_ACCEL_CONFIG,MPU_ACCEL_CONFIG2};
	/* reg data that will write into */
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
#if USED_LPF	
	for ( int i = 0 ; i < 3 ; i++ )
	{
		lpf2pInit(&gyroLpf[i], 1000, GYRO_LPF_CUTOFF_FREQ);
		lpf2pInit(&accLpf[i],  1000, ACCEL_LPF_CUTOFF_FREQ);
	}
#endif	
	/* ok . great */
  return FS_OK;
}

/* USER CODE END */

































