/*
 * motor_temp.h
 *
 *  Created on: Jan 7, 2025
 *      Author: Sahil
 */

#ifndef INC_MOTOR_TEMP_H_
#define INC_MOTOR_TEMP_H_

#include "stdlib.h"
#include "stdio.h"
#include "main.h"

void get_motor_temp (int*temperature);
uint8_t check_I2C_is_ready(void);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);

#endif /* INC_MOTOR_TEMP_H_ */
