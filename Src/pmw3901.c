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
/* driver */

#define RESOLUTION			(0.2131946f)/*1m�߶��� 1�����ض�Ӧ��λ�ƣ���λcm*/
#define OULIER_LIMIT 		(100)		/*������������޷�*/
#define VEL_LIMIT			(150.f)		/*�����ٶ��޷�*/

#define VEL_LPF_FILTER			/*��ͨ�˲�*/

void vTaskDelay_p(unsigned int t)
{
	t *= 200;
	
	while(t--);
}

//#define AVERAGE_FILTER		/*��ֵ�˲�*/
typedef struct opFlow_s 
{
	float pixSum[2];		/*�ۻ�����*/
	float pixComp[2];		/*���ز���*/
	float pixValid[2];		/*��Ч����*/
	float pixValidLast[2];	/*��һ����Ч����*/
	
	float deltaPos[2];		/*2֮֡���λ�� ��λcm*/
	float deltaVel[2];		/*�ٶ� ��λcm/s*/
	float posSum[2];		/*�ۻ�λ�� ��λcm*/
	float velLpf[2];		/*�ٶȵ�ͨ ��λcm/s*/
	
	unsigned char isOpFlowOk;		/*����״̬*/
	unsigned char isDataValid;		/*������Ч*/

} opFlow_t;

//static unsigned char isInit = 0;
//static unsigned char outlierCount = 0;			/*���ݲ����ü���*/

opFlow_t opFlow;	/*����*/

#if defined(__CC_ARM) 
	#pragma anon_unions	/*����֧�ֽṹ��������*/
#endif

typedef __packed struct motionBurst_s 
{
	__packed union 
	{
		unsigned char motion;
		__packed struct 
		{
			unsigned char frameFrom0    : 1;
			unsigned char runMode       : 2;
			unsigned char reserved1     : 1;
			unsigned char rawFrom0      : 1;
			unsigned char reserved2     : 2;
			unsigned char motionOccured : 1;
		};
	};

	unsigned char observation;
	int16_t deltaX;
	int16_t deltaY;

	unsigned char squal;

	unsigned char rawDataSum;
	unsigned char maxRawData;
	unsigned char minRawData;

	unsigned short shutter;
} motionBurst_t;

motionBurst_t currentMotion;

static void InitRegisters(void);

static void registerWrite(unsigned char reg, unsigned char value)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	
	unsigned char d[2] = { ( reg | 0x80 ) , value };
	
	HAL_SPI_Transmit(spi_handle,d,2,0xffff);
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

static unsigned char registerRead(unsigned char reg)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	
	unsigned char d = (reg & 0x7F);
	
	HAL_SPI_Transmit(spi_handle,&d,1,0xffff);

	HAL_SPI_Receive(spi_handle,&d,1,0xffff);
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	return d;
}

static void readMotion(motionBurst_t * motion)
{
	unsigned char address = 0x16;

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	
	unsigned char d = address;
	
	HAL_SPI_Transmit(spi_handle,&d,1,0xffff);
	
	HAL_SPI_Receive(spi_handle,(unsigned char*)motion,sizeof(motionBurst_t),0xffff);
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	unsigned short realShutter = (motion->shutter >> 8) & 0x0FF;
	realShutter |= (motion->shutter & 0x0ff) << 8;
	motion->shutter = realShutter;
}

