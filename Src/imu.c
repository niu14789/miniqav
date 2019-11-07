/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : imu.c
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
#include <math.h>
#include "fs.h"
#include "mpu9250.h"
#include "state.h"
/* some decleres  */
static int IMUupdate( state_def * state );
static int imu_heap_init(void);
static struct file * imu_fopen (FAR struct file *filp);
static unsigned int imu_fread(FAR struct file *filp, FAR void * buffer, unsigned int buflen);
static int imu_ioctrl(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data);
/* USER CODE BEGIN Includes */
FS_INODE_REGISTER("imu.a",imu,imu_heap_init,0);
/* defined functions */
static int imu_heap_init(void)
{
	/* init as default */
	imu.flip.f_inode = &imu;
	/* file interface  */
	imu.ops.read  = imu_fread;
	imu.ops.open  = imu_fopen;
	imu.ops.ioctl = imu_ioctrl;
  /* heap init . add your own code here */
	
	/* reutnr OK */
	return FS_OK;
}
/* file & driver 's interface */
static struct file * imu_fopen (FAR struct file *filp)
{
	/* return flip data */
	return &imu.flip;
}
/* data read */
static unsigned int imu_fread(FAR struct file *filp, FAR void * buffer, unsigned int buflen)
{
	/* read length */
	if( buflen != sizeof(state_def) )
	{
		return 0; // can not supply this format 
	}
	/* calibrate attitude */
	IMUupdate(buffer);
	/*-------------*/
	return buflen;
}
/* mpu9250 ioctrl */
static int imu_ioctrl(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data)
{
	/* nothing diffrent */
	int ret = FS_OK;
	/* select a cmd */
	switch(cmd)
	{
		case 0:
		  /* read length */
			if( arg != sizeof(state_def) )
			{
				return FS_ERR; // can not supply this format 
			}
			/* calibrate attitude */
			IMUupdate(pri_data);		
      /* end of func */			
			break;
		case 1:
			break;
		default:
			break;
	}
	/* return ret */
	return ret;
}
/* data fusion */
/* some decleres */
static float Kp = 0.4f;
static float Ki = 0.001f;
static float exInt = 0.0f;
static float eyInt = 0.0f;
static float ezInt = 0.0f;
/* Q */
static float q0 = 1.0f;
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;	
static float rMat[3][3];
/* calutate rotation matrix */
static void imuComputeRotationMatrix(void)
{
	float q1q1 = q1 * q1;
	float q2q2 = q2 * q2;
	float q3q3 = q3 * q3;

	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q0q3 = q0 * q3;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q2q3 = q2 * q3;

	rMat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
	rMat[0][1] = 2.0f * (q1q2 + -q0q3);
	rMat[0][2] = 2.0f * (q1q3 - -q0q2);

	rMat[1][0] = 2.0f * (q1q2 - -q0q3);
	rMat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
	rMat[1][2] = 2.0f * (q2q3 + -q0q1);

	rMat[2][0] = 2.0f * (q1q3 + -q0q2);
	rMat[2][1] = 2.0f * (q2q3 - -q0q1);
	rMat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;
}
/* step1 */
static float invSqrt(float x)	/*快速开平方求倒*/
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
/* data fusion start */
static int IMUupdate( state_def * state )
{
	float normalise;
	float ex, ey, ez;
  /* dt */
	float dt = 1.0f / 250.0f;	
	/* halT */
	float halfT = 0.5f * ( dt );
  /* change to deg */
	state->ins.gyro[0] = state->ins.gyro[0] * DEG2RAD;	/* 度转弧度 */
	state->ins.gyro[1] = state->ins.gyro[1] * DEG2RAD;
	state->ins.gyro[2] = state->ins.gyro[2] * DEG2RAD;
	
	/* 加速度计输出有效时,利用加速度计补偿陀螺仪*/
	if((state->ins.accel[0] != 0.0f) || (state->ins.accel[1] != 0.0f) || (state->ins.accel[2] != 0.0f))
	{
		/*单位化加速计测量值*/
		normalise = invSqrt(state->ins.accel[0] * state->ins.accel[0] + state->ins.accel[1] * state->ins.accel[1] + state->ins.accel[2] * state->ins.accel[2]);
		state->ins.accel[0] *= normalise;
		state->ins.accel[1] *= normalise;
		state->ins.accel[2] *= normalise;

		/*加速计读取的方向与重力加速计方向的差值，用向量叉乘计算*/
		ex = (state->ins.accel[1] * rMat[2][2] - state->ins.accel[2] * rMat[2][1]);
		ey = (state->ins.accel[2] * rMat[2][0] - state->ins.accel[0] * rMat[2][2]);
		ez = (state->ins.accel[0] * rMat[2][1] - state->ins.accel[1] * rMat[2][0]);
		
		/*误差累计，与积分常数相乘*/
		exInt += Ki * ex * dt ;  
		eyInt += Ki * ey * dt ;
		ezInt += Ki * ez * dt ;
		
		/*用叉积误差来做PI修正陀螺零偏，即抵消陀螺读数中的偏移量*/
		state->ins.gyro[0] += Kp * ex + exInt;
		state->ins.gyro[1] += Kp * ey + eyInt;
		state->ins.gyro[2] += Kp * ez + ezInt;
	}
	/* 一阶近似算法，四元数运动学方程的离散化形式和积分 */
	float q0Last = q0;
	float q1Last = q1;
	float q2Last = q2;
	float q3Last = q3;
	q0 += (-q1Last * state->ins.gyro[0] - q2Last * state->ins.gyro[1] - q3Last * state->ins.gyro[2]) * halfT;
	q1 += ( q0Last * state->ins.gyro[0] + q2Last * state->ins.gyro[2] - q3Last * state->ins.gyro[1]) * halfT;
	q2 += ( q0Last * state->ins.gyro[1] - q1Last * state->ins.gyro[2] + q3Last * state->ins.gyro[0]) * halfT;
	q3 += ( q0Last * state->ins.gyro[2] + q1Last * state->ins.gyro[1] - q2Last * state->ins.gyro[0]) * halfT;
	
	/*单位化四元数*/
	normalise = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= normalise;
	q1 *= normalise;
	q2 *= normalise;
	q3 *= normalise;
	
	imuComputeRotationMatrix();	/*计算旋转矩阵*/
	
	/*计算roll pitch yaw 欧拉角*/
	state->att.pitch = -asinf(rMat[2][0]) * RAD2DEG; 
	state->att.roll = atan2f(rMat[2][1], rMat[2][2]) * RAD2DEG;
	state->att.yaw = atan2f(rMat[1][0], rMat[0][0]) * RAD2DEG;
	/* return */
	return FS_OK;;
}

/************************************************ end of file *******************************************************/














