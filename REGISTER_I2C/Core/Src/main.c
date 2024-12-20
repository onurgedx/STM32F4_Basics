/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void RCC_Config()
{
    // HSE (High Speed External clock) Oscillator Enable
    RCC->CR |= 0x00010000;

    // Wait until HSE is ready
    while (!(RCC->CR & 0x00020000));

    // PLL (Phase Locked Loop) Enable
    RCC->CR |= 0x01000000;

    // Configure PLL: PLL Source = HSE, PLL_M = 4, PLL_N = 168, PLL_P = 2
    RCC->PLLCFGR |= 0x00400000;  // PLL Source = HSE
    RCC->PLLCFGR |= 0x00000004;  // PLL_M = 4
    RCC->PLLCFGR |= 0x00005A00;  // PLL_N = 168
    RCC->PLLCFGR |= 0x00000000;  // PLL_P = 2

    // Configure AHB and APB Prescalers
    RCC->CFGR |= 0x00000000;     // AHB Prescaler = 1
    RCC->CFGR |= 0x00008000;     // APB2 Prescaler = 2
    RCC->CFGR |= 0x00001400;     // APB1 Prescaler = 4

    // Clear Flags (optional, depending on initialization sequence)
    RCC->CIR = 0x00800000;       // Clear HSE Ready Interrupt Flag
}

void GPIO_Config()
{
	RCC->AHB1ENR |= 3<<0; // GPIOA and GPIOB clock enabled
	GPIOB-> AFR[1] |= (4<<8) | (4<<12)   ;  // PB10 and PB11 AF4  (I2C)
	GPIOB->MODER |= (2<<20) | (2<<22); //PB10 and PB11 AF
	GPIOB->OTYPER |= (1<<11) | (1<<12); // Open-Drain
}

void I2C_Config()
{
	RCC->APB1ENR |= 1<<22; // i2c clock enable

	I2C2->CR2 |= 0x0008;	// i2c clock 8MHz
	I2C2->CRR |= 0x0028;	// i2c 100 kHz
	I2C2->TRISE |= 0x09;
	I2C2->CR1 |= 0x0001; // i2c enable
}

uint8_t m_adress = 0x4E;
void I2C_Write(uint8_ adresss, uint8_t data)
{
	I2C2->CR1 |=1<<8; // send start bit
	while(!(I2C2->SR1 & 0x0001)); 	// wait for start
	I2C2->DR |=0x4E; // slave adress
	while(!(I2C2->SR1 & 0x0002)); // wait send adress (acknowledge)
	while(!(I2C2->SR2 & 0x0001)); // wiat Master selected

	//I2C2->DR = adress

	while(!(I2C2->SR1 & 0x0080 )); // wait until TXE empty

	I2C2->DR = data;
	while(!(I2C2->SR1 & 0x0080 )); // wait until TXE empty

	while(!(I2C2->SR1 & 0x0004 )); // wait until BTF successful

	I2C2->CR1 |= 1<<9;			// send stop bit


}




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
  //HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  //SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
	RCC_Config();
	GPIO_Config();
	I2C_Config();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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

#ifdef  USE_FULL_ASSERT
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
