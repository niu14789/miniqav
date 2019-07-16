
#include <math.h>
#include "mpu9250.h"
#include "imu.h"

#define Kp 10.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.001f                   // half the sample period???????
 
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error


void IMUupdate( const MPU9250_INS_DEF * ins , ATTITUDE_DEF * att)
{
  float norm;
//  float hx, hy, hz, bx, bz;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;
 
	float acc_t[3],gyro_t[3];
	
  // ???????????
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
//  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
//  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
 
    if(ins->accel[0]*ins->accel[1]*ins->accel[2]==0)
        return;
 
  norm = sqrt(ins->accel[0]*ins->accel[0] + ins->accel[1]*ins->accel[1] + ins->accel[2]*ins->accel[2]);       //acc?????
  acc_t[0] = ins->accel[0] /norm;
  acc_t[1] = ins->accel[1] / norm;
  acc_t[2] = ins->accel[2] / norm;
 
  // estimated direction of gravity and flux (v and w)              ?????????/??
  vx = 2*(q1q3 - q0q2);                                             //????xyz???
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;
 
  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (acc_t[1]*vz - acc_t[2]*vy) ;                                             //???????????????
  ey = (acc_t[2]*vx - acc_t[0]*vz) ;
  ez = (acc_t[0]*vy - acc_t[1]*vx) ;
 
  exInt = exInt + ex * Ki;                                //???????
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;
 
  // adjusted gyroscope measurements
  gyro_t[0] = ins->gyro[0] + Kp*ex + exInt;                                              //???PI???????,???????
  gyro_t[1] = ins->gyro[1] + Kp*ey + eyInt;
  gyro_t[2] = ins->gyro[2] + Kp*ez + ezInt;                                          //???ins->gyro[2]????????????????,??????????????
 
  // integrate quaternion rate and normalise                           //????????
  q0 = q0 + (-q1*gyro_t[0] - q2*gyro_t[1] - q3*gyro_t[2])*halfT;
  q1 = q1 + (q0*gyro_t[0] + q2*gyro_t[2] - q3*gyro_t[1])*halfT;
  q2 = q2 + (q0*gyro_t[1] - q1*gyro_t[2] + q3*gyro_t[0])*halfT;
  q3 = q3 + (q0*gyro_t[2] + q1*gyro_t[1] - q2*gyro_t[0])*halfT;
 
  // normalise quaternion
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
 
  att->yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; // yaw
  att->pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  att->roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
}



