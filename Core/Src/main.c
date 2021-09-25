/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// my sample for OLED SSD1306 unit 128*64 or 128*32

#define OLED_HEIGHT 32
// #define OLED_HEIGHT 64

#define OLED_SETCONTRAST 0x81
#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5
#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7
#define OLED_DISPLAYOFF 0xAE
#define OLED_DISPLAYON 0xAF
#define OLED_SETDISPLAYOFFSET 0xD3
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9
#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10
#define OLED_SETSTARTLINE 0x40
#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22
#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8
#define OLED_SEGREMAP 0xA0
#define OLED_CHARGEPUMP 0x8D

///////////////////////////////////////////////////////////////////////////////

static const char OLED_characters[] = { 0x00, 0x00, 0x00, 0x00, 0x00, //    (Space)
		0x00, 0x00, 0x5F, 0x00, 0x00, //    !
		0x00, 0x07, 0x00, 0x07, 0x00, //    "
		0x14, 0x7F, 0x14, 0x7F, 0x14, //    #
		0x24, 0x2A, 0x7F, 0x2A, 0x12, //    $
		0x23, 0x13, 0x08, 0x64, 0x62, //    %
		0x36, 0x49, 0x56, 0x20, 0x50, //    &
		0x00, 0x08, 0x07, 0x03, 0x00, //    '
		0x00, 0x1C, 0x22, 0x41, 0x00, //    (
		0x00, 0x41, 0x22, 0x1C, 0x00, //    )
		0x2A, 0x1C, 0x7F, 0x1C, 0x2A, //    *
		0x08, 0x08, 0x3E, 0x08, 0x08, //    +
		0x00, 0x00, 0x70, 0x30, 0x00, //    ,
		0x08, 0x08, 0x08, 0x08, 0x08, //    -
		0x00, 0x00, 0x60, 0x60, 0x00, //    .
		0x20, 0x10, 0x08, 0x04, 0x02, //    /
		0x3E, 0x51, 0x49, 0x45, 0x3E, //    0
		0x00, 0x42, 0x7F, 0x40, 0x00, //    1
		0x72, 0x49, 0x49, 0x49, 0x46, //    2
		0x21, 0x41, 0x49, 0x4D, 0x33, //    3
		0x18, 0x14, 0x12, 0x7F, 0x10, //    4
		0x27, 0x45, 0x45, 0x45, 0x39, //    5
		0x3C, 0x4A, 0x49, 0x49, 0x31, //    6
		0x41, 0x21, 0x11, 0x09, 0x07, //    7
		0x36, 0x49, 0x49, 0x49, 0x36, //    8
		0x46, 0x49, 0x49, 0x29, 0x1E, //    9
		0x00, 0x00, 0x14, 0x00, 0x00, //    :
		0x00, 0x40, 0x34, 0x00, 0x00, //    ;
		0x00, 0x08, 0x14, 0x22, 0x41, //    <
		0x14, 0x14, 0x14, 0x14, 0x14, //    =
		0x00, 0x41, 0x22, 0x14, 0x08, //    >
		0x02, 0x01, 0x59, 0x09, 0x06, //    ?
		0x3E, 0x41, 0x5D, 0x59, 0x4E, //    @
		0x7C, 0x12, 0x11, 0x12, 0x7C, //    A
		0x7F, 0x49, 0x49, 0x49, 0x36, //    B
		0x3E, 0x41, 0x41, 0x41, 0x22, //    C
		0x7F, 0x41, 0x41, 0x41, 0x3E, //    D
		0x7F, 0x49, 0x49, 0x49, 0x41, //    E
		0x7F, 0x09, 0x09, 0x09, 0x01, //    F
		0x3E, 0x41, 0x41, 0x51, 0x73, //    G
		0x7F, 0x08, 0x08, 0x08, 0x7F, //    H
		0x00, 0x41, 0x7F, 0x41, 0x00, //    I
		0x20, 0x40, 0x41, 0x3F, 0x01, //    J
		0x7F, 0x08, 0x14, 0x22, 0x41, //    K
		0x7F, 0x40, 0x40, 0x40, 0x40, //    L
		0x7F, 0x02, 0x1C, 0x02, 0x7F, //    M
		0x7F, 0x04, 0x08, 0x10, 0x7F, //    N
		0x3E, 0x41, 0x41, 0x41, 0x3E, //    O
		0x7F, 0x09, 0x09, 0x09, 0x06, //    P
		0x3E, 0x41, 0x51, 0x21, 0x5E, //    Q
		0x7F, 0x09, 0x19, 0x29, 0x46, //    R
		0x26, 0x49, 0x49, 0x49, 0x32, //    S
		0x03, 0x01, 0x7F, 0x01, 0x03, //    T
		0x3F, 0x40, 0x40, 0x40, 0x3F, //    U
		0x1F, 0x20, 0x40, 0x20, 0x1F, //    V
		0x3F, 0x40, 0x38, 0x40, 0x3F, //    W
		0x63, 0x14, 0x08, 0x14, 0x63, //    X
		0x03, 0x04, 0x78, 0x04, 0x03, //    Y
		0x61, 0x59, 0x49, 0x4D, 0x43, //    Z
		0x00, 0x7F, 0x41, 0x41, 0x41, //    [
		0x02, 0x04, 0x08, 0x10, 0x20, //    "\"
		0x00, 0x41, 0x41, 0x41, 0x7F, //    ]
		0x04, 0x02, 0x01, 0x02, 0x04, //    ^
		0x40, 0x40, 0x40, 0x40, 0x40, //    _
		0x00, 0x03, 0x07, 0x08, 0x00, //    `
		0x20, 0x54, 0x54, 0x38, 0x40, //    a
		0x7F, 0x28, 0x44, 0x44, 0x38, //    b
		0x38, 0x44, 0x44, 0x44, 0x28, //    c
		0x38, 0x44, 0x44, 0x28, 0x7F, //    d
		0x38, 0x54, 0x54, 0x54, 0x18, //    e
		0x00, 0x08, 0x7E, 0x09, 0x02, //    f
		0x0C, 0x52, 0x52, 0x4A, 0x3C, //    g
		0x7F, 0x08, 0x04, 0x04, 0x78, //    h
		0x00, 0x44, 0x7D, 0x40, 0x00, //    i
		0x20, 0x40, 0x40, 0x3D, 0x00, //    j
		0x7F, 0x10, 0x28, 0x44, 0x00, //    k
		0x00, 0x41, 0x7F, 0x40, 0x00, //    l
		0x7C, 0x04, 0x78, 0x04, 0x78, //    m
		0x7C, 0x08, 0x04, 0x04, 0x78, //    n
		0x38, 0x44, 0x44, 0x44, 0x38, //    o
		0x7C, 0x18, 0x24, 0x24, 0x18, //    p
		0x18, 0x24, 0x24, 0x18, 0x7C, //    q
		0x7C, 0x08, 0x04, 0x04, 0x08, //    r
		0x48, 0x54, 0x54, 0x54, 0x24, //    s
		0x04, 0x04, 0x3F, 0x44, 0x24, //    t
		0x3C, 0x40, 0x40, 0x20, 0x7C, //    u
		0x1C, 0x20, 0x40, 0x20, 0x1C, //    v
		0x3C, 0x40, 0x30, 0x40, 0x3C, //    w
		0x44, 0x28, 0x10, 0x28, 0x44, //    x
		0x4C, 0x50, 0x50, 0x50, 0x3C, //    y
		0x44, 0x64, 0x54, 0x4C, 0x44, //    z
		0x00, 0x08, 0x36, 0x41, 0x00, //    {
		0x00, 0x00, 0x77, 0x00, 0x00, //    |
		0x00, 0x41, 0x36, 0x08, 0x00, //    }
		0x02, 0x01, 0x02, 0x04, 0x02, //    ~
		};

