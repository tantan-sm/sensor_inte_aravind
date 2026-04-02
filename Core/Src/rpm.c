/*
 * rpm.c
 *
 *  Created on: Feb 23, 2025
 *      Author: Sahil
 */



#include <rpm.h>


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;


void start_timer(void);
void get_rpm(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


uint32_t pulse;
uint8_t volatile done = 0;
uint32_t cpulse;
float rpm;
uint8_t volatile count_for_imu_fusion_mode = 0;
uint8_t volatile ble_count = 0;


void start_timer(void)
{
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start(&htim2);
}

void get_rpm(void)
{
	if(done == 1)
	{
		HAL_TIM_Base_Stop(&htim3);
		HAL_TIM_Base_Stop(&htim2);
		done = 0;
		pulse = __HAL_TIM_GET_COUNTER(&htim2);
		rpm = (pulse/18.0)*(60.0/TIM3_Interval);
		rpm = (int)rpm;
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		start_timer();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3)
	{
	done = 1;
	count_for_imu_fusion_mode++ ;
	ble_count++ ;
	}
}

