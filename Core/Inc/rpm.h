/*
 * rpm.h
 *
 *  Created on: Feb 23, 2025
 *      Author: Sahil
 */

#ifndef INC_RPM_H_
#define INC_RPM_H_

#include "main.h"

#define TIM3_Interval 0.25


extern float rpm;
extern uint8_t volatile count_for_imu_fusion_mode;
extern uint8_t volatile ble_count;

void start_timer(void);
void get_rpm(void);



#endif /* INC_RPM_H_ */