static void InitRegisters(void)
{	
	registerWrite(0x7F, 0x00);
	registerWrite(0x61, 0xAD);
	registerWrite(0x7F, 0x03);
	registerWrite(0x40, 0x00);
	registerWrite(0x7F, 0x05);
	registerWrite(0x41, 0xB3);
	registerWrite(0x43, 0xF1);
	registerWrite(0x45, 0x14);
	registerWrite(0x5B, 0x32);
	registerWrite(0x5F, 0x34);
	registerWrite(0x7B, 0x08);
	registerWrite(0x7F, 0x06);
	registerWrite(0x44, 0x1B);
	registerWrite(0x40, 0xBF);
	registerWrite(0x4E, 0x3F);
	registerWrite(0x7F, 0x08);
	registerWrite(0x65, 0x20);
	registerWrite(0x6A, 0x18);
	registerWrite(0x7F, 0x09);
	registerWrite(0x4F, 0xAF);
	registerWrite(0x5F, 0x40);
	registerWrite(0x48, 0x80);
	registerWrite(0x49, 0x80);
	registerWrite(0x57, 0x77);
	registerWrite(0x60, 0x78);
	registerWrite(0x61, 0x78);
	registerWrite(0x62, 0x08);
	registerWrite(0x63, 0x50);
	registerWrite(0x7F, 0x0A);
	registerWrite(0x45, 0x60);
	registerWrite(0x7F, 0x00);
	registerWrite(0x4D, 0x11);
	registerWrite(0x55, 0x80);
	registerWrite(0x74, 0x1F);
	registerWrite(0x75, 0x1F);
	registerWrite(0x4A, 0x78);
	registerWrite(0x4B, 0x78);
	registerWrite(0x44, 0x08);
	registerWrite(0x45, 0x50);
	registerWrite(0x64, 0xFF);
	registerWrite(0x65, 0x1F);
	registerWrite(0x7F, 0x14);
	registerWrite(0x65, 0x67);
	registerWrite(0x66, 0x08);
	registerWrite(0x63, 0x70);
	registerWrite(0x7F, 0x15);
	registerWrite(0x48, 0x48);
	registerWrite(0x7F, 0x07);
	registerWrite(0x41, 0x0D);
	registerWrite(0x43, 0x14);
	registerWrite(0x4B, 0x0E);
	registerWrite(0x45, 0x0F);
	registerWrite(0x44, 0x42);
	registerWrite(0x4C, 0x80);
	registerWrite(0x7F, 0x10);
	registerWrite(0x5B, 0x02);
	registerWrite(0x7F, 0x07);
	registerWrite(0x40, 0x41);
	registerWrite(0x70, 0x00);

	vTaskDelay_p(10); // delay 10ms

	registerWrite(0x32, 0x44);
	registerWrite(0x7F, 0x07);
	registerWrite(0x40, 0x40);
	registerWrite(0x7F, 0x06);
	registerWrite(0x62, 0xF0);
	registerWrite(0x63, 0x00);
	registerWrite(0x7F, 0x0D);
	registerWrite(0x48, 0xC0);
	registerWrite(0x6F, 0xD5);
	registerWrite(0x7F, 0x00);
	registerWrite(0x5B, 0xA0);
	registerWrite(0x4E, 0xA8);
	registerWrite(0x5A, 0x50);
	registerWrite(0x40, 0x80);
	
//	/*��ʼ��LED_N*/
//	registerWrite(0x7F, 0x0E);
//	registerWrite(0x72, 0x0F);
//	registerWrite(0x7F, 0x00);
}
/*��λ��������*/
static void resetOpFlowData(void)
{
	for(int i=0; i<2; i++)
	{
		opFlow.pixSum[i] = 0;
		opFlow.pixComp[i] = 0;
		opFlow.pixValid[i] = 0;
		opFlow.pixValidLast[i] = 0;
	}
}

void opticalFlowInit(void)
{
	
	resetOpFlowData();
	opFlow.isOpFlowOk = 1;				

	unsigned char chipId = registerRead(0);
	unsigned char invChipId = registerRead(0x5f);
//	printf("Motion chip is: 0x%x\n", chipId);
//	printf("si pihc noitoM: 0x%x\n", invChipId);

	// �ϵ縴λ
	registerWrite(0x3a, 0x5a);
	vTaskDelay_p(5);

	InitRegisters();
	vTaskDelay_p(5);
}

extern UART_HandleTypeDef huart1;

void opticalFlowTask(void *param)
{	
	readMotion(&currentMotion);
	
	/*����2֮֡������ر仯������ʵ�ʰ�װ������� (pitch:x)  (roll:y)*/
	int16_t pixelDx = currentMotion.deltaY;
	int16_t pixelDy = -currentMotion.deltaX;

	opFlow.pixSum[0] += pixelDx;
	opFlow.pixSum[1] += pixelDy;
	
	
	HAL_UART_Transmit(&huart1,(unsigned char *)&pixelDx,2,0xffffffff);

}
unsigned char getOpFlowData(float * a,float * b)
{
	static unsigned char cnt = 0;
		
	if(1)	/*4m��Χ�ڣ���������*/
	{
		cnt= 0;
		
		opFlow.pixComp[0] = 0;
		opFlow.pixComp[1] = 0;
		opFlow.pixValid[0] = (opFlow.pixSum[0] + opFlow.pixComp[0]);	/*ʵ���������*/
		opFlow.pixValid[1] = (opFlow.pixSum[1] + opFlow.pixComp[1]);		
		
		opFlow.deltaPos[0] = 1 * (opFlow.pixValid[0] - opFlow.pixValidLast[0]);	/*2֮֡��λ�Ʊ仯������λcm*/
		opFlow.deltaPos[1] = 1 * (opFlow.pixValid[1] - opFlow.pixValidLast[1]);	
		opFlow.pixValidLast[0] = opFlow.pixValid[0];	/*��һ��ʵ���������*/
		opFlow.pixValidLast[1] = opFlow.pixValid[1];
		opFlow.deltaVel[0] = opFlow.deltaPos[0] / 1;	/*�ٶ� cm/s*/
		opFlow.deltaVel[1] = opFlow.deltaPos[1] / 1;
		
#ifdef AVERAGE_FILTER
		velFilter(opFlow.deltaVel, opFlow.velLpf);		/*�޷���ֵ�˲���*/
#else
		opFlow.velLpf[0] += (opFlow.deltaVel[0] - opFlow.velLpf[0]) * 0.15f;	/*�ٶȵ�ͨ cm/s*/
		opFlow.velLpf[1] += (opFlow.deltaVel[1] - opFlow.velLpf[1]) * 0.15f;	/*�ٶȵ�ͨ cm/s*/
#endif			
		
		opFlow.posSum[0] += opFlow.deltaPos[0];	/*�ۻ�λ�� cm*/
		opFlow.posSum[1] += opFlow.deltaPos[1];	/*�ۻ�λ�� cm*/
	}
  
   *a = opFlow.deltaVel[0];
   *b = opFlow.deltaVel[1];
	
	return opFlow.isOpFlowOk;	/*���ع���״̬*/
}










































