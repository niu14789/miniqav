#ifndef __SENSORS_TYPES_H
#define __SENSORS_TYPES_H
#include "axis.h"

/********************************************************************************	 
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 * ALIENTEK MiniFly
 * sensor参数类型定义	
 * 正点原子@ALIENTEK
 * 技术论坛:www.openedv.com
 * 创建日期:2017/5/12
 * 版本：V1.3
 * 版权所有，盗版必究。
 * Copyright(C) 广州市星翼电子科技有限公司 2014-2024
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
