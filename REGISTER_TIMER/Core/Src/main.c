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
int16_t count=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void RCC_Config(void)
{
	RCC->CR |= 0x00010000; //HSEON enable
	while(!(RCC->CR & 0x00020000)); //HSEON Ready Flag wait
	RCC->CR |= 0x00080000; // CSS Enable
	RCC->CR |= 0x01000000; //PLL ON
	RCC->PLLCFGR |= 0x00400000;// PLL e HSE
	RCC->PLLCFGR |= 0x00000004;// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;// PLL N = 168
	RCC->PLLCFGR |= 0x00000000;// PLL P = 2
	RCC->CFGR |=0x00000000; //AHB Prescaler = 1
	RCC->CFGR |=0x00080000; //APB2 Prescaler = 2
	RCC->CFGR |=0x00001400; //APB1 Prescaler = 4
	RCC->CIR |= 0x00080000; //HSERDY Flag clear
	RCC->CIR |= 0x00800000; // CSS Flag clear
}

void TIM_Config(void)
{
	// Time2 clock enable
	RCC->APB1ENR  |= 1<<0; // same : RCC->APB1ENR  |= 0x00000001;

	//TIMx control register 1 (TIMx_CR1)
	TIM2->CR1 |= 1<<0; //CEN Timer2 Counter Enable
	TIM2->CR1 &= ~(1<<4); //Bit 4 DIR Direction // Upcounter here
						//   0: Counter used as upcounter
						// 	 1: Counter used as downcounter

	//Bits 6:5 CMS Center-aligned mode selection
		//00 Edge-aligned mode. The Counter countes up or down depending on the direction bit (DIR)
	TIM2->CR1 &= ~( 1<<5 || 1<<6 );

	//Bits 9:8  CKD : Clock Division
				// 00 tDTS = tCK_INT  // directly timer clock
				// 01 tDTS = 2 x tCK_INT // timer clock twice
				//...
	TIM2->CR1 &= ~(1<<9 || 1<<8); // Directly timer clock

	// TIMx slave mode control register  TIMx_SMCR

	// Bits 2:0 SMS Slave Mode Selection
			//  000: Slave mode disabled - if CEN = ‘1 then the prescaler is clocked directly by the internal clock.
	TIM2->SMCR &= ~(1<<0 || 1<<1 || 1<<2 );


	TIM2->EGR |= 1<<0; // Timer update generation // Especially after some configuration such as prescaler or auto-reload



	//TIMx Counter (TIMx_CNT) // Read or write counter
			//Bits 31:16CNT[31:16]: High counter value (on TIM2 and TIM5).
			//Bits 15:0CNT[15:0]: Counter value


	//TIMx prescaler (TIMx_PSC) // That is prescaler
	/*
	 Bits 15:0 PSC[15:0]: Prescaler value
 The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1).
 PSC contains the value to be loaded in the active prescaler register at each update event
(including when the counter is cleared through UG bit of TIMx_EGR register or through
trigger controller when configured in “reset mode”).
	 */
	TIM2->PSC = 41999; // We set prescaler value = 41999


	//TIMx auto-reload register (TIMx_ARR) // That is period
	/*
	 Bits 31:16ARR[31:16]: High auto-reload value (on TIM2 and TIM5).
 Bits 15:0 ARR[15:0]: Auto-reload value
 ARR is the value to be loaded in the actual auto-reload register.
 Refer to the Section18.3.1: Time-base unit for more details about ARR update and
behavior.
 The counter is blocked while the auto-reload value is null.
	 */
	TIM2->ARR =2000;

	//1 second = (TIM->PSC + 1) * (TIM2->ARR + 1)  /  TIME CLOCK
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

	RCC_Config();
	TIM_Config();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  count = TIM2->CNT;
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
