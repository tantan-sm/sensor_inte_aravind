/*
 * imu.h
 *
 *  Created on: Feb 23, 2025
 *      Author: Sahil
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_


#include "main.h"
#include "motor_temp.h"


#define BNO055_ID 					0xA0
#define SYS_TRIGGER_ADDR 0x3F
#define P_BNO055  (0x28<<1)
#define ST_RESULT_ADDR	0x36
#define SYS_ERR_ADDR	0x3A
#define SYS_STATUS_ADDR	0x39
#define ACCEL_DATA_BASEADDR 0x08
#define OPR_MODE_ADDR  0x3D
#define CHIP_ID_ADDR 				0x00
#define OPR_MODE 0x3D
#define best_fusion_mode  0x0c


void get_imu_data(void);
void test_imu(void);
void set_mode(void);




extern uint8_t value;

extern uint8_t chip_id;
extern uint8_t buffer[6];
extern uint8_t euler_data[6];
extern int16_t raw_yaw, raw_roll, raw_pitch;
extern float acc_x,acc_y, acc_z ,yaw,roll,pitch;


#endif /* INC_IMU_H_ */
