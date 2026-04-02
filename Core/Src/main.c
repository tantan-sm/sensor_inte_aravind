	/* USER CODE BEGIN Header */
	/**
	  ******************************************************************************
	  * @file           : main.c
	  * @brief          : Main program body
	  ******************************************************************************
	  * @attention
	  *
	  * Copyright (c) 2025 STMicroelectronics.
	  * All rights reserved.
	  *
	  * This software is licensed under terms that can be found in the LICENSE file
	  * in the root directory of this software component.
	  * If no LICENSE file comes with this software, it is provided AS-IS.
	  *
	  ******************************************************************************
	  */
	/* USER CODE END Header */
	/* Includes ------------------------------------------------------------------*/
	#include "main.h"
	#include "app_fatfs.h"

	/* Private includes ----------------------------------------------------------*/
	/* USER CODE BEGIN Includes */
	#include <motor_temp.h>
	//#include <analog_sensor.h>
	//#include <rpm.h>
	#include <imu.h>
	#include <bms.h>
	#include <a_sensor.h>
	#include "sd_functions.h"
	#include "stdio.h"
	#define CAPTURE_BUF_LEN 8
	#define RPM_TIMEOUT_MS 2500
	#define pulseonerev 6
	/* USER CODE END Includes */

	/* Private typedef -----------------------------------------------------------*/
	/* USER CODE BEGIN PTD */

	/* USER CODE END PTD */

	/* Private define ------------------------------------------------------------*/
	/* USER CODE BEGIN PD */

	/* USER CODE END PD */

	/* Private macro -------------------------------------------------------------*/
	/* USER CODE BEGIN PM */

	/* USER CODE END PM */

	/* Private variables ---------------------------------------------------------*/
	ADC_HandleTypeDef hadc1;
	DMA_HandleTypeDef hdma_adc1;

	FDCAN_HandleTypeDef hfdcan1;

	I2C_HandleTypeDef hi2c1;
	I2C_HandleTypeDef hi2c2;

	SPI_HandleTypeDef hspi1;
	DMA_HandleTypeDef hdma_spi1_rx;
	DMA_HandleTypeDef hdma_spi1_tx;

	TIM_HandleTypeDef htim2;
	TIM_HandleTypeDef htim3;
	DMA_HandleTypeDef hdma_tim2_ch1;

	UART_HandleTypeDef huart4;
	UART_HandleTypeDef huart3;
	DMA_HandleTypeDef hdma_uart4_rx;
	DMA_HandleTypeDef hdma_uart4_tx;

	/* USER CODE BEGIN PV */

	/* USER CODE END PV */

	/* Private function prototypes -----------------------------------------------*/
	void SystemClock_Config(void);
	static void MX_GPIO_Init(void);
	static void MX_DMA_Init(void);
	static void MX_I2C1_Init(void);
	static void MX_ADC1_Init(void);
	static void MX_TIM2_Init(void);
	static void MX_TIM3_Init(void);
	static void MX_UART4_Init(void);
	static void MX_I2C2_Init(void);
	static void MX_FDCAN1_Init(void);
	static void MX_SPI1_Init(void);
	static void MX_USART3_UART_Init(void);
	/* USER CODE BEGIN PFP */

	/* USER CODE END PFP */

	/* Private user code ---------------------------------------------------------*/
	/* USER CODE BEGIN 0 */


	extern float totalVoltage;
	extern float soc;
	int motor_temp;
	extern uint8_t motor_current;
	int i=0;
	FRESULT mount_sd;
	FRESULT unmount_sd;
	FDCAN_TxHeaderTypeDef TxHeader;
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t TxData[16];
	uint8_t RxData[16];
	void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
	{

		if(HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0,&RxHeader,RxData)!=HAL_OK)
		{
			Error_Handler();
		}
		if(HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0)!=HAL_OK)
			{
				Error_Handler();
			}
	}
	//rpm
	//
		volatile uint16_t ic_buf[CAPTURE_BUF_LEN];
		volatile uint8_t edge_seen = 0;

		volatile uint32_t period_us = 0;
		volatile float rpm = 0.0f;
		volatile uint32_t last_edge_tick = 0;
		uint16_t rpm_int=0;
		void Process_DMA_Captures(uint8_t start, uint8_t end)
		{
			static uint16_t last_capture;
			static uint8_t first = 1;

			uint32_t sum_dt = 0;
			uint32_t edges = 0;

			for (uint8_t i = start; i < end; i++)
			{
				uint16_t now = ic_buf[i];

				if (first)
				{
					last_capture = now;
					first = 0;
					continue;
				}

				uint16_t dt = (uint16_t)(now - last_capture);
				last_capture = now;

				if (dt)
				{
					sum_dt += dt;
					edges++;
				}
			}

			if (edges)
			{
				float avg_ticks = (float)sum_dt / edges;
				rpm = 60.0f * 1e6f / (avg_ticks*pulseonerev);
				last_edge_tick = HAL_GetTick();
			}
		}
		void HAL_TIM_IC_CaptureHalfCpltCallback(TIM_HandleTypeDef *htim)
		{
		    if (htim->Instance == TIM2)
		    {

		        Process_DMA_Captures(0, CAPTURE_BUF_LEN / 2);

		    }
		}

		void HAL_TIM_IC_CaptureCpltCallback(TIM_HandleTypeDef *htim)
		{
		    if (htim->Instance == TIM2)
		    {

		        Process_DMA_Captures(CAPTURE_BUF_LEN / 2, CAPTURE_BUF_LEN);

		    }
		}
	//
	/* USER CODE END 0 */

	/**
	  * @brief  The application entry point.
	  * @retval int
	  */
	int main(void)
	{

	  /* USER CODE BEGIN 1 */

	  /* USER CODE END 1 */

	  /* MCU Configuration--------------------------------------------------------*/

	  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	  HAL_Init();

	  /* USER CODE BEGIN Init */

	  /* USER CODE END Init */

	  /* Configure the system clock */
	  SystemClock_Config();

	  /* USER CODE BEGIN SysInit */

	  /* USER CODE END SysInit */

	  /* Initialize all configured peripherals */
	  MX_GPIO_Init();
	  MX_DMA_Init();
	  MX_I2C1_Init();
	  MX_ADC1_Init();
	  MX_TIM2_Init();
	  MX_TIM3_Init();
	  MX_UART4_Init();
	  MX_I2C2_Init();
	  MX_FDCAN1_Init();
	  MX_SPI1_Init();
	  MX_USART3_UART_Init();
	  /* USER CODE BEGIN 2 */
	  //can
	  if (HAL_FDCAN_Start(&hfdcan1)!=HAL_OK)
		   {
			Error_Handler();
		   }
		   if(HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0)!=HAL_OK)
		   {
			Error_Handler();
		   }

		   TxHeader.Identifier =0x33;
		   TxHeader.IdType =FDCAN_STANDARD_ID;
		   TxHeader.DataLength =FDCAN_DLC_BYTES_16;
		   TxHeader.ErrorStateIndicator =FDCAN_ESI_ACTIVE;
		   TxHeader.BitRateSwitch =FDCAN_BRS_OFF;
		   TxHeader.FDFormat =FDCAN_FD_CAN;
		   TxHeader.TxEventFifoControl =FDCAN_NO_TX_EVENTS;
		   TxHeader.MessageMarker=0;

	//
	 // test_imu();
	  set_mode();
	  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	  //start_timer();
	  //sensor_calliberation();
	  HAL_TIM_IC_Start_DMA(
			  &htim2,
			  TIM_CHANNEL_1,
			  (uint32_t*)ic_buf,
			  CAPTURE_BUF_LEN
		  );
	  //
	  //sd card
	  //
	 mount_sd=sd_mount();




	  //sd_unmount();
	  HAL_Delay(1000);
	  //start_timer();
	  //HAL_TIM_Base_Start_IT(&htim3);
	  /* USER CODE END 2 */

	  /* Infinite loop */
	  /* USER CODE BEGIN WHILE */
	  while (1)
	  {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */


		  Calculate_Vref();
		  a_sensor_value ();
		  readBasicInfo();
		  get_motor_temp(&motor_temp);
		  //get_rpm();
		  get_imu_data();
		  if (HAL_GetTick() - last_edge_tick > RPM_TIMEOUT_MS)
														{
															rpm = 0.0f;
														}

	TxData[0]=((int)rpm & 0xFF);
	TxData[1]=(((int)rpm >> 8) & 0xFF);
	TxData[2]=totalVoltage;
	TxData[3]=bt1;
	TxData[4]=bt2;
	TxData[5]=soc;
	TxData[6]=((int)moc & 0xFF);
	TxData[7]=(((int)moc >> 8) & 0xFF);
	TxData[8]=motor_temp;
	TxData[9]=mct;



		  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData)!= HAL_OK)
															   {
												//watchdog
															   }

		  char logbuf[320];

		  snprintf(logbuf, sizeof(logbuf),
			  "MOC:%.2f ,MPPTC:%.2f ,BV:%.2f, MCT:%.1f, BT1:%.1f, BT2:%.1f,MPPTCF:%.2f,VREF:%.2f,TV:%.2f,SOC:%.2f,"
			  ",AX:%.4f,AY:%.4f,AZ:%.4f,YAW:%.4f,ROLL:%.4f,PITCH:%.4f,RPM:%.0f,MT:%d,LAT:  %d  %d  %d%d%d%d,LON:  %d  %d%d  %d%d%d%d\r\n",
			  moc, mpptc, bv, mct, bt1, bt2, mpptcf, vref_voltage, totalVoltage,
			  soc,
			  acc_x, acc_y, acc_z,
			  yaw, roll, pitch,
			  rpm, motor_temp,RxData[0],RxData[1],RxData[2],RxData[3],RxData[4],RxData[5],RxData[7],RxData[8],RxData[9],RxData[10],RxData[11],RxData[12],RxData[13]);
		 if(mount_sd!=FR_OK)
		  {
			 // sd_unmount();
			  mount_sd=sd_mount();

		  }

		  if(sd_append_file("File6.txt", logbuf)!=FR_OK)
		  {
			 // sd_unmount();
			  mount_sd=sd_mount();
		  }

		  //for setting the imu in fusion mode so that roll, pitch and yaw can be get
		 /* if(count_for_imu_fusion_mode >= 40)  //used the same timer for rpm one just increase the variable
		  {                                    // for every timer interrupt and 40 is used because rpm timer is
			   count_for_imu_fusion_mode = 0;  // for 250ms and I needed to set the imu to fusion mode every 10 sec
											   //  so 1000ms/250ms = 4 and 10*4 = 40 counts;
			  set_mode();                      // function to set imu in the fusion mode;
		  }*/

	  }
	  /* USER CODE END 3 */
	}

	/**
	  * @brief System Clock Configuration
	  * @retval None
	  */
	void SystemClock_Config(void)
	{
	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  /** Configure the main internal regulator output voltage
	  */
	  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure.
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	  {
		Error_Handler();
	  }
	}

	/**
	  * @brief ADC1 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_ADC1_Init(void)
	{

	  /* USER CODE BEGIN ADC1_Init 0 */

	  /* USER CODE END ADC1_Init 0 */

	  ADC_MultiModeTypeDef multimode = {0};
	  ADC_ChannelConfTypeDef sConfig = {0};

	  /* USER CODE BEGIN ADC1_Init 1 */

	  /* USER CODE END ADC1_Init 1 */

	  /** Common config
	  */
	  hadc1.Instance = ADC1;
	  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1.Init.GainCompensation = 0;
	  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  hadc1.Init.LowPowerAutoWait = DISABLE;
	  hadc1.Init.ContinuousConvMode = DISABLE;
	  hadc1.Init.NbrOfConversion = 7;
	  hadc1.Init.DiscontinuousConvMode = DISABLE;
	  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc1.Init.DMAContinuousRequests = DISABLE;
	  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	  hadc1.Init.OversamplingMode = DISABLE;
	  if (HAL_ADC_Init(&hadc1) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure the ADC multi-mode
	  */
	  multimode.Mode = ADC_MODE_INDEPENDENT;
	  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_VREFINT;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_6;
	  sConfig.Rank = ADC_REGULAR_RANK_2;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_8;
	  sConfig.Rank = ADC_REGULAR_RANK_3;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_9;
	  sConfig.Rank = ADC_REGULAR_RANK_4;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_2;
	  sConfig.Rank = ADC_REGULAR_RANK_5;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_7;
	  sConfig.Rank = ADC_REGULAR_RANK_6;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = ADC_REGULAR_RANK_7;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN ADC1_Init 2 */

	  /* USER CODE END ADC1_Init 2 */

	}

	/**
	  * @brief FDCAN1 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_FDCAN1_Init(void)
	{

	  /* USER CODE BEGIN FDCAN1_Init 0 */

	  /* USER CODE END FDCAN1_Init 0 */

	  /* USER CODE BEGIN FDCAN1_Init 1 */

	  /* USER CODE END FDCAN1_Init 1 */
	  hfdcan1.Instance = FDCAN1;
	  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
	  hfdcan1.Init.FrameFormat = FDCAN_FRAME_FD_NO_BRS;
	  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
	  hfdcan1.Init.AutoRetransmission = ENABLE;
	  hfdcan1.Init.TransmitPause = DISABLE;
	  hfdcan1.Init.ProtocolException = DISABLE;
	  hfdcan1.Init.NominalPrescaler = 8;
	  hfdcan1.Init.NominalSyncJumpWidth = 5;
	  hfdcan1.Init.NominalTimeSeg1 = 26;
	  hfdcan1.Init.NominalTimeSeg2 = 5;
	  hfdcan1.Init.DataPrescaler = 8;
	  hfdcan1.Init.DataSyncJumpWidth = 1;
	  hfdcan1.Init.DataTimeSeg1 = 2;
	  hfdcan1.Init.DataTimeSeg2 = 1;
	  hfdcan1.Init.StdFiltersNbr = 1;
	  hfdcan1.Init.ExtFiltersNbr = 0;
	  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN FDCAN1_Init 2 */
	  FDCAN_FilterTypeDef sFilterConfig;
		   sFilterConfig.IdType =FDCAN_STANDARD_ID;
		   sFilterConfig.FilterIndex=0;
		   sFilterConfig.FilterType=FDCAN_FILTER_MASK;
		   sFilterConfig.FilterConfig =FDCAN_FILTER_DISABLE;
		   sFilterConfig.FilterID1 =0x11;
		   sFilterConfig.FilterID2 =0x7FF;
		   if(HAL_FDCAN_ConfigFilter(&hfdcan1,&sFilterConfig)!=HAL_OK){
			Error_Handler();
		   }
	  /* USER CODE END FDCAN1_Init 2 */

	}

	/**
	  * @brief I2C1 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_I2C1_Init(void)
	{

	  /* USER CODE BEGIN I2C1_Init 0 */

	  /* USER CODE END I2C1_Init 0 */

	  /* USER CODE BEGIN I2C1_Init 1 */

	  /* USER CODE END I2C1_Init 1 */
	  hi2c1.Instance = I2C1;
	  hi2c1.Init.Timing = 0x00503D58;
	  hi2c1.Init.OwnAddress1 = 0;
	  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c1.Init.OwnAddress2 = 0;
	  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Analogue filter
	  */
	  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Digital filter
	  */
	  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN I2C1_Init 2 */

	  /* USER CODE END I2C1_Init 2 */

	}

	/**
	  * @brief I2C2 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_I2C2_Init(void)
	{

	  /* USER CODE BEGIN I2C2_Init 0 */

	  /* USER CODE END I2C2_Init 0 */

	  /* USER CODE BEGIN I2C2_Init 1 */

	  /* USER CODE END I2C2_Init 1 */
	  hi2c2.Instance = I2C2;
	  hi2c2.Init.Timing = 0x00503D58;
	  hi2c2.Init.OwnAddress1 = 0;
	  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c2.Init.OwnAddress2 = 0;
	  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Analogue filter
	  */
	  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Configure Digital filter
	  */
	  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN I2C2_Init 2 */

	  /* USER CODE END I2C2_Init 2 */

	}

	/**
	  * @brief SPI1 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_SPI1_Init(void)
	{

	  /* USER CODE BEGIN SPI1_Init 0 */

	  /* USER CODE END SPI1_Init 0 */

	  /* USER CODE BEGIN SPI1_Init 1 */

	  /* USER CODE END SPI1_Init 1 */
	  /* SPI1 parameter configuration*/
	  hspi1.Instance = SPI1;
	  hspi1.Init.Mode = SPI_MODE_MASTER;
	  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	  hspi1.Init.NSS = SPI_NSS_SOFT;
	  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	  hspi1.Init.CRCPolynomial = 7;
	  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	  if (HAL_SPI_Init(&hspi1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN SPI1_Init 2 */

	  /* USER CODE END SPI1_Init 2 */

	}

	/**
	  * @brief TIM2 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_TIM2_Init(void)
	{

	  /* USER CODE BEGIN TIM2_Init 0 */

	  /* USER CODE END TIM2_Init 0 */

	  TIM_MasterConfigTypeDef sMasterConfig = {0};
	  TIM_IC_InitTypeDef sConfigIC = {0};

	  /* USER CODE BEGIN TIM2_Init 1 */

	  /* USER CODE END TIM2_Init 1 */
	  htim2.Instance = TIM2;
	  htim2.Init.Prescaler = 15;
	  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim2.Init.Period = 65535;
	  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	  sConfigIC.ICFilter = 0;
	  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN TIM2_Init 2 */

	  /* USER CODE END TIM2_Init 2 */

	}

	/**
	  * @brief TIM3 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_TIM3_Init(void)
	{

	  /* USER CODE BEGIN TIM3_Init 0 */

	  /* USER CODE END TIM3_Init 0 */

	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	  TIM_MasterConfigTypeDef sMasterConfig = {0};

	  /* USER CODE BEGIN TIM3_Init 1 */

	  /* USER CODE END TIM3_Init 1 */
	  htim3.Instance = TIM3;
	  htim3.Init.Prescaler = 15999;
	  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim3.Init.Period = 199;
	  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN TIM3_Init 2 */

	  /* USER CODE END TIM3_Init 2 */

	}

	/**
	  * @brief UART4 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_UART4_Init(void)
	{

	  /* USER CODE BEGIN UART4_Init 0 */

	  /* USER CODE END UART4_Init 0 */

	  /* USER CODE BEGIN UART4_Init 1 */

	  /* USER CODE END UART4_Init 1 */
	  huart4.Instance = UART4;
	  huart4.Init.BaudRate = 9600;
	  huart4.Init.WordLength = UART_WORDLENGTH_8B;
	  huart4.Init.StopBits = UART_STOPBITS_1;
	  huart4.Init.Parity = UART_PARITY_NONE;
	  huart4.Init.Mode = UART_MODE_TX_RX;
	  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	  if (HAL_UART_Init(&huart4) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN UART4_Init 2 */

	  /* USER CODE END UART4_Init 2 */

	}

	/**
	  * @brief USART3 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_USART3_UART_Init(void)
	{

	  /* USER CODE BEGIN USART3_Init 0 */

	  /* USER CODE END USART3_Init 0 */

	  /* USER CODE BEGIN USART3_Init 1 */

	  /* USER CODE END USART3_Init 1 */
	  huart3.Instance = USART3;
	  huart3.Init.BaudRate = 115200;
	  huart3.Init.WordLength = UART_WORDLENGTH_8B;
	  huart3.Init.StopBits = UART_STOPBITS_1;
	  huart3.Init.Parity = UART_PARITY_NONE;
	  huart3.Init.Mode = UART_MODE_TX_RX;
	  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	  if (HAL_UART_Init(&huart3) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN USART3_Init 2 */

	  /* USER CODE END USART3_Init 2 */

	}

	/**
	  * Enable DMA controller clock
	  */
	static void MX_DMA_Init(void)
	{

	  /* DMA controller clock enable */
	  __HAL_RCC_DMAMUX1_CLK_ENABLE();
	  __HAL_RCC_DMA1_CLK_ENABLE();

	  /* DMA interrupt init */
	  /* DMA1_Channel1_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	  /* DMA1_Channel2_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	  /* DMA1_Channel3_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	  /* DMA1_Channel4_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	  /* DMA1_Channel5_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	  /* DMA1_Channel6_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

	}

	/**
	  * @brief GPIO Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_GPIO_Init(void)
	{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  /* USER CODE BEGIN MX_GPIO_Init_1 */
	  /* USER CODE END MX_GPIO_Init_1 */

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

	  /*Configure GPIO pin : CS_Pin */
	  GPIO_InitStruct.Pin = CS_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(CS_GPIO_Port, &GPIO_InitStruct);

	  /* USER CODE BEGIN MX_GPIO_Init_2 */
	  /* USER CODE END MX_GPIO_Init_2 */
	}

	/* USER CODE BEGIN 4 */

	/* USER CODE END 4 */

	/**
	  * @brief  This function is executed in case of error occurrence.
	  * @retval None
	  */
	void Error_Handler(void)
	{
	  /* USER CODE BEGIN Error_Handler_Debug */
	  /* User can add his own implementation to report the HAL error return state */
	  __disable_irq();
	  while (1)
	  {
	  }
	  /* USER CODE END Error_Handler_Debug */
	}
	#ifdef USE_FULL_ASSERT
	/**
	  * @brief  Reports the name of the source file and the source line number
	  *         where the assert_param error has occurred.
	  * @param  file: pointer to the source file name
	  * @param  line: assert_param error line source number
	  * @retval None
	  */
	void assert_failed(uint8_t *file, uint32_t line)
	{
	  /* USER CODE BEGIN 6 */
	  /* User can add his own implementation to report the file name and line number,
		 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	  /* USER CODE END 6 */
	}
	#endif /* USE_FULL_ASSERT */
