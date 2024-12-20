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

char Rx_Buff[100];

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
	RCC->AHB1ENR |= 0x00000002; // GPIOB Clock Enable
	GPIOB->MODER |= (2<<20) | (2<<22); //PB10 and PB11 AF
	GPIOB-> AFR[1] |= (7<<8) | (7<<12);  // PB10 and AF7 (USART3)
}

void USART_Config()
{
	RCC-> APB1ENR |=1<<18; // USART3 CLOCK ENABLE

	// Baud rate calculating !!!
	// Baud rate register -> USART_BRR

	//9600 BAUD RATE:
	USART3->BRR |=0x1112; // Baud rate : 9600


	//CONTROL REGISTER 1  USART_CR1
	USART3->CR1 |= 1<<2;  // Rx enable
	USART3->CR1 |= 1<<3;  // Tx enable
	USART3->CR1 |= 1<<5;  // RXNE interrupt enable  // RXNEIE
	USART3->CR1 &= ~(1<<10); // No parity

	USART3->CR1 &= ~(1<<12); // 0 -> Word length 8 bit
				// 0 : 1 start bit ,8 data bits,  n Stop bit
				// 1 : 1 start bit ,9 data bits,  n Stop bit

/////////////////////
	// CR2

	USART3->CR2 &= ~(3<<12); // STOP : STOP bits  -> 00 1 Stop bit
									//00 1 Stop bit
									//01 0.5 Stop bit
									//10 2 Stop bit
									//11 1.5 Stop bit
////////////////

	// finally, we enable the usart
	USART3->CR1 |= 1<<13; // USART enable
}

void NVIC_Config()
{
	NVIC->ISER[1] |= 1<<7;
}



// interruot // we get the message that came
void USART3_IRQHandler()
{
	static int counter=0;

	volatile int Str;

	Str = USART3->SR;
	Rx_Buff[counter++] = USART3->DR; // USART3 DATA REGISTER USART_DR
}


// WHAT IF WE WANT TO SEND MESSAGE:

void Send_Char(char message)
{
	//Wait if TXE Buffer is full
	while(!(USART3->SR & 0x00000080)) ; // USART3->SR  :   Status register USART_SR index 7 (TXE) is whether true or false
	USART3->DR = message;
}

void Send_Message(char *str)
{
	while(*str)
	{
		Send_Char(*str);
		str++;
	}
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
	USART_Config();
	NVIC_Config();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Send_Message("Hello World \n");
	  for(int i= 0;i<100000;i++);


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
