
#ifndef __IMU_H__
#define __IMU_H__

#define DEG2RAD		0.017453293f	/* 度转弧度 π/180 */
#define RAD2DEG		57.29578f		/* 弧度转度 180/π */

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


typedef struct  
{
	unsigned int timestamp;	/*时间戳*/

	float roll;
	float pitch;
	float yaw;
} attitude_t;

struct  vec3_s 
{
	unsigned int timestamp;	/*时间戳*/

	float x;
	float y;
	float z;
};

typedef struct vec3_s point_t;
typedef struct vec3_s velocity_t;
typedef struct vec3_s acc_t;

/* Orientation as a quaternion */
typedef struct quaternion_s 
{
	unsigned int  timestamp;

	union 
	{
		struct 
		{
			float q0;
			float q1;
			float q2;
			float q3;
		};
		struct 
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
} quaternion_t;

typedef struct toaMeasurement_s 
{
	char senderId;
	float x, y, z;
	long long rx, tx;
} toaMeasurement_t;

typedef struct tdoaMeasurement_s {
  point_t anchorPosition[2];
  float distanceDiff;
  float stdDev;
} tdoaMeasurement_t;

typedef struct positionMeasurement_s 
{
	union 
	{
		struct 
		{
			float x;
			float y;
			float z;
		};
		float pos[3];
	};
	float stdDev;
} positionMeasurement_t;

typedef struct distanceMeasurement_s 
{
	union 
	{
		struct 
		{
			float x;
			float y;
			float z;
		};
		float pos[3];
	};
	float distance;
	float stdDev;
} distanceMeasurement_t;

typedef struct zRange_s 
{
	unsigned int  timestamp;	//时间戳
	float distance;		//测量距离
	float quality;		//可信度
} zRange_t;

/** Flow measurement**/
typedef struct flowMeasurement_s 
{
	unsigned int  timestamp;
	union 
	{
		struct 
		{
			float dpixelx;  // Accumulated pixel count x
			float dpixely;  // Accumulated pixel count y
		};
		float dpixel[2];  // Accumulated pixel count
	};
	float stdDevX;      // Measurement standard deviation
	float stdDevY;      // Measurement standard deviation
	float dt;           // Time during which pixels were accumulated
} flowMeasurement_t;


/** TOF measurement**/
typedef struct tofMeasurement_s 
{
	unsigned int  timestamp;
	float distance;
	float stdDev;
} tofMeasurement_t;

typedef struct
{
	float pressure;
	float temperature;
	float asl;
} baro_t;

typedef struct
{
	Axis3f acc;
	Axis3f gyro;
	Axis3f mag;
	baro_t baro;
	point_t position;
	zRange_t zrange;
} sensorData_t;

typedef struct
{
	attitude_t attitude;
	quaternion_t attitudeQuaternion;
	point_t position;
	velocity_t velocity;
	acc_t acc;
	unsigned char isRCLocked;
} state_t;


void imuUpdate(Axis3f acc, Axis3f gyro, state_t *state , float dt);

#endif



