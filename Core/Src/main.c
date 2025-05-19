/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

	#include "local_config.h"
	#include "tm1638_led8keys_sm.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

	/*
	 * 		TM1638 LED & KEYS:
	 * 	VCC		-> 3v3	power
	 * 	GND		-> GND	power
	 * 	STB		-> PB9	Output
	 * 	CLK		-> PB8	Output
	 * 	DIO		-> PB7	Output + Input
	 *
	 * 		stm32f103c8t6:
	 * 	SWDIO	-> PA13
	 * 	SWCLK	-> PA14
	 * 	debug 	-> PA2, 9600
	 * 	LED		-> PB2
	 * 	BTN		-> PA0
	 * 	8MHz	-> PD0, PD1
	 */

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

	int  __io_putchar		(int);	// fflush(stdout);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	#define 	DATE_as_str 	(__DATE__)
	#define 	TIME_as_str 	(__TIME__)
	DBG1("\r\n\r\n");
	DBG1("\tTM1638 + LED & KEYS board \r\n"	); // 8
	DBG1("\tTM1638 + LED 8 KEYS "	  		); // &
	int ver[3];
	ver[0] = (SOFT_VERSION / 100)     ;
	ver[1] = (SOFT_VERSION /  10) %10 ;
	ver[2] = (SOFT_VERSION      ) %10 ;
	DBG1("v%d.%d.%d \r\n",	ver[0], ver[1], ver[2] );
	DBG1("\tLED & KEYS development board\r\n");
	DBG1("\tstm32f103c8t6\r\n");
	DBG2("\t%s, %s \r\n" , DATE_as_str , TIME_as_str);
	DBG3("\tfor debug: PA2, UART2, 9600\r\n");

	TM1638_Init(BRIGHT_2);
	TM1638_Clear();	//	clear display
	DBG1("TM1638_Init - Ok.\r\n");

	DBG1("Test: 0 1 2 3 4 5 6 7\r\n");
	for (uint8_t pos = 0; pos < 8; pos++) {
		uint8_t digit 	= pos;
		uint8_t led 	= 1;
		TM1638_SetDigit(pos, digit, led); // print:  0 1 2 3 4 5 6 7
		DBG1("pos=%d dgt=%2d;   ", pos+1, digit); fflush(stdout);
		HAL_Delay(300);
	}
	DBG1("\r\n");
	HAL_Delay(1000);

	DBG1("Test: 8 9 a b c d e f\r\n");
	TM1638_Clear();	//	clear display
	for (uint8_t pos = 0; pos < 8; pos++) {
		uint8_t digit 	= pos + 8;
		uint8_t led 	= 1;
		TM1638_SetDigit(pos, digit, led); // print:  8 9 a b c d e f
		DBG1("pos=%d dgt=%2d;   ", pos+1, digit); fflush(stdout);
		HAL_Delay(300);
	}
	DBG1("\r\n");
	HAL_Delay(1000);

	DBG1("Test: blank . _ - ^ f e d\r\n");
	TM1638_Clear();	//	clear display
	for (uint8_t pos = 0; pos < 8; pos++) {
		uint8_t digit 	= 20 - pos;	//	20 = _blank_
		uint8_t led 	= 1;
		TM1638_SetDigit(pos, digit, led); // print:  blank . _ - ^ f e d
		DBG1("pos=%d dgt=%2d;   ", pos+1, digit); fflush(stdout);
		HAL_Delay(300);
	}
	DBG1("\r\n");
	HAL_Delay(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	static uint32_t cipher_u32	= 12345000;
	static uint8_t led_bit_mask = 0b00000001;

	int keys = TM1638_ReadButtons();
	if (keys == -1) {
		cipher_u32++;
		TM1638_Set_Cipher(cipher_u32, DEC_TYPE, led_bit_mask); // print cinher on display
		if (led_bit_mask == 0 ) {
			led_bit_mask = 1;
		} else {
			led_bit_mask = led_bit_mask<<1;
		}
		DBG1("%3lu ", cipher_u32); fflush(stdout);
	} else {
		uint32_t dec_multiplier = pow(10, (8-keys));
		uint32_t cipher_key	= ((8-keys)+1)*dec_multiplier;
		led_bit_mask = 1<<(8-keys);
		TM1638_Set_Cipher(cipher_key, DEC_TYPE, led_bit_mask); // flash LED
		DBG1(" %d ", keys); fflush(stdout);
	}

	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); // blink LED on STM32board
	HAL_Delay(200);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

	int __io_putchar(int ch) {	// for printf
		HAL_UART_Transmit(&UART_DEBUG, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
		return ch;
	}  //**************************************************************************

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
