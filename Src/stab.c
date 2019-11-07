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
/* functions declerae */
static int default_config(void);
static int stab_heap_init(void);
static void stabilizer_thread(void *p);
static struct file * stab_fopen (FAR struct file *filp);
static int stab_ioctrl(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data);
/* USER CODE BEGIN Includes */
FS_INODE_REGISTER("stab.a",stab,stab_heap_init,0);
/* define the drone 's state */
static state_def state;
/* publish the state */
FS_SHELL_STATIC(state,state,SHELL_TYPE(_CB_RT_,TYPE_STRUCT,sizeof(state)));
/* define some param */
static struct file * imu_handle , * mpu_handle ;
/*-------------------*/
static int stab_heap_init(void)
{
  /* init as default */
	stab.flip.f_inode = &stab;
	/* file interface  */
	stab.ops.open  = stab_fopen;
	stab.ops.ioctl = stab_ioctrl;
	/* config to create task */
	stab.config = default_config;
  /* return */	
	return FS_OK;
}
/* default config for creating a task */
static int default_config(void)
{
	/* open mpu */
	if( ( mpu_handle = open("mpu9250.d",__FS_OPEN_ALWAYS )) == 0 )
	{
		/* can not find the mpu9250 dev */
		return FS_ERR;
	}
	/* open imu */
	if( ( imu_handle = open("imu.a",__FS_OPEN_ALWAYS )) == 0 ) 
	{
		/* can not find the imu application */
		return FS_ERR;		
	}
	/* create a task run as 1ms per second */
	xTaskCreate(stabilizer_thread, "stabilizer", 128 /* stack size */, 0 , 5 /* priority */ , 0 );
	/* return ok */
	return FS_OK;
}
/* file & driver 's interface */
static struct file * stab_fopen (FAR struct file *filp)
{
	/* return flip data */
	return &stab.flip;
}
/* mpu9250 ioctrl */
static int stab_ioctrl(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data)
{
	/* nothing diffrent */
	int ret = FS_OK;
	/* select a cmd */
	switch(cmd)
	{
		case 0:
			/* sus task */
		break;
		default:
			break;
	}
	/* return */
	return ret;
}
/* stabiliter task */
static void stabilizer_thread(void *p)
{
	/* Ignore compiler warnings */
	(void)p;
	/* init something */
	unsigned int lasttime;
	unsigned int freq_ctrl = 1;
	unsigned int mpu_ready_flag;
	/* loop */
	while(1)
	{
		 vTaskDelayUntil(&lasttime, 1 /* 1ms */ );
		 /* read mpu data */
		 mpu_ready_flag = fs_read(mpu_handle,&state.ins,sizeof(state.ins));
		 /* stabiliter */
		 if( ( mpu_ready_flag == sizeof(state.ins) ) && ( (( freq_ctrl ++ ) % 4 ) == 0) )
		 {
       /* 250HZ */
		   fs_read(imu_handle,&state,sizeof(state));
		 }
	}
}
































