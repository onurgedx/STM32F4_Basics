/*
 * bmp180.h
 *
 *  Created on: Nov 28, 2024
 *      Author: pc
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#define BMP180_DEVICE_WRITE_REGISTER_ADDRESS 0xEE
#define BMP180_DEVICE_READ_REGISTER_ADDRESS 0xEF

#define	BMP180_CALIBRATION_VALUE_LENGTH 22

#define BMP180_CALIBRATION_START_ADRESS 0xAA

#include "stm32f4xx_hal.h"


extern I2C_HandleTypeDef hi2c1;

typedef struct
{
	int16_t AC1;
	int16_t AC2;
	int16_t AC3;
	int16_t AC4;
	int16_t AC5;
	int16_t AC6;
	int16_t B1;
	int16_t B2;
	int16_t MB;
	int16_t MC;
	int16_t MD;
} BMP180_CalibrationValues;

// Temperature Values
int16_t unCompensatedTemperature;
float temperature;
int32_t tX1, tX2, tB5;



void BMP180_Init(void);
void BMP180_GetCalibration(void);
void BMP180_GetCalibration_Value(void);
void BMP180_Get_Uncompansated_Temperature_Value(void);
void BMP180_Get_Temperature(void);
void BMP180_Get_Uncompansated_Presure_Value(void);
void BMP180_Get_Presure(void);
#endif /* INC_BMP180_H_ */