char _address = 0x78;
// char _address = 0x3c; // for Arduino

char I2C_DATA[2]; // max character width 5dot + data

void OLED_command(char command) {
	I2C_DATA[0] = 0;
	I2C_DATA[1] = command;
	HAL_I2C_Master_Transmit(&hi2c1, _address, (char*) I2C_DATA, 2, 300);
}

void OLED_data(char data) {
	I2C_DATA[0] = 0x40;
	I2C_DATA[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, _address, (char*) I2C_DATA, 2, 300);
}

void OLED_Init() {
	OLED_command(OLED_DISPLAYOFF);         // 0xAE
	OLED_command(OLED_SETDISPLAYCLOCKDIV); // 0xD5
	OLED_command(0x80);                    // the suggested ratio 0x80
	OLED_command(OLED_SETMULTIPLEX);       // 0xA8

#if OLED_HEIGHT == 32
	OLED_command(0x1F);
#elif OLED_HEIGHT == 64
	OLED_command(0x7F);
	#endif

	OLED_command(OLED_SETDISPLAYOFFSET);   // 0xD3
	OLED_command(0x0);                     // no offset
	OLED_command(OLED_SETSTARTLINE | 0x0); // line #0
	OLED_command(OLED_CHARGEPUMP);         // 0x8D
	OLED_command(0xAF);
	OLED_command(OLED_MEMORYMODE); // 0x20
	OLED_command(0x00);            // 0x0 act like ks0108
	OLED_command(OLED_SEGREMAP | 0x1);
	OLED_command(OLED_COMSCANDEC);
	OLED_command(OLED_SETCOMPINS); // 0xDA

#if OLED_HEIGHT == 32
	OLED_command(0x02);
#elif OLED_HEIGHT == 64
OLED_command(0x12);
#endif

	OLED_command(OLED_SETCONTRAST); // 0x81

#if OLED_HEIGHT == 32
	OLED_command(0x8F);
#elif OLED_HEIGHT == 64
	OLED_command(0x9F);
#endif

	OLED_command(OLED_SETPRECHARGE); // 0xd9
	OLED_command(0xF1);
	OLED_command(OLED_SETVCOMDETECT); // 0xDB
	OLED_command(0x40);
	OLED_command(OLED_DISPLAYALLON_RESUME); // 0xA4
	OLED_command(OLED_NORMALDISPLAY);       // 0xA6
	OLED_command(OLED_DISPLAYON);           //--turn on oled panel
}

