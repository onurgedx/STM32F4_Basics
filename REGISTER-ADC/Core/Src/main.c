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

uint8_t adc_value;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint8_t Read_ADC()
{
	uint8_t value;
	ADC1->CR2 |= 0x40000000; //SWSTART

	// ADC_SR ADC Status Register
	while(!(ADC1->SR & 0x00000002)); // WAIT FOR ADC1 SR  EOC FLAG

	value = ADC1->DR; // ADC1-DR  ADC Data Register
	return value;
}

void RCC_Config()
{
	RCC->CR |= 0x00030000; // HSEON and HSEONRDY enable
	while(!(RCC->CR &0x00020000)); // HSEON  Ready Flag wait
	RCC->CR |= 0x00080000; // CSS Enable
	RCC->PLLCFGR |= 0x00400000; // PLL => HSE
	RCC->PLLCFGR |= 0x00000004; // PLL M =4;
	RCC->PLLCFGR |= 0x00005A00; // PLL N =168;
	RCC->PLLCFGR |= 0x00000004; // PLL P =2;
	RCC->CFGR |= 0x00000000; // AHB Prescaler = 1;
	RCC->CFGR |= 0x00080000; // APB2 Prescaler = 2;
	RCC->CFGR |= 0x00001400; // APB1 Prescaler = 4
	RCC->CIR |= 0x0008000;  //HSERDY Flag clear;
	RCC->CIR |= 0x00800000;  //CSS flag clear;
}

void GPIO_Config()
{
	RCC->AHB1ENR = 0x00000008; //  GPIOD are ACTIVATED
	GPIOD->MODER = 0x55000000; // GPIOD pin12..15 output
	GPIOD->OTYPER = 0x00000000;
	GPIOD->OSPEEDR = 0xFF000000; //
	GPIOD->PUPDR = 0x00000000;

	RCC->AHB1ENR |= 0x00000001; // GPIOA is ACTIVATED
	GPIOA->MODER |= 0x00000003; // MODER0 -> ANALOG MOD 11
	GPIOA->OSPEEDR = 0x00000003; // moder0 100MHz
}

void ADC_Config()
{
	RCC->APB2ENR |= 0x00000100; //ADC1 clock is enabled
	ADC1->CR1 |= 0x02000000; // Resolution 8bit
	ADC1->CR2 |= 0x00000001; // ADC is enable
	ADC1->SMPR2 |= 0x00000003; // 56 Cycles
	ADC->CCR |= 0x00010000;

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	RCC_Config();
	GPIO_Config();
	ADC_Config();
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

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  adc_value = Read_ADC();
	  if(adc_value>204 )
	  {
		  GPIOD->ODR = 0x0000F000;
	  }
	  else if(adc_value>153 )
	  {
	  	  GPIOD->ODR = 0x00007000;
	  }
	  else if(adc_value>102)
	  {
	  	  GPIOD->ODR = 0x00003000;
	  }
	  else if(adc_value>51 )
	  {
	  	  GPIOD->ODR = 0x00001000;
	  }
	  else
	  {
	  	  GPIOD->ODR = 0x00000000;
	  }
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
