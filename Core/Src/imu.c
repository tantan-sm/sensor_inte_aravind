/*
 * imu.c
 *
 *  Created on: Feb 23, 2025
 *      Author: Sahil
 */
#include "imu.h"

extern I2C_HandleTypeDef hi2c2;

extern HAL_StatusTypeDef status;
extern uint32_t error_code;
uint8_t value;
uint8_t reset = 0x20;
uint8_t chip_id=0;
uint8_t buffer[6];
uint8_t euler_data[6];
int16_t raw_yaw, raw_roll, raw_pitch;
float acc_x,acc_y,acc_z,yaw,roll,pitch;



//__weak void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
uint8_t check_IMU_is_ready(void);
void get_imu_data(void);
void test_imu(void);
void set_mode(void);
uint8_t config_mode = 0x00;




/*uint8_t check_IMU_is_ready(void)//for checking whether the device is ready to comunicate or not
{
for(int i = 0;i<10;i++)
{
status = HAL_I2C_IsDeviceReady(&hi2c1,P_BNO055 + 1, 5 ,50);
}
if(status != HAL_OK)
{
	HAL_I2C_ErrorCallback(&hi2c1);
}

	return status;

}*/





void test_imu(void)
{
	HAL_I2C_Mem_Read(&hi2c2,P_BNO055,ST_RESULT_ADDR, 1, &value, 1, 100);
	if(value == 0)
	{
		value = 0;
		HAL_Delay(50);
		HAL_I2C_Mem_Read(&hi2c2,P_BNO055,SYS_STATUS_ADDR, 1, &value, 1, 100);
		if(value != 0)
		{
			if(value == 6)
			{
				HAL_Delay(50);
				HAL_I2C_Mem_Write(&hi2c2, P_BNO055, OPR_MODE, 1, (uint8_t*)best_fusion_mode , 1, 100);//Type for fusion algorithm enable it
			}
			else
			{
				value = 0;
				HAL_Delay(50);
				HAL_I2C_Mem_Read(&hi2c2, P_BNO055, SYS_ERR_ADDR, 1, &value, 1, 100);
			}

				if(value == 10)
				{
					HAL_I2C_Mem_Write(&hi2c2, P_BNO055, SYS_TRIGGER_ADDR, 1, &reset, 1, 100);//write for chip reset code of bno055
					HAL_Delay(650);//test_imu();
					chip_id = 0;
					HAL_I2C_Mem_Read(&hi2c2, P_BNO055, CHIP_ID_ADDR, 1, &chip_id, 1, 100);
					if(chip_id != BNO055_ID)
					{
						//test_imu();
						//set_mode();
					}
				}
			}
		}
	}



void set_mode(void)
{
	HAL_I2C_Mem_Write(&hi2c2, 0x28 << 1, 0x3D, 1, &config_mode, 1, 100);
	HAL_Delay(20);
	HAL_I2C_Mem_Write(&hi2c2, P_BNO055, OPR_MODE, 1, (uint8_t*)best_fusion_mode , 1, 100); //enabling NDOF mode.
	HAL_Delay(100);

}


void get_imu_data(void)
{
	//check_IMU_is_ready();
	//HAL_I2C_Mem_Read(&hi2c1,P_BNO055,ST_RESULT_ADDR, 1, &value, 1, 100);
	if(HAL_I2C_Mem_Read(&hi2c2, P_BNO055 ,ACCEL_DATA_BASEADDR, 1, buffer, 6, 100) == HAL_OK)
	{
		acc_x = (float)(((int16_t) ((buffer[1] << 8) | buffer[0]))/100.0);
		acc_y = (float)(((int16_t) ((buffer[3] << 8) | buffer[2]))/100.0);
		acc_z = (float)(((int16_t) ((buffer[5] << 8) | buffer[4]))/100.0);
		HAL_Delay(50);
	}
	else
	{
		HAL_I2C_ErrorCallback(&hi2c2);
		//test_imu();
		//set_mode();
	}
	// HAL_Delay(50);
	if (HAL_I2C_Mem_Read(&hi2c2, P_BNO055 , 0x1A, 1, euler_data, 6, 100) == HAL_OK)
	{
		raw_yaw = (euler_data[1] << 8) | euler_data[0];
		raw_roll = (euler_data[3] << 8) | euler_data[2];
		raw_pitch = (euler_data[5] << 8) | euler_data[4];

		yaw = raw_yaw / 16.0;  //for converting to degrees
		roll = raw_roll / 16.0;
		pitch = raw_pitch / 16.0;
		HAL_Delay(50);
	}
	else
	{
		HAL_I2C_ErrorCallback(&hi2c2);
	}


	/*else
	{
		HAL_I2C_ErrorCallback(&hi2c1);
		//test_imu();
		//set_mode();
	}*/

}



/*void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) //for handling i2c error
{
	if(hi2c->Instance == I2C1)
	{
		 error_code = HAL_I2C_GetError(&hi2c1);
		if (error_code == HAL_I2C_ERROR_BERR)//error_code = 0x00000001U
		        {
		            __HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_FLAG_BERR);
		        }
		        else if (error_code == HAL_I2C_ERROR_ARLO)//error_code = 0x00000002U
		        {
		            __HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_FLAG_ARLO);
		        }
		        else if (error_code == HAL_I2C_ERROR_AF)//error_code = 0x00000004U
		        {
		            __HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_FLAG_AF);
		        }
		        else if (error_code == HAL_I2C_ERROR_TIMEOUT)//error_code = 0x00000020U
		        {
		            __HAL_I2C_CLEAR_FLAG(&hi2c1 , I2C_ISR_TIMEOUT);
		        }
		        else if (error_code == HAL_I2C_ERROR_SIZE)//error_code = 0x00000040U
		        {
		        	//__HAL_I2C_CLEAR_FLAG(&hi2c1 , I2C_ERROR_SIZE);
		        	HAL_I2C_DeInit(&hi2c1);
		        	HAL_Delay(10);
		            HAL_I2C_Init(&hi2c1);
		        }
		        else
		        {
		        	HAL_I2C_DeInit(&hi2c1);
		        	HAL_Delay(10);
		        	HAL_I2C_Init(&hi2c1);
		        }
		HAL_I2C_DeInit(&hi2c1);
    	HAL_Delay(10);
		HAL_I2C_Init(&hi2c1);

	}
}*/