void OLED_write_bit(char ch, int x, int y) {
	OLED_command(0x20);            //
	OLED_command(0x02);            // Page addressing mode
	OLED_command(y + 0xB0);        // Set page start addres
	OLED_command(x & 0x0F);        // Set lower column start address
	OLED_command(0x10 + (x >> 4)); // Set higher column start address

	OLED_data(ch);
}

void OLED_clear() {
	uint8_t x = 0;
	uint8_t y = 0;
	for (x = 0; x < 128; x++) {
		for (y = 0; y < OLED_HEIGHT / 8; y++) {
			OLED_write_bit(0, x, y);
		}
	}
}

// x: 0 to 127
// y: 0 to 3 or 7

void OLED_char6(char ch, int x, int y) {

	int table_offset = (ch - ' ') * 5;

	for (int i = 0; i < 5; i++) {
		OLED_write_bit(OLED_characters[i + table_offset], x + i, y);
	}

}

void OLED_string6(char *str, int x, int y) {

	int pos = 0;
	int startx = x;
	int starty = y;
	while (str[pos] != '\0') {
		OLED_char6(str[pos], startx, starty);
		if (startx >= 123) {
			starty++;
			startx = 0;
		} //wrap around
		startx += 6;
		pos += 1;
	}
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	OLED_Init();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		OLED_clear();

		for (int i = 0; i < 3; i++) {
			HAL_GPIO_WritePin(PB3_GPIO_Port, PB3_Pin, 1);
			HAL_Delay(100);
			HAL_GPIO_WritePin(PB3_GPIO_Port, PB3_Pin, 0);
			HAL_Delay(100);
		}

		//           12345678901234567890
		OLED_string6("   Booting...", 0, 0);
		HAL_Delay(2000);

		OLED_string6("    Nucleo", 0, 2);
		OLED_string6("      by HAL.", 0, 3);
		HAL_Delay(1000);

#if OLED_HEIGHT == 64
//            1234567890123456789
OLED_string6(" Mac 12", 0, 5);

HAL_Delay(1000);
#endif

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00602173;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 38400;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(PB3_GPIO_Port, PB3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : PB3_Pin */
	GPIO_InitStruct.Pin = PB3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(PB3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
