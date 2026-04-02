/*
 * a_sensor.h
 *
 *  Created on: Feb 21, 2026
 *      Author: Sahil
 */

#ifndef INC_A_SENSOR_H_
#define INC_A_SENSOR_H_

#include "main.h"





#define adc_max_value 4095.0;
#define vout_c1 0.00625; //motor current sensor
#define VOUT_MAX  3.25f //voltage sensor
#define PRIMARY_MAX_V    100.0f
#define BETA_MC 3950 //motor controller thermistor
#define R 100000 //series resistance of thermistor with motor controller
#define RT0 100000
#define T0 298.15
#define beta_battery_pack 3900 //battery pack thermistor
#define RT1 1500
#define R1 1500




void sensor_calliberation(void);
void Calculate_Vref(void);
void a_sensor_value(void);


extern float vref_voltage;
extern float moc, mpptc, bv, mct, bt1, bt2, mpptcf;

#endif /* INC_A_SENSOR_H_ */
