/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "GRI.h"																					// Прототипы функций управления выводами индикаторов
#include "Flash.h"																				// Прототипы функций работы с FLASH памятью

RTC_TimeTypeDef sTime;																		// Структура хранения времени
uint8_t SIM[] = {20, 20, 20, 20};															// Массив хранения разрядов индикаторов
uint8_t SIMs[4];																						// Вспомогательный массив хранения разрядов индикаторов
uint8_t CUR[4];																						// Массив хранения текущих разрядов индикаторов
uint8_t POS[] = {1, 0, 2, 9, 3, 8, 4, 7, 5, 6};											// Массив хранения порядка катодов в индикаторе
uint8_t MASK[] = {1,1,1,1};																	// Маска отображения разрядов индикаторов

uint8_t TYPE[4][12] = {																			// Структура хранения цоколевки индикаторов ИН-16 и ИН-14
		{11, 0, 7, 2, 4, 5, 6, 1, 9, 10, 8, 3},
		{10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 0},
		{3, 8, 10, 9, 1, 6, 5, 4, 2, 7, 0, 11},
		{0, 11, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10}
};

uint8_t VLTset=0;																					// Флаг включенного режима установки скважности ШИМ
uint8_t sel=0;																							// Счетчик изменяемого параметра в режиме установки времени
uint8_t dot;																								// Флаг индикации точки
uint8_t setup=0;																					// Флаг влюченного режима установки времени
uint32_t btn_count=0;																			// Счетчик удержания центральной кнопки
uint32_t double_count=0;																	// Счетчик удержания боковых кнопок
uint32_t valM_count=0;																		// Счетчик удержания левой кнопки
uint32_t valP_count=0;																			// Счетчик удержания правой кнопки
uint32_t type1=0;																					// Переменная хранения типа левых индикаторов
uint32_t type2=0;																					// Переменная хранения типа правых индикаторов
uint32_t VOLT;																						// Переменная хранения параметра скважности ШИМ
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
RTC_HandleTypeDef hrtc;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Измененная функция инициализации часов
void RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}
    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0x32F2);
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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim2);																													// Запуск таймера динамической индикации
  HAL_TIM_Base_Start_IT(&htim2);																												// Включение прерываний таймера динамической индикации
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);																					// Запуск генерации ШИМ сигнала высоковольтного преобразователя
  HVset(1500, VOLT);																																		// Установка параметров ШИМ сигнала

  type1 = flash_read(0x08007FF0);																												// Считывание типа левых индикаторов
  type2 = flash_read(0x08007FF4);																												// Считывание типа правых индикаторов
  VOLT = flash_read(0x08007FF8);																												// Считывание параметра скважности ШИМ

  if(type1>1) type1=0;																																	// Если считанные данные некорректны устанавливается значение по умолчанию
  if(type2>1) type2=0;																																	// Если считанные данные некорректны устанавливается значение по умолчанию
  if(VOLT<700 || VOLT>1370) VOLT=1040;																								// Если считанные данные некорректны устанавливается значение по умолчанию

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);																		// Считывание текущего значения времени

	  if(sTime.Seconds%2)																																// Переключение состояния точки и светодиода с периодом 1 сек.
	  {
		  dot=1;
	  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	  }
	  else
	  {
		  dot=0;
	  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------Режим прожига---------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_L == GPIO_PIN_RESET && BUT_R == GPIO_PIN_RESET && !setup)								// Если произошло одновременное нажатие боковых кнопок в основном режиме
	  {
		  double_count++;																																// Увеличение счетчика удержания нажатых кнопок
		  HAL_Delay(1);																																	// Задержка 1 мс

	  	  if(double_count>1000)																													// При превышении счетчика значения 1000
	  	  {
	  		  double_count=0;																															// Сброс счетчика удержания двух кнопок
			  uint8_t exit=1;																																// Флаг выхода из режима прожига
			  ResAll();																																		// Отключение всех катодов

			  for(int k=1; k<=4; k++) OPres(k);																							// Отключение всех анодов

			  HAL_TIM_Base_Stop_IT(&htim2);																								// Отключение динамической индикации
			  uint8_t OP_cur=1;																														// Переменная выбора анода
			  uint8_t TR_cur=0;																														// Переменная выбора катода
			  OPset(OP_cur);																															// Включение текущего анода
			  TRset(TYPE[type2][TR_cur]);																										// Включение катода цифры 0
			  HVset(2000, 1870);																														// Установка параметров ШИМ генератора для прожига

			  while(BUT_L == GPIO_PIN_RESET || BUT_R == GPIO_PIN_RESET);										// Ожидаем, пока не будет отпущена хотя бы одна кнопка

			  HAL_Delay(50);																															// Задержка устранения дребезга

			  while(exit)																																	// Подцикл режима прожига
			  {
				  if(BUT_L ==  GPIO_PIN_RESET)																								// Если произошло нажатие левой кнопки
				  {
					  OPres(OP_cur);																													// Отключение текущего анода
					  if(OP_cur<4) OP_cur++; else OP_cur=1;																		// Переключение индикатора
					  ResAll();																																// Отключение всех катодов

					  if(OP_cur>2)																														// Включение катода в зависимости от типа индикатора
						  TRset(TYPE[type1][TR_cur]);
					  else
						  TRset(TYPE[type2][TR_cur]);

					  OPset(OP_cur);																													// Включение текущего анода
					  while(BUT_L ==  GPIO_PIN_RESET);																				// Ожидаем, пока не будет отпущена кнопка
					  HAL_Delay(50);																													// Задержка устранения дребезга
				  }

				  if(BUT_R ==  GPIO_PIN_RESET)																							// Если произошло нажатие правой кнопки
				  {
					  ResAll();																																// Отключение всех катодов
					  if(TR_cur<11) TR_cur++; else TR_cur=0;																		// Переключение катода

					  if(OP_cur>2)																														// Включение катода в зависимости от типа индикатора
						  TRset(TYPE[type1][TR_cur]);
					  else
						  TRset(TYPE[type2][TR_cur]);

					  while(BUT_R ==  GPIO_PIN_RESET);																				// Ожидаем, пока не будет отпущена кнопка
					  HAL_Delay(50);																													// Задержка устранения дребезга
				  }

				  if(BUT_M == GPIO_PIN_RESET) exit=0;																				// При нажатии на центральную кнопку выход из подцикла
			  }

			  ResAll();																																		// Отключение всех катодов
			  for(int k=1; k<=4; k++)OPres(k);																								// Отключение всех анодов
			  HAL_TIM_Base_Start_IT(&htim2);																								// Включение динамической индикации
	  	  }
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------Режим настройки--------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_M == GPIO_PIN_RESET)																										// Если нажата средняя кнопка
	  {
		  if(btn_count!=40000) btn_count++;																							// Увеличение счетчика нажатия средней кнопки
		  else																																					// Если значение счетчика больше 40000
		  {
			  setup=!setup;																																// Переключение режима настройки времени
			  if(setup) sel=1; else sel=0;																										// Установка номера изменяемого параметра
			  btn_count++;																																// Увеличение счетчика удержания кнопки еще на еденицу
		  }
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_L == GPIO_PIN_RESET && setup)																						// Если в режиме настройки нажата левая кнопка
	  {
		  if(sel==1)																																			// Если выбран первый параметр
			  if(sTime.Hours>0) sTime.Hours -= 1; else sTime.Hours += 23;											// Уменьшение значения часов
		  else if(sel==2)																																// Если выбран второй параметр
			  if(sTime.Minutes>0) sTime.Minutes -= 1; else sTime.Minutes += 59;									// Уменьшение значения минут
		  else if(sel==3)																																// Если выбран третий параметр
			  if(sTime.Seconds>0) sTime.Seconds -= 1; else sTime.Seconds += 59;								// Уменьшение значения секунд

		  while(BUT_L == GPIO_PIN_RESET && valM_count<200)														// Пока кнопка удерживается менее 200 мс
		  {
			  valM_count++; 																															// Увеличение счетчика удержания кнопки
			  HAL_Delay(1);																																// Задержка 1 мс
		  }

		  if(valM_count>=200) 																													// Если кнопка удерживается более 200 мс
			  HAL_Delay(100);																															// Задержка 100 мс

		  if(setup) HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);												// Обновление системного времени
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_R == GPIO_PIN_RESET && setup)																						// Если в режиме настройки нажата правая кнопка
	  {
		  if(sel==1)																																			// Если выбран первый параметр
			  if(sTime.Hours<23) sTime.Hours += 1; else sTime.Hours -= 23;											// Увеличение значения часов
		  else if(sel==2)																																// Если выбран второй параметр
			  if(sTime.Minutes<59) sTime.Minutes += 1; else sTime.Minutes -= 59;								// Увеличение значения минут
		  else if(sel==3)																																// Если выбран третий параметр
			  if(sTime.Seconds<59) sTime.Seconds += 1; else sTime.Seconds -= 59;								// Увеличение значения секунд

		  while(BUT_R == GPIO_PIN_RESET && valP_count<200)															// Пока кнопка удерживается менее 200 мс
		  {
			  valP_count++;																																// Увеличение счетчика удержания кнопки
			  HAL_Delay(1);																																// Задержка 1 мс
		  }

		  if(valP_count>=200) 																														// Если кнопка удерживается более 200 мс
			  HAL_Delay(100);																															// Задержка 100 мс

		  if(setup) HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);												// Обновление системного времени
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------Режим изменения типа индикаторов---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_L== GPIO_PIN_RESET && BUT_R == GPIO_PIN_SET && !setup)									// Если в основном режиме нажата левая кнопка и отпущена правая
	  {
		  if(valM_count<500)																														// Если счетчик удержания не превышает значения 500
		  {
			  HAL_Delay(1);																																// Задержка 1 мс
			  valM_count++;																															// Увеличение счетчика удержания кнопки
		  }
		  else																																					// Если счетчик удержания превышает значение 500
		  {
			  dot=0;																																			// Отключается индикация точки
			  if(type2<1) type2++; else type2=0;																						// Переключается тип двух левых индикаторов
			  SIM[0] = 0;																																	// Отображение нуля
			  SIM[1] = 0;																																	// Отображение нуля
			  MASK[2] = 0; MASK[3] = 0;																										// Отключение индикации правых индикаторов
			  while(BUT_L == GPIO_PIN_RESET);																							// Ожидаем, пока не будет отпущена кнопка

			  HVset(1500, 0);																															// Кратковременное уменьшение скважности
			  flash_unlock();																																// Разблокировка FLASH
			  flash_erase_page(0x08007FF0);																								// Стирание страницы
			  flash_write(0x08007FF0, type1);																								// Запись в память текущий тип правых индикаторов
			  flash_write(0x08007FF4, type2);																								// Запись в память текущий тип левых индикаторов
			  flash_write(0x08007FF8, VOLT);																								// Запись текущего значения скважности ШИМ
			  flash_lock();																																	// Блокировка FLASH
		  }
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_R == GPIO_PIN_RESET && BUT_L == GPIO_PIN_SET && !setup)									// Если в основном режиме нажата правая кнопка и отпущена левая
	  {
		  if(valP_count<500)																															// Если счетчик удержания не превышает значения 500
		  {
			  HAL_Delay(1);																																// Задержка 1 мс
			  valP_count++;																																// Увеличение счетчика удержания кнопки
		  }
		  else																																					// Если счетчик удержания превышает значение 500
		  {
			  dot=0;																																			// Отключается индикация точки
			  if(type1<1) type1++; else type1=0;																						// Переключается тип двух правых индикаторов
			  SIM[2] = 0;																																	// Отображение нуля
			  SIM[3] = 0;																																	// Отображение нуля
			  MASK[0] = 0; MASK[1] = 0;																										// Отключение индикации левых индикаторов
			  while(BUT_R == GPIO_PIN_RESET); 																						// Ожидаем, пока не будет отпущена кнопка

			  HVset(1500, 0);																															// Кратковременное уменьшение скважности
			  flash_unlock();																																// Разблокировка FLASH
			  flash_erase_page(0x08007FF0);																								// Стирание страницы
			  flash_write(0x08007FF0, type1);																								// Запись в память текущий тип правых индикаторов
			  flash_write(0x08007FF4, type2);																								// Запись в память текущий тип левых индикаторов
			  flash_write(0x08007FF8, VOLT);																								// Запись текущего значения скважности ШИМ
			  flash_lock();																																	// Блокировка FLASH
		  }
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------Обработка отпускания кнопок------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_M == GPIO_PIN_SET)																												// Если отпущена средняя кнопка
	  {
		  if(btn_count<40000 && btn_count>0 && setup)																		// Если была кратковремено нажата средняя кнопка в режиме установки времени
			  if(sel<3)sel++; else sel=1;																										// Переключение номера изменяемого параметра

		  	  btn_count=0;																																// Сброс значения счетчика удержания кнопки
	  	 }

	  if(BUT_L == GPIO_PIN_SET)																												// Если отпущена левая кнопка
	  {
		  valM_count=0;																																	// Сброс счетчика удержания кнопки
		  if(double_count) VLTset=1;																											// Если были кратковременно нажаты крайние кнопки
		  double_count=0;																																// Сброс счетчика удержания двух кнопок
	  }

	  if(BUT_R == GPIO_PIN_SET)																												// Если отпущена правая кнопка
	  {
		  valP_count=0;																																	// Сброс счетчика удержания кнопки
		  if(double_count) VLTset=1;																											// Если были кратковременно нажаты крайние кнопки
		  double_count=0;																																// Сброс счетчика удержания двух кнопок
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------Режимы отображения времени------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(sel==0)																																				// Если выбран основной режим отображения времени
	  {
		  CUR[3] = sTime.Minutes%10;																										// Текущие единицы минут
		  CUR[2] = sTime.Minutes/10;																											// Текущие десятки минут
		  CUR[1] = sTime.Hours%10;																											// Текущие единицы часов
		  CUR[0] = sTime.Hours/10;																												// Текущие десятки минут

		  MASK[1] = 1;																																	// Отображать соответствующий разряд
		  MASK[0] = 1;																																	// Отображать соответствующий разряд
		  MASK[3] = 1;																																	// Отображать соответствующий разряд
		  MASK[2] = 1;																																	// Отображать соответствующий разряд

		  	  	  	  	  	  	  	  	  	  	  	  //------Антиотравление-----//

		  if(CUR[0]!=SIMs[0] || CUR[1]!=SIMs[1] || CUR[2]!=SIMs[2] || CUR[3]!=SIMs[3] )					// Если значение одного из индикаторов отличается от текущего времени
		  {
			  dot=0;																																			// Отключение индикации точки
			  for(int i=0; i<4; i++)																													// Цикл для копирования
				  SIMs[i] = SIM[i];																														// Копирование значений индикаторов во вспомогателный массив

			  for(int j=0; j<10; j++)																												// Цикл перебора для антиотравления
			  {
				  for(int i=0; i<4; i++)																												// Перебор четырех индикаторов
					  if(CUR[i]!=SIMs[i]) 																												// Если текущее значение разряда не совпадает со значением из вспомогательного массива
						  SIM[i] = POS[j];																												// Устанавливается значение через массив расположения цифр внутри индикатора
				  HAL_Delay(50);																														// Задержка 50 мс
			  }
		  }
		  for(int i=0; i<4; i++)																														// Цикл для копирования
			  SIM[i] = CUR[i];																															// Установка текущих значений
	  }
	  else if(sel<3)																																		// Если выбран режим настройки часов или минут
	  {
		  SIM[3] = sTime.Minutes%10;																											// Разряд единиц минут
		  SIM[2] = sTime.Minutes/10;																											// Разряд десятков минут
		  SIM[1] = sTime.Hours%10;																											// Разряд единиц минут
		  SIM[0] = sTime.Hours/10;																												// Разряд десятков минут
		  MASK[1] = 1;																																	// Отображать соответствующий разряд
		  MASK[0] = 1;																																	// Отображать соответствующий разряд
	  }
	  else																																						// Если выбран режим настройки секунд
	  {
		  SIM[3] = sTime.Seconds%10;																										// Разряд единиц секунд
		  SIM[2] = sTime.Seconds/10;																											// Разряд десятков секунд
		  MASK[1] = 0;																																	// Не отображать соответствующий разряд
		  MASK[0] = 0;																																	// Не отображать соответствующий разряд
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------Режим настройки скважности ШИМ----------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  while(VLTset)
	  {
		  uint16_t tmp_cnt;																															// Локальный счетчик удержания кнопок
		  dot=0;																																				// Отключается индикация точки

		  SIM[0] = VOLT/1000;																														// Разряд тысяч
		  SIM[1] = VOLT%1000/100;																											// Разряд сотен
		  SIM[2] = VOLT%100/10;																												// Разряд десятков
		  SIM[3] = VOLT%10;																														// Разряд единиц

		  if(BUT_M == GPIO_PIN_RESET)																									// Если нажата средняя кнопка
		  {
			  VLTset=0;																																		// Выход из режима настройки ШИМ
			  HVset(1500, 0);																															// Кратковременное уменьшение скважности
			  flash_unlock();																																// Разблокировка FLASH
			  flash_erase_page(0x08007FF0);																								// Стирание страницы
			  flash_write(0x08007FF0, type1);																								// Запись в память текущий тип правых индикаторов
			  flash_write(0x08007FF4, type2);																								// Запись в память текущий тип левых индикаторов
			  flash_write(0x08007FF8, VOLT);																								// Запись текущего значения скважности ШИМ
			  flash_lock();																																	// Блокировка FLASH
		  }

		  if(BUT_L == GPIO_PIN_RESET)																										// Если нажата левая кнопка
		  {
			  HAL_Delay(10);																															// Задержка 10 мс
			  if(VOLT>700) VOLT--;																												// Уменьшение значения скважности до порогового
			  tmp_cnt=0;																																	// Обнуление счетчика удержания кнопок
			  while(BUT_L == GPIO_PIN_RESET)																							// Пока левая кнопка нажата
			  {
				  HAL_Delay(10);																														// Задержка 10 мс
				  if(tmp_cnt<70) tmp_cnt++; else if(VOLT>700) VOLT--;													// Обработка автоматического уменьшения значения при удержании левой кнопки
				  SIM[0] = VOLT/1000;																												// Разряд тысяч
				  SIM[1] = VOLT%1000/100;																									// Разряд сотен
				  SIM[2] = VOLT%100/10;																										// Разряд десятков
				  SIM[3] = VOLT%10;																												// Разряд единиц
			  }
		  }

		  if(BUT_R == GPIO_PIN_RESET)																										// Если нажата правая кнопка
		  {
			  HAL_Delay(10);																															// Задержка 10 мс
			  if(VOLT<1370) VOLT++;																											// Увеличение значения скважности до порогового
			  tmp_cnt=0;																																	// Обнуление счетчика удержания кнопок
			  while(BUT_R == GPIO_PIN_RESET)																							// Пока правая кнопка нажата
			  {
				  HAL_Delay(10);																														// Задержка 10 мс
				  if(tmp_cnt<70) tmp_cnt++; else if(VOLT<1370) VOLT++;												// Обработка автоматического увеличения значения при удержании правой кнопки
				  SIM[0] = VOLT/1000;																												// Разряд тысяч
				  SIM[1] = VOLT%1000/100;																									// Разряд сотен
				  SIM[2] = VOLT%100/10;																										// Разряд десятков
				  SIM[3] = VOLT%10;																												// Разряд единиц
			  }
		  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /**Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 35;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 3000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1500;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8 
                          |GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 
                           PA4 PA5 PA6 PA8 
                           PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8 
                          |GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 PB12 PB13 
                           PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
