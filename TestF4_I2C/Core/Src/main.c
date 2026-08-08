/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Пример работы с DS1307Z+ (RTC) по I2C1 и выводом в UART1
  * @author         : Автор
  * @board          : Red Kit (STM32F411CE)
  * @peripherals    : I2C1 (PB8 - SDA, PB9 - SCL), USART1 (PB6 - TX, PB7 - RX)
  ******************************************************************************
  В данном листинге представлен полный проект для микроконтроллера
  STM32F411CE (плата Red Kit), демонстрирующий работу с внешними
  часами реального времени DS1307Z+ по интерфейсу I2C.
  Данные считываются каждую секунду и выводятся в UART-терминал.
  Код использует HAL-библиотеку, содержит функции преобразования
   BCD-формата и корректную обработку ошибок,
   включая проверку инициализации UART перед выводом критических сообщений.
  */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// Структура для хранения времени и даты DS1307
typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t dayOfWeek;   // 1-7 (Sunday = 1)
    uint8_t dayOfMonth;  // 1-31
    uint8_t month;       // 1-12
    uint8_t year;        // 0-99
} RTC_TimeTypeDef;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DS1307_ADDR         0x68  // 7-битный адрес DS1307
#define DS1307_ADDR_WRITE   (DS1307_ADDR << 1)      // 0xD0
#define DS1307_ADDR_READ    ((DS1307_ADDR << 1) | 1) // 0xD1

// Регистры DS1307
#define DS1307_REG_SECONDS      0x00
#define DS1307_REG_MINUTES      0x01
#define DS1307_REG_HOURS        0x02
#define DS1307_REG_DAYOFWEEK    0x03
#define DS1307_REG_DAYOFMONTH   0x04
#define DS1307_REG_MONTH        0x05
#define DS1307_REG_YEAR         0x06
#define DS1307_REG_CONTROL      0x07

// Бит CH (Clock Halt) в регистре секунд
#define DS1307_BIT_CH           7

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef currentTime;
char displayBuffer[64];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void DS1307_Init(void);
uint8_t DS1307_ReadByte(uint8_t reg);
void DS1307_WriteByte(uint8_t reg, uint8_t data);
void DS1307_ReadTime(RTC_TimeTypeDef *time);
void DS1307_SetTime(RTC_TimeTypeDef *time);
void DS1307_PrintTime(void);
uint8_t BCD_To_Dec(uint8_t bcd);
uint8_t Dec_To_BCD(uint8_t dec);
void Error_Handler(void);
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
  RTC_TimeTypeDef initialTime = {
      .seconds = 0,
      .minutes = 0,
      .hours   = 12,
      .dayOfWeek = 6,   // Saturday
      .dayOfMonth = 8,
      .month   = 8,
      .year    = 26
  };
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
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  printf("\n\r=== DS1307 RTC Demo ===\n\r");
  printf("Initialization of DS1307...\n\r");

  DS1307_Init();

  //Пример установки времени
  printf("Set Time Type: 12:00:00 08/08/2026\n\r");
  DS1307_SetTime(&initialTime);

  printf("Done. Time reading every 1 second:\n\r");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    DS1307_ReadTime(&currentTime);
    DS1307_PrintTime();

    HAL_Delay(1000);
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief Преобразование BCD в десятичное число
  */
uint8_t BCD_To_Dec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

/**
  * @brief Преобразование десятичного числа в BCD
  */
uint8_t Dec_To_BCD(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

/**
  * @brief Инициализация DS1307: проверка наличия и запуск генератора
  */
void DS1307_Init(void) {
    // Проверяем доступность устройства
    if (HAL_I2C_IsDeviceReady(&hi2c1, DS1307_ADDR_WRITE, 3, 100) != HAL_OK) {
        printf("ERROR: DS1307 is not detected on the I2C bus!\n\r");
        Error_Handler();
    }

    // Читаем регистр секунд и сбрасываем бит CH (Clock Halt)
    uint8_t seconds = DS1307_ReadByte(DS1307_REG_SECONDS);
    if (seconds & (1 << DS1307_BIT_CH)) {
        DS1307_WriteByte(DS1307_REG_SECONDS, seconds & ~(1 << DS1307_BIT_CH));
        printf("DS1307 is running (the CH bit is reset)\n\r");
    } else {
        printf("DS1307 is already working\n\r");
    }
}

/**
  * @brief Чтение одного байта из регистра DS1307
  */
uint8_t DS1307_ReadByte(uint8_t reg) {
    uint8_t data = 0;
    HAL_I2C_Mem_Read(&hi2c1, DS1307_ADDR_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return data;
}

/**
  * @brief Запись одного байта в регистр DS1307
  */
void DS1307_WriteByte(uint8_t reg, uint8_t data) {
    HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDR_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

/**
  * @brief Чтение полного времени из DS1307
  */
void DS1307_ReadTime(RTC_TimeTypeDef *time) {
    uint8_t data[7];

    // Последовательное чтение 7 байт с регистра 0x00
    HAL_I2C_Mem_Read(&hi2c1, DS1307_ADDR_WRITE, DS1307_REG_SECONDS,
                     I2C_MEMADD_SIZE_8BIT, data, 7, 100);

    // Преобразование BCD в десятичные значения
    time->seconds = BCD_To_Dec(data[0] & 0x7F);  // Бит CH исключаем
    time->minutes = BCD_To_Dec(data[1]);
    time->hours   = BCD_To_Dec(data[2] & 0x3F);  // 24-часовой режим
    time->dayOfWeek = BCD_To_Dec(data[3]);
    time->dayOfMonth = BCD_To_Dec(data[4]);
    time->month = BCD_To_Dec(data[5]);
    time->year  = BCD_To_Dec(data[6]);
}

/**
  * @brief Установка времени в DS1307
  */
void DS1307_SetTime(RTC_TimeTypeDef *time) {
    uint8_t data[7];

    data[0] = Dec_To_BCD(time->seconds);
    data[1] = Dec_To_BCD(time->minutes);
    data[2] = Dec_To_BCD(time->hours);
    data[3] = Dec_To_BCD(time->dayOfWeek);
    data[4] = Dec_To_BCD(time->dayOfMonth);
    data[5] = Dec_To_BCD(time->month);
    data[6] = Dec_To_BCD(time->year);

    // Последовательная запись 7 байт начиная с регистра 0x00
    HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDR_WRITE, DS1307_REG_SECONDS,
                      I2C_MEMADD_SIZE_8BIT, data, 7, 100);
}

/**
  * @brief Вывод времени в UART
  */
void DS1307_PrintTime(void) {
    sprintf(displayBuffer, "Time: %02d:%02d:%02d  Date: %02d/%02d/20%02d  Day: %d\n\r",
            currentTime.hours, currentTime.minutes, currentTime.seconds,
            currentTime.dayOfMonth, currentTime.month, currentTime.year,
            currentTime.dayOfWeek);

    printf("%s", displayBuffer);
}

/**
  * @brief Переопределение _write для перенаправления printf в UART1
  */
int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	// Check if UART is initialized before using printf
	if (huart1.Instance != NULL) {
	      if (huart1.Instance->CR1 & USART_CR1_UE) {
	          printf("!!! Critical ERROR !!!\n\r");
	      }
	  }
	__disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
