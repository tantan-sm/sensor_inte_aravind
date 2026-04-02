/*
 * a_sensor.c
 *
 *  Created on: Feb 21, 2026
 *      Author: Sahil
 */

#include <a_sensor.h>

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;





void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc);
void Calculate_vref(void);
void a_sensor_value(void);
void sensor_calliberation(void);



uint16_t raw_adc[7];
/*
1 - Vref,
2 - PC0 Motor Current - 1
3 - PC2 Mppt Current - 2
4 - PC3 Battery Voltage - 3
5 - PA1 Motor Controller Thermistor - 4
6 - PC1 battery temperature 1 - 5
7 - PA0 battery temperature 2  - 6*/
float avg_adc[7];
volatile uint8_t isADCFinished ;
uint32_t adc_vrefint;
uint32_t vdda_voltage;
float vref_voltage;
float moc, mpptc, bv, mct, bt1, bt2, mpptcf, bv_adc;
float VRT,VR,RT,ln; //thermistor parameters variable
/*
 moc - motor current
 mpptc - mppt current
 bv - battery voltage
 mct - motor controller temperature
 bt1  - battery temperature 1
 bt2 - battery teperature 2
 */
const float alpha = 0.05;

void Calculate_Vref(void)
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)raw_adc,7);
	while(isADCFinished != 1);
	isADCFinished = 0;
	HAL_ADC_Stop_DMA(&hadc1);
	adc_vrefint = raw_adc[0];
	vdda_voltage = __HAL_ADC_CALC_VREFANALOG_VOLTAGE(adc_vrefint, ADC_RESOLUTION_12B);
	vref_voltage = vdda_voltage / 1000.0;
	vref_voltage = floor(vref_voltage*1000)/1000;

}

void sensor_calliberation(void)
{
	//memset(avg_adc,0,sizeof(avg_adc));
	for (uint8_t i = 0; i<10 ; i++)
	{
		HAL_ADC_Start_DMA(&hadc1,(uint32_t*)raw_adc,7);
		while(isADCFinished != 1);
		isADCFinished = 0;
		avg_adc[1] += raw_adc[1];
		avg_adc[2] += raw_adc[2];
		avg_adc[3] += raw_adc[3];
		avg_adc[4] += raw_adc[4];
		avg_adc[5] += raw_adc[5];
		avg_adc[6] += raw_adc[6];
	}
	avg_adc[1] = avg_adc[1]/10.0;
	avg_adc[2] = avg_adc[2]/10.0;
	avg_adc[3] = avg_adc[3]/10.0;
	avg_adc[4] = avg_adc[4]/10.0;
	avg_adc[5] = avg_adc[5]/10.0;
	avg_adc[6] = avg_adc[6]/10.0;

}


void a_sensor_value(void)
{
	sensor_calliberation();
	moc = (((avg_adc[1]/4095.0)*vdda_voltage - 2500)/1000.0) / vout_c1;// motor current
	mpptc = (((avg_adc[2]/4095.0)*vdda_voltage - 2500)/1000.0)*(1/0.04); //mppt current 0.04 sensitivity
	mpptcf += alpha * (mpptc - mpptcf);
	bv_adc = (avg_adc[3]/4095.0)*vref_voltage;
	bv = (bv_adc/VOUT_MAX)*PRIMARY_MAX_V; //battery voltage


	VRT = (3.3 / 4095) * avg_adc[4]; //motor controller temperature
	VR = 3.3 - VRT;
    RT = VRT / (VR / R);
	ln = log(RT / RT0);
	mct = (1 / ((ln / BETA_MC) + (1 / T0)));
	mct = mct - 273.15;

	VRT = 0;   //battery pack 1 thermistor
	VR = 0;
	RT = 0;
	ln = 0;
	VRT = (3.3 / 4095) * avg_adc[5];
	VR = 3.3 - VRT;
	RT = VRT/(VR/R1);
	ln = log(RT / RT1);
	bt1 = (1/((ln/beta_battery_pack)+(1/T0)));
	bt1 = bt1 - 273.15;

	VRT = 0; //battery pack 2 thermistor
	VR = 0;
	RT = 0;
	ln = 0;
	VRT = (3.3 / 4095) * avg_adc[6];
	VR = 3.3 - VRT;
	RT = VRT/(VR/R1);
	ln = log(RT / RT1);
	bt2 = (1/((ln/beta_battery_pack)+(1/T0)));
	bt2 = bt2 - 273.15;

}





HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC1)
       {
    	isADCFinished = 1;
    }
}

