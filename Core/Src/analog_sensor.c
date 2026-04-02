/*
 * analog_sensor.c
 *
 *  Created on: Jan 17, 2025
 *      Author: Sahil
 */
/*
#include <analog_sensor.h>






extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;



#define vout_c1 0.00625
#define vc 4.86
#define beta_motor_controller 3528
#define beta_battery_pack 3900
#define RT0 1500 //motor controller thermistor
#define R 1500 //motor controller temp resistance
#define T0 298.15
#define RT1 1500
#define R1 1500
#define MPPT_Vol_R1 196700.0
#define MPPT_Vol_R2 3300.0
#define Bat_vol_R1  196700.0
#define Bat_vol_R2  3300.0




void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc);
void Calculate_vref(void);
void all_sensor_value (float *arr);
void sensor_calliberation(void);




HAL_StatusTypeDef analog_ready_status;




uint16_t analog_value[4];/* 1 - vref , 2 - motor_current sensor, 4 - mppt_currrent sensor,
3 - motor controller  sensor , 5 - battery pack thermistor, 6 - battery pack thermistor
7 - mppt voltage sensor , 8 - battery voltage sensor, 9 - */
/*
float average_adc[5];

uint8_t i ;
float vref_voltage;
uint32_t adc_vrefint;
uint32_t vdda_voltage;
float VRT,VR,RT,ln; //thermistor parameters variable
uint8_t motor_current;
uint8_t pack1temp;
uint8_t pack2temp;







void all_sensor_value (float *arr)
{
	Calculate_Vref();
	sensor_calliberation();
	arr[0] = (((average_adc[1]/4095.0)*vdda_voltage - 2500)/1000.0) / vout_c1;// motor current

	//arr[1] = (((float)average_adc[2]/1000.0)- (2.44))*(1/0.04)*(5/vc);
	arr[1] = (((average_adc[2]/4095.0)*vdda_voltage - 2500)/1000.0)*(1/0.04); // mppt current
	motor_current=arr[1];

	VRT = (3.3 / 4095) * average_adc[3]; //motor controller temperature
    VR = 3.3 - VRT;
	RT = VRT / (VR / R);
	ln = log(RT / RT0);
	arr[2] = (1 / ((ln / beta_motor_controller) + (1 / T0)));
	arr[2] = arr[2] - 273.15;

	/*battery pack 1 thermistor*/
/*
	VRT = 0;
	VR = 0;
	RT = 0;
	ln = 0;
	VRT = (3.3 / 4095) * average_adc[4];
	VR = 3.3 - VRT;
	RT = VRT/(VR/R1);
	ln = log(RT / RT1);
	arr[3] = (1/((ln/beta_battery_pack)+(1/T0)));
	arr[3] = arr[3] - 273.15;

	/*battery pack 2 thermistor*/
/*
	VRT = 0;
	VR = 0;
	RT = 0;
	ln = 0;
	VRT = (3.3 / 4095) * average_adc[5];
	VR = 3.3 - VRT;
	RT = VRT/(VR/R1);
	ln = log(RT / RT1);
	arr[4] = (1/((ln/beta_battery_pack)+(1/T0)));
	arr[4] = arr[4] - 273.15;

	/*mppt voltage*/
/*
	arr[5] = (average_adc[6]*(3.3/4095))*((MPPT_Vol_R1 + MPPT_Vol_R2)/MPPT_Vol_R2);//Mppt Voltage;
	/*battery voltage*/
/*
	arr[6] = (average_adc[7]*(3.3/4095))*((Bat_vol_R1 + Bat_vol_R2)/Bat_vol_R2);

	/*Smoke Sensor*/
/*
	 arr[7] = (average_adc[8]*(3.3/4095));
	 arr[8] = (average_adc[9]*(3.3/4095));
      pack1temp=arr[3];
       pack2temp=arr[4];

}



void sensor_calliberation(void)
{
	for (i = 0; i<10 ; i++)
	{
		HAL_ADC_Start_DMA(&hadc1,(uint32_t*)analog_value,5);
		//while(isADCFinished != 1);
		//isADCFinished = 0;
		average_adc[1] = average_adc[1]+analog_value[1];
		average_adc[2] = average_adc[2]+analog_value[2];
		average_adc[3] = average_adc[3]+analog_value[3];
		average_adc[4] = average_adc[4]+analog_value[4];
		average_adc[5] = average_adc[5]+analog_value[5];
		average_adc[6] = average_adc[6]+analog_value[6];
		average_adc[7] = average_adc[7]+analog_value[7];
		average_adc[8] = average_adc[8]+analog_value[8];
		average_adc[9] = average_adc[9]+analog_value[9];
		HAL_ADC_Stop_DMA(&hadc1);
	}
	average_adc[1] = average_adc[1]/10.0; //motor current sensor
	average_adc[2] = average_adc[2]/10.0; //mppt current sensor
	average_adc[3] = average_adc[3]/10.0; //motor controller temperature
	average_adc[4] = average_adc[4]/10.0; //battery pack 1 thermistor
	average_adc[5] = average_adc[5]/10.0; //battery pack 2 thermistor
	average_adc[6] = average_adc[6]/10.0; //MPPT Voltage
	average_adc[7] = average_adc[7]/10.0; //Battery_Volatge
	average_adc[8] = average_adc[8]/10.0; //Smoke Sensor 1
	average_adc[9] = average_adc[9]/10.0; //Smoke sensor 2

}*/







