#ifndef __SENSORS_TYPES_H
#define __SENSORS_TYPES_H
#include "axis.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * sensor�������Ͷ���	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/12
 * �汾��V1.3
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

#if defined(__CC_ARM) 
	#pragma anon_unions
#endif

typedef union 
{
	struct
	{
		short x;
		short y;
		short z;
	};
	short axis[3];
} Axis3i16;

typedef union 
{
	struct 
	{
		int x;
		int y;
		int z;
	};
	int axis[3];
} Axis3i32;

typedef union 
{
	struct 
	{
		long long x;
		long long y;
		long long z;
	};
	long long axis[3];
} Axis3i64;

typedef union 
{
	struct 
	{
		float x;
		float y;
		float z;
	};
	float axis[3];
} Axis3f;
 
#endif /* __SENSORS_TYPES_H */
