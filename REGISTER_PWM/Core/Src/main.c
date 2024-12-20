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

void GPIO_Config()
{
	RCC->AHB1ENR |= 1<<3; // AHB1 CLOCK BUS is ACTIVATED for D port

	// GPIO D port pins ALTARNATE FUNCTION MODE 01
	GPIOD->MODER |= 2<<24 | 2<<26 | 2<<28 | 2<<30; // Port D ->  12nd,13rd, 14th,15th pins 01 AF

	//We defined alternate function mode


	// 8.4.9 GPIO alternate function low register ( GPIOx_AFRL )
	// 8.4.10 GPIO alternate function high register ( GPIOx_AFRH )

	//GPIOD->AFR[0]  LOW
	//GPIOD->AFR[1]  HIGH
	GPIOD->AFR[1] |= 2<<16 | 2<< 20 | 2<<24 | 2<<28; // Port D -> Pin 12 | 13 | 14 | 15 TIM4


}

void TIM_Config()
{
	RCC->APB1ENR |= 1<<2; // tim4 clock enabled

	TIM4->CR1 &= ~(1<<4); // DIR Direction 0 = Counter  used as upcounter.  Timer counter mode =  Upcounter
	TIM4->CR1 &= ~(3<<5); // Bits 6:5 CMS: Center-aligned mode selection
					//   00: Edge-aligned mode. The counter counts up or down depending on the direction bit DIR).


	//Bits 9:8 CKD: Clock division
	// This bit-field indicates the division ratio between the timer clock (CK_INT) frequency and
	//sampling clock used by the digital filters (ETR, TIx),
	// 00: tDTS = tCK_INT
	// 01: tDTS = 2 × tCK_INT
	// 10: tDTS = 4 × tCK_INT
	// 11: Reserved
	TIM4->CR1 &= ~(3<<8); //Timer clock div 1



	//TIMx capture/compare mode register 1 (TIMx_CCMR1)

	/*

	 Bits 1:0 CC1S: Capture/Compare 1 selection
 This bit-field defines the direction of the channel (input/output) as well as the used input.
00: CC1 channel is configured as output.
 01: CC1 channel is configured as input, IC1 is mapped on TI1.
 10: CC1 channel is configured as input, IC1 is mapped on TI2.
 11: CC1 channel is configured as input, IC1 is mapped on TRC. This mode is working only if
an internal trigger input is selected through TS bit (TIMx_SMCR register)
 Note: CC1S bits are writable only when the channel is OFF (CC1E = 0 in TIMx_CCER).

	 */

	//TIM4->CCMR1 for channel 1 and channel 2
	//TIM4->CCMR2 for channel 3 and channel 4

	TIM4->CCMR1 &= ~(3<<0);//00: CC1 channel is configured as output.

	/*
	  Bits 6:4 OC1M: Output compare 1 mode
 These bits define the behavior of the output reference signal OC1REF from which OC1 and
OC1N are derived. OC1REF is active high whereas OC1 and OC1N active level depends
on CC1P and CC1NP bits.
 000: Frozen - The comparison between the output compare register TIMx_CCR1 and the
counter TIMx_CNT has no effect on the outputs.(this mode is used to generate a timing
base).
 001: Set channel 1 to active level on match. OC1REF signal is forced high when the counter
TIMx_CNT matches the capture/compare register 1 (TIMx_CCR1).
 010: Set channel 1 to inactive level on match. OC1REF signal is forced low when the
counter TIMx_CNT matches the capture/compare register 1 (TIMx_CCR1).
 011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1.
 100: Force inactive level - OC1REF is forced low.
 101: Force active level - OC1REF is forced high.
 110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0) as long as
TIMx_CNT>TIMx_CCR1 else active (OC1REF=1).
 111: PWM mode 2 - In upcounting, channel 1 is inactive as long as TIMx_CNT<TIMx_CCR1
else active. In downcounting, channel 1 is active as long as TIMx_CNT>TIMx_CCR1 else
inactive.
 Note: In PWM mode 1 or 2, the OCREF level changes only when the result of the
comparison changes or when the output compare mode switches from “frozen” mode
to “PWM” mode.

	 */
	TIM4->CCMR1 |= 6<<4;

	/*   Bits 9:8 CC2S[1:0]: Capture/Compare 2 selection
 This bit-field defines the direction of the channel (input/output) as well as the used input.
00: CC2 channel is configured as output
 01: CC2 channel is configured as input, IC2 is mapped on TI2
 10: CC2 channel is configured as input, IC2 is mapped on TI1
 11: CC2 channel is configured as input, IC2 is mapped on TRC. This mode is working only if
an internal trigger input is selected through the TS bit (TIMx_SMCR register)
 Note: CC2S bits are writable only when the channel is OFF (CC2E = 0 in TIMx_CCER)*/
	TIM4->CCMR1 &= ~(3<<8);

	// OCM2M[2:0]
	TIM4->CCMR1 |= 6<<12;

	// Capture/compare 1 & 2 output selected
	// Capture/compare 1 & 2 PWM1 selected

	///////////////////

	TIM4->CCMR2 &= ~(3<<0);//00: CC3 channel 3 is configured as output.
	TIM4->CCMR2 |= 6<<4; // Channel3 PWM1 selected
	TIM4->CCMR2 &= ~(3<<8); //00: CC3 channel 4 is configured as output.
	TIM4->CCMR2 |= 6<<12;  // Channel 4 PWM1 selected
	// Capture/compare 3 & 4 output selected
	// Capture/compare 3 & 4 PWM1 selected



	// 18.4.9 TIMx capture/compare enable register (TIMx_CCER)
	//POLARITIES AND OUTPUTS

	TIM4->CCER |=  (1<<0 | 1<<4 | 1<<8 | 1<<12); // Capture/Compare 1 & 2 & 3 & 4 output enable

	TIM4->PSC =400 -1;// PRESCALER
	TIM4->ARR =10000 -1;// AUTO-RELOAD


	//12nd pin's pulse -> TIM4_CCR1
	//13nd pin's pulse -> TIM4_CCR2
	//14nd pin's pulse -> TIM4_CCR3
	//15nd pin's pulse -> TIM4_CCR4

	//set value between 0 and 100
	TIM4->CCR1 =2000 -1;
	TIM4->CCR2 =4000 -1;
	TIM4->CCR3 =6000 -1;
	TIM4->CCR4 =8000 -1;

	TIM4->CR1 |= 1<<0;

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
	TIM_Config();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  RCC_Config();
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
