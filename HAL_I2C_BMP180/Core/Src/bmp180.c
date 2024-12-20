/*
 * bmp180.c
 *
 *  Created on: Nov 28, 2024
 *      Author: Onur Gedik
 */

#include "bmp180.h"

void BMP180_Init()
{
	if(HAL_I2C_IsDeviceReady(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 1, 100)!=HAL_OK)
	{

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	}

	BMP180_GetCalibration_Value();

}

void BMP180_GetCalibration()
{

}

BMP180_CalibrationValues calibrationValues = {0};

void BMP180_GetCalibration_Value()
{
	uint8_t calibBuff[BMP180_CALIBRATION_VALUE_LENGTH] = {0};
	/**
	  * @brief  Read an amount of data in blocking mode from a specific memory address
	  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
	  *                the configuration information for the specified I2C.
	  * @param  DevAddress Target device address: The device 7 bits address value
	  *         in datasheet must be shifted to the left before calling the interface
	  * @param  MemAddress Internal memory address
	  * @param  MemAddSize Size of internal memory address
	  * @param  pData Pointer to data buffer
	  * @param  Size Amount of data to be sent
	  * @param  Timeout Timeout duration
	  * @retval HAL status
	  */
	if(	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, BMP180_CALIBRATION_START_ADRESS, 1, calibBuff, BMP180_CALIBRATION_VALUE_LENGTH, 100)==HAL_OK)
	{
		calibrationValues.AC1 = (uint16_t)(  calibBuff[0]<<8 | calibBuff[1]);
		calibrationValues.AC2 = (uint16_t)( calibBuff[2]<<8 | calibBuff[3]);
		calibrationValues.AC3 = (uint16_t)( calibBuff[4]<<8 | calibBuff[5]);
		calibrationValues.AC4 = (uint16_t)( calibBuff[6]<<8 | calibBuff[7]);
		calibrationValues.AC5 = (uint16_t)( calibBuff[8]<<8 | calibBuff[9]);
		calibrationValues.AC6 = (uint16_t)( calibBuff[10]<<8 | calibBuff[11]);
		calibrationValues.B1 = (uint16_t)( calibBuff[12]<<8 | calibBuff[13]);
		calibrationValues.B2 = (uint16_t)( calibBuff[14]<<8 | calibBuff[15]);
		calibrationValues.MB = (uint16_t)( calibBuff[16]<<8 | calibBuff[17]);
		calibrationValues.MC = (uint16_t)( calibBuff[18]<<8 | calibBuff[19]);
		calibrationValues.MD = (uint16_t)( calibBuff[20]<<8 | calibBuff[21]);
	}

}

void BMP180_Get_Uncompansated_Temperature_Value(void)
{
	uint8_t rData[2] = {0};
	uint8_t wData[1];
	wDat[0] =0x2E;
	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS , 0xF4, 1, wData, 1, 10000);
	HAL_Delay(5);
	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS , 0xF6, 1, rData, 1, 10000);
	unCompTemperature = (int16_t)( (rData[0]<<8) |rData[1]);



}
void BMP180_Get_Temperature(void)
{
	tX1 = ((int32_t) unCompTemperature - (int32_t)calibrationValues.AC6) * (int32_t)calibrationValues.AC5 / 32768;
	tX2 =(int32_t)calibrationValues.MC * 2048 / (tX1 + (int32_t)calibrationValues.MD);
	tB5 = tX1 + tX2;
	temperature = (float)(tB5 +8)/16;
}
void BMP180_Get_Uncompansated_Presure_Value(void);
void BMP180_Get_Presure(void);

