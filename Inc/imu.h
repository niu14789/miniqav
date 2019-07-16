
#ifndef __IMU_H__
#define __IMU_H__

#define DEG2RAD		0.017453293f	/* deg to rad */
#define RAD2DEG		57.29578f		  /* rad to deg */

typedef struct{
	float roll;
	float pitch;
	float yaw;
}ATTITUDE_DEF;

#endif



