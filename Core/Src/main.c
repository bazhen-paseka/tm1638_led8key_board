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

	#include <stdio.h>
 	#include <string.h>
	#include <math.h>

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

	#define DEBUG_LVL 3

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

	#if DEBUG_LVL >= 1
		#define DBG1(...)  printf(__VA_ARGS__)
	#else
		#define DBG1(...)
	#endif

	#if DEBUG_LVL >= 2
		#define DBG2(...)  printf(__VA_ARGS__)
	#else
		#define DBG2(...)
	#endif

	#if DEBUG_LVL >= 3
		#define DBG3(...)  printf(__VA_ARGS__)
	#else
		#define DBG3(...)
	#endif

	#define TM1638_STB_HIGH() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)
	#define TM1638_STB_LOW()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)

	#define TM1638_CLK_HIGH() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)
	#define TM1638_CLK_LOW()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)

	#define TM1638_DIO_READ() HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_7)
	#define TM1638_DIO_HIGH() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
	#define TM1638_DIO_LOW()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

	char debug_buffer[0xff]; // глобальний приймальний буфер
    uint32_t cnt	= 11223344;
    uint8_t led_u8	= 1;

    const uint8_t SEG_TAB[] = {
    		0b00111111,	// 0
    		0b00000110,	// 1
    		0b01011011, // 2
			0b01001111, // 3
			0b01100110, // 4
			0b01101101, // 5
			0b01111101, // 6
			0b00000111, // 7
			0b01111111, // 8
			0b01101111, // 9
			0b01110111, // a
			0b01111100, // b
			0b00111001, // c
			0b01011110, // d
			0b01111001, // e
			0b01110001, // f
			0b00000001, // ^
			0b01000000, // -
			0b00001000, // _
			0b10000000, // _point_
			0b00000000, // _blank_
    };
    const uint8_t LED_TAB[] = {
			0x00, // Off
			0xFF, // On
    };

	typedef enum {
		BRIGHT_0	=	0x88,
		BRIGHT_1	=	0x89,
		BRIGHT_2	=	0x8a,
		BRIGHT_3	=	0x8b,
		BRIGHT_4	=	0x8c,
		BRIGHT_5	=	0x8d,
		BRIGHT_6	=	0x8e,
		BRIGHT_7	=	0x8f8,
	} bright_level ;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

	int  __io_putchar		(int);	// fflush(stdout);
	void TM1638_Dio_Output	(void);
	void TM1638_Dio_Input	(void);
	void TM1638_SendByte	(uint8_t);
	void TM1638_Init		(bright_level);
	void TM1638_Clear		(void);
	void TM1638_SetDigit	(uint8_t, uint8_t, uint8_t);
	void TM1638_Set_Cipher	(uint32_t, uint8_t, uint8_t);
	int  TM1638_ReadButtons	(void);

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
	DBG1("\r\n\r\n\tTM1638 -> LED8KEYS  ( LED&KEYS development_board)\r\n");
	DBG3("\t%s, %s. \r\n" , DATE_as_str , TIME_as_str);
	DBG2("\tfor debug: PA2, UART2, 9600\r\n");

	TM1638_Init(BRIGHT_2);
	TM1638_Clear();
	DBG1("TM1638_Init - Ok.\r\n");
	HAL_Delay(500);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	int keys = TM1638_ReadButtons();
		if (keys>0) {
		DBG1("\t%d ", keys); fflush(stdout);
		uint32_t ten_pwr_u32 = pow(10, 8-keys);
		TM1638_Set_Cipher((9-keys)*ten_pwr_u32, 0, (1<<(8-keys)));
	} else {
		DBG1("\r\n%3lu) ", cnt); //fflush(stdout);
		TM1638_Set_Cipher(cnt++, 0, led_u8);
		if (led_u8 == 0 ) {
			led_u8 = 1;
		} else {
			led_u8 = led_u8<<1;
		}
	}
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	HAL_Delay(100);
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

int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}  //**************************************************************************

void TM1638_Dio_Output(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
} //**************************************************************************

void TM1638_Dio_Input(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
} //**************************************************************************

void TM1638_SendByte(uint8_t data) {
    TM1638_Dio_Output();
    for (int i = 0; i < 8; i++) {
        TM1638_CLK_LOW();
        if (data & 0x01)
            TM1638_DIO_HIGH();
        else
            TM1638_DIO_LOW();
        data >>= 1;
        TM1638_CLK_HIGH();
    }
} //**************************************************************************

void TM1638_Init(bright_level _bright_lvl) {
    TM1638_STB_LOW();
    TM1638_SendByte(_bright_lvl); // Включити дисплей + яскравість
    TM1638_STB_HIGH();
} //**************************************************************************

void TM1638_SetDigit(uint8_t pos, uint8_t value, uint8_t led) {
	TM1638_Init(BRIGHT_2); // захист від зависання індикації

    TM1638_STB_LOW();
    TM1638_SendByte(0x44);  // Адресна передача
    TM1638_STB_HIGH();

    TM1638_STB_LOW();
    TM1638_SendByte(0xC0 + pos*2); // Адреса позиції
    TM1638_SendByte(SEG_TAB[value]);
    TM1638_STB_HIGH();

    //HAL_Delay(1);

    TM1638_STB_LOW();
    TM1638_SendByte(0xC0 + pos*2 + 1); // Адреса позиції
    TM1638_SendByte(LED_TAB[led]); // Світлодіод: ON або OFF
    TM1638_STB_HIGH();

} //**************************************************************************

void TM1638_Set_Cipher	(uint32_t _cipher_u32, uint8_t _hex_flag, uint8_t _led_u8) {
	uint32_t division;
	if ( _hex_flag == 0) {
		division = 10;
	} else {
		division = 16;
	}

	uint32_t val_arr[8] = {0};
	uint8_t led_arr[8] = {0};

	for (uint8_t pos = 0; pos <8; pos++) {
		val_arr[7-pos] = _cipher_u32 % division;
		led_arr[7-pos] = (_led_u8 >> (pos)) & 1;
		_cipher_u32 = _cipher_u32 / division;
	}

	for (uint8_t pos = 0; pos <8; pos++) {
		TM1638_SetDigit(pos, val_arr[pos], led_arr[pos]);
	}
}  //**************************************************************************

void TM1638_Clear(void) {
	for (uint8_t pos = 0; pos <8; pos++) {
		TM1638_SetDigit(pos, 21, 0);
	}
} //**************************************************************************

int TM1638_ReadButtons(void) {
    uint8_t buttons = 0;

    TM1638_STB_LOW();
    TM1638_SendByte(0x42); // Команда зчитування
    TM1638_Dio_Input();\

    for (uint8_t i = 0; i < 4; i++) {
        uint8_t data = 0;
        for (uint8_t b = 0; b < 8; b++) {
            TM1638_CLK_LOW();
            if (TM1638_DIO_READ())
                data |= (1 << b);
            TM1638_CLK_HIGH();
        }

        // Розбір кожного біта
        if (data & 0x01) { buttons |= (1 << i);     } // S1..S4
        if (data & 0x10) { buttons |= buttons | (1 << (i+4)); } // S5..S8
    }

    TM1638_Dio_Output();
    TM1638_STB_HIGH();

    if (buttons > 0) {
		int btn = 0;
		for (uint8_t i = 0; i < 8; i++) {
			if (buttons & (1 << i)) {
				btn |= btn | i; // Кнопки нумеруються з 1 до 8
			}
		}
		return btn+1;
    }
    return -1;
} //**************************************************************************

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
