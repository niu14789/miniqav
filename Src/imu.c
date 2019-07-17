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
/* some decleres */
int IMUupdate( state_def * state );
static int imu_heap_init(void);
static struct file * imu_fopen (FAR struct file *filp);
static unsigned int imu_fread(FAR struct file *filp, FAR void * buffer, unsigned int buflen);
static int imu_ioctrl(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data);
/* proportional gains */
#define Kp    (10.0f)  /* proportional gain governs rate of convergence to accelerometer/magnetometer */
#define Ki    (0.008f) /* integral gain governs rate of convergence of gyroscope biases */
#define halfT (0.001f) /* half the sample period */
/* define quaternion elements for four */
/* quaternion elements representing the estimated orientation */
static float q0 = 1 , q1 = 0 , q2 = 0 , q3 = 0 ;   
/* integral error default */
static float exInt = 0, eyInt = 0, ezInt = 0; /* scaled integral error */
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
/*
  The input angular velocity unit of complementary 
	filtering attitude calculation function should be rad/s.
*/
int IMUupdate( state_def * state )
{
	/* some defines */
  float norm;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;
  /* tmp acc gyro data */
	float acc_t[3],gyro_t[3];
	/* rotation matrix */
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  /* Q0 */
  float q1q1 = q1*q1;
  /* Q1 */
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
  /* is accel OK ? */
  if( state->ins.accel[0] * 
		  state->ins.accel[1] *
	    state->ins.accel[2] == 0 )
	{
		/* no need . return direction */
    return FS_OK;
	}
	/* normalization */
  norm = sqrt( state->ins.accel[0] * state->ins.accel[0] + 
	             state->ins.accel[1] * state->ins.accel[1] + 
	             state->ins.accel[2] * state->ins.accel[2]);  
	/* calibrate the accel */
  acc_t[0] = state->ins.accel[0] / norm;
  acc_t[1] = state->ins.accel[1] / norm;
  acc_t[2] = state->ins.accel[2] / norm;
  /* estimated direction of gravity and flux (v and w) */
  vx = 2 * ( q1q3 - q0q2 );   
  vy = 2 * ( q0q1 + q2q3 );
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;
  /* error is sum of cross product between reference 
	direction of fields and direction measured by sensors */
  ex = ( acc_t[1] * vz - acc_t[2] * vy ) ;                                         
  ey = ( acc_t[2] * vx - acc_t[0] * vz ) ;
  ez = ( acc_t[0] * vy - acc_t[1] * vx ) ;
  /* error os sum of cross prodect between refefence 
	direction of fields and direction measeured by sensors */
  exInt += ex * Ki;
  eyInt += ey * Ki;
  ezInt += ez * Ki;
  /* adjusted gyroscope measurements */
  gyro_t[0] = state->ins.gyro[0] + Kp * ex + exInt; 
  gyro_t[1] = state->ins.gyro[1] + Kp * ey + eyInt;
  gyro_t[2] = state->ins.gyro[2] + Kp * ez + ezInt; 
  /* integrate quaternion rate and normalise */
  q0 += (-q1 * gyro_t[0] - q2 * gyro_t[1] - q3 * gyro_t[2] ) * halfT;
  q1 += ( q0 * gyro_t[0] + q2 * gyro_t[2] - q3 * gyro_t[1] ) * halfT;
  q2 += ( q0 * gyro_t[1] - q1 * gyro_t[2] + q3 * gyro_t[0] ) * halfT;
  q3 += ( q0 * gyro_t[2] + q1 * gyro_t[1] - q2 * gyro_t[0] ) * halfT;
  /* normalise quaternion */
  norm = sqrt( q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3 );
	/* calibrate quaternion elements */
  q0 /= norm ;
  q1 /= norm ;
  q2 /= norm ;
  q3 /= norm ;
  /* calibrate the drone's attitude */
  state->att.yaw = atan2( 2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1 ) * RAD2DEG; /* yaw */
  state->att.pitch  = asin( -2 * q1 * q3 + 2 * q0 * q2 ) * RAD2DEG; /* pitch */
  state->att.roll = atan2( 2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1 ) * RAD2DEG; /* roll */
	/* return OK */
	return FS_OK;
}
/* end of files */
















