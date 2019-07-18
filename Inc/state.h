
#ifndef __IMU_H__
#define __IMU_H__

#define DEG2RAD		(0.017453293f)	/* deg to rad */
#define RAD2DEG		(57.29578f)		  /* rad to deg */

/* mpu9250 ins sensor default */
typedef struct{
	float accel[3];
	float gyro[3];
	float mpu9250_temperature;
}MPU9250_INS_DEF;
/* mpu9250 mag def */
typedef struct{
	float mag[3];
}MPU9250_MAG_DEF;
/* attitude define */
typedef struct{
	float roll;
	float pitch;
	float yaw;
}ATTITUDE_DEF;
/* redef */
typedef struct
{
	/* analog channel */
  unsigned short channel[4];
  /* digital channel */
  unsigned short  channel567;
  /* unique id */
  unsigned short unique_id;
  /* crc */
  unsigned short crc ;
  /*-------------------------*/
}rcs_HandleTypeDef;

/* system math def */
typedef struct
{
	MPU9250_INS_DEF ins;
	ATTITUDE_DEF att;
}state_def;

#endif



