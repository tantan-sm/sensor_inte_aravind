/*
 * motor_temp.c
 *
 *  Created on: Jan 7, 2025
 *      Author: Sahil
 */


#include <motor_temp.h>



extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;



void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
void DMA_ErrorHandler(DMA_HandleTypeDef *hdma);
uint8_t CalculateCRC(uint8_t *data, uint8_t length);


HAL_StatusTypeDef status;
HAL_StatusTypeDef data_read_status;


uint8_t i2cdata[3];
uint8_t crc_data[5];
uint8_t crc_;
uint16_t raw_data;
uint32_t error_code;




uint8_t check_I2C_is_ready(void)//for checking whether the device is ready to comunicate or not
{
for(int i = 0;i<10;i++)
{
status = HAL_I2C_IsDeviceReady(&hi2c1, (0x5A<<1)+1, 5 ,50);
}
if(status != HAL_OK)
{
	HAL_I2C_ErrorCallback(&hi2c1);
}

	return status;

}




void get_motor_temp (int*temperature)//for calculating temperature in celcius
{

	if(check_I2C_is_ready() == HAL_OK)
	{
	data_read_status = HAL_I2C_Mem_Read(&hi2c1, (0x5A<<1) , 0X07 , 1, i2cdata, 3, 100);
	if (data_read_status == HAL_OK)
	{
		raw_data = (i2cdata[1] << 8) | i2cdata[0];
		crc_data[0] = (0x5A<<1);
		crc_data[1] = 0x07;
		crc_data[2] = (0x5A<<1)+1;
		crc_data[3] = i2cdata[0];
		crc_data[4] = i2cdata[1];
		crc_ = CalculateCRC(crc_data, 5);
		if(crc_ == i2cdata[2])
		{
			if(raw_data & 0x8000U)
			{
				*temperature = 00;
			}
			else
			{
			  *temperature = (raw_data * 0.02f) - 273.15f;
			}
		}

	}
	}
}




uint8_t CalculateCRC(uint8_t *data, uint8_t length)//for calculating crc of reading a memory
{
	uint8_t crc = 0;

		    for (uint8_t i = 0; i < length; i++) {
		        crc ^= data[i];

		        for (uint8_t j = 0; j < 8; j++)
		        {
		            if (crc & 0x80) {
		                crc = (crc << 1) ^ 0x07;
		            } else {
		                crc <<= 1;
		            }
		        }
		    }

		    return crc;
}








void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) //for handling i2c error
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
	if(hi2c->Instance == I2C2)
		{
			 error_code = HAL_I2C_GetError(&hi2c2);
			if (error_code == HAL_I2C_ERROR_BERR)//error_code = 0x00000001U
			        {
			            __HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_BERR);
			        }
			        else if (error_code == HAL_I2C_ERROR_ARLO)//error_code = 0x00000002U
			        {
			            __HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_ARLO);
			        }
			        else if (error_code == HAL_I2C_ERROR_AF)//error_code = 0x00000004U
			        {
			            __HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_AF);
			        }
			        else if (error_code == HAL_I2C_ERROR_TIMEOUT)//error_code = 0x00000020U
			        {
			            __HAL_I2C_CLEAR_FLAG(&hi2c2 , I2C_ISR_TIMEOUT);
			        }
			        else if (error_code == HAL_I2C_ERROR_SIZE)//error_code = 0x00000040U
			        {
			        	//__HAL_I2C_CLEAR_FLAG(&hi2c1 , I2C_ERROR_SIZE);
			        	HAL_I2C_DeInit(&hi2c2);
			        	HAL_Delay(10);
			            HAL_I2C_Init(&hi2c2);
			        }
			        else
			        {
			        	HAL_I2C_DeInit(&hi2c2);
			        	HAL_Delay(10);
			        	HAL_I2C_Init(&hi2c2);
			        }
			HAL_I2C_DeInit(&hi2c2);
	    	HAL_Delay(10);
			HAL_I2C_Init(&hi2c2);

		}
}


