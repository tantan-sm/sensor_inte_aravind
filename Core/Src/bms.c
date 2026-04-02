/*
 * bms.c
 *
 *  Created on: Mar 11, 2025
 *      Author: sahil
 */
#include "bms.h"

extern UART_HandleTypeDef huart4;
//extern DMA_HandleTypeDef hdma_uart4_rx;
// extern DMA_HandleTypeDef hdma_uart4_tx;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void calcChecksum(void);
void readBasicInfo();




volatile uint8_t transmit = 0;
volatile uint8_t uart_data_ready = 0;
uint8_t requestBasicInfo[13] = {0xA5, 0x40, 0x90, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t response[13];
float totalVoltage;
float soc;

void calcChecksum()
{
  uint16_t sum = 0;
  for (int i = 0; i < 12; i++) sum += requestBasicInfo[i];
  requestBasicInfo[12] = sum & 0xFF;
}



void readBasicInfo()
{
	calcChecksum();
	if(transmit == 0)
	{
      HAL_UART_Transmit_DMA(&huart4,requestBasicInfo,13);
      transmit = 1;
	}

	HAL_UART_Receive_DMA(&huart4,response,13);

	if((transmit == 1) && (uart_data_ready == 1))
	{
    if (response[0] == 0xA5 && response[2] == 0x90)
    {
      totalVoltage = ((response[4] << 8) | response[5]) * 0.1;

      soc = ((response[10] << 8) | response[11]) * 0.1;
      if (soc > 100.0)
    	  {
    	  soc = 100.0;
    	  }
      if (soc < 0.0)
    	  {
    	  soc = 0.0;
    	  }
    }
    transmit = 0;
    uart_data_ready = 0;
  }
}





void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == UART4)
	{
	        uart_data_ready = 1;
	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == UART4)
	{
		transmit = 1;
	}
}
