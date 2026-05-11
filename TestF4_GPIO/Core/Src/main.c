/* USER CODE BEGIN Header */

/**

******************************************************************************

*@file: main. c

*@brief: Кнопка на PA8 управляет светодиодом на PC13

*@author: Илон Маск — в молодости

*@version: 1.0

Подобные вещи привыкайте писать, это будет удобно просматривать спустя месяцы после работы с проектом, когда нюансы всегда азбываются.

******************************************************************************

*/

/* USER CODE END Header */

#include "main.h"

/* Объявления пользовательских функций */
static void MX_GPIO_Init (void); // Описана функция ниже

int main (void)

{

/* 1. Сброс всех периферийных устройств, инициализация Flash, SysTick */
HAL_Init (); //инициализация библиотеки HAL

/* 2. Настройка системного таймера (SysTick) */
SystemClock_Config ();
// функция генерируется CubeMX и описана ниже



/* 3. Инициализация всех периферийных устройств (включая GPIO) */
MX_GPIO_Init ();

/* 4. Бесконечный цикл */
while (1)

{
/* Считываем состояние кнопки на PA8 */
GPIO_PinState buttonState = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_8);


/* Управление светодиодом на PC13.
Светодиод подключен катодом к PC13, анодом к питанию (+3.3В). Логика: чтобы светодиод светился, на PC13 должно быть LOW (0В), чтобы погас — HIGH (3.3В). Кнопка подтянута к питанию: при нажатии (PA8 = LOW) светодиод светится — такая логика программы. Писать не обязательно, но понимать необходимо до всех деталей.
*/

// кнопка нажата (LOW на PA8)
if (buttonState == GPIO_PIN_RESET)

{

// LOW светодиод светится
HAL_GPIO_WritePin (GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);}
else // кнопка не нажата (HIGH на PA8)

{

	// HIGH светодиод не светится
	HAL_GPIO_WritePin (GPIOC, GPIO_PIN_13, GPIO_PIN_SET);}
	/* Небольшая задержка для подавления дребезга контактов (50 мс) — хотя тема подавления дребезга заслуживает отдельной главы */
	HAL_Delay (50);}
}

/**
* @brief Инициализация GPIO (конфигурация выводов)
* @param None
* @retval None
*/

static void MX_GPIO_Init (void)

{

GPIO_InitTypeDef GPIO_InitStruct = {0};

/* Включаем тактирование портов GPIOA и GPIOC */
__HAL_RCC_GPIOA_CLK_ENABLE ();
__HAL_RCC_GPIOC_CLK_ENABLE ();

/* — — — Настройка кнопки на PA8 как входа — — — */

GPIO_InitStruct. Pin = GPIO_PIN_8;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // режим входа
GPIO_InitStruct. Pull = GPIO_NOPULL; /* Внешняя подтяжка к питанию, внутреннюю подтяжку отключаем */
GPIO_InitStruct. Speed = GPIO_SPEED_FREQ_LOW;

HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

/* — — — Настройка светодиода на PC13 как выхода (push-pull) — — — */

GPIO_InitStruct. Pin = GPIO_PIN_13;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

// режим выхода push-pull
GPIO_InitStruct. Pull = GPIO_NOPULL; // подтяжка не требуется
GPIO_InitStruct. Speed = GPIO_SPEED_FREQ_LOW;

HAL_GPIO_Init (GPIOC, &GPIO_InitStruct);

/* Начальное состояние: светодиод выключен (HIGH на PC13) */
HAL_GPIO_WritePin (GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

/**

* @brief Системная тактовая конфигурация (пример для STM32F411CE, 100 МГц)

*/
void SystemClock_Config (void)

{

RCC_OscInitTypeDef RCC_OscInitStruct = {0};
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

/* Configure the main internal regulator output voltage
*/
__HAL_RCC_PWR_CLK_ENABLE();
__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

/* Настройка HSE (внешнего кварца) или HSI (внутреннего) */
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
RCC_OscInitStruct.HSEState = RCC_HSE_ON;
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
RCC_OscInitStruct.PLL.PLLM = 12;
RCC_OscInitStruct.PLL.PLLN = 96;
RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
RCC_OscInitStruct.PLL.PLLQ = 4;

/* Настройка шин */

RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_3);

}
