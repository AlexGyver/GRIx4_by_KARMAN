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

#include "GRI.h"																					// ��������� ������� ���������� �������� �����������
#include "Flash.h"																				// ��������� ������� ������ � FLASH �������

RTC_TimeTypeDef sTime;																		// ��������� �������� �������
uint8_t SIM[] = {20, 20, 20, 20};															// ������ �������� �������� �����������
uint8_t SIMs[4];																						// ��������������� ������ �������� �������� �����������
uint8_t CUR[4];																						// ������ �������� ������� �������� �����������
uint8_t POS[] = {1, 0, 2, 9, 3, 8, 4, 7, 5, 6};											// ������ �������� ������� ������� � ����������
uint8_t MASK[] = {1,1,1,1};																	// ����� ����������� �������� �����������

uint8_t TYPE[4][12] = {																			// ��������� �������� ��������� ����������� ��-16 � ��-14
		{11, 0, 7, 2, 4, 5, 6, 1, 9, 10, 8, 3},
		{10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 0},
		{3, 8, 10, 9, 1, 6, 5, 4, 2, 7, 0, 11},
		{0, 11, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10}
};

uint8_t VLTset=0;																					// ���� ����������� ������ ��������� ���������� ���
uint8_t sel=0;																							// ������� ����������� ��������� � ������ ��������� �������
uint8_t dot;																								// ���� ��������� �����
uint8_t setup=0;																					// ���� ���������� ������ ��������� �������
uint32_t btn_count=0;																			// ������� ��������� ����������� ������
uint32_t double_count=0;																	// ������� ��������� ������� ������
uint32_t valM_count=0;																		// ������� ��������� ����� ������
uint32_t valP_count=0;																			// ������� ��������� ������ ������
uint32_t type1=0;																					// ���������� �������� ���� ����� �����������
uint32_t type2=0;																					// ���������� �������� ���� ������ �����������
uint32_t VOLT;																						// ���������� �������� ��������� ���������� ���
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

// ���������� ������� ������������� �����
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

  HAL_TIM_Base_Start(&htim2);																													// ������ ������� ������������ ���������
  HAL_TIM_Base_Start_IT(&htim2);																												// ��������� ���������� ������� ������������ ���������
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);																					// ������ ��������� ��� ������� ��������������� ���������������
  HVset(1500, VOLT);																																		// ��������� ���������� ��� �������

  type1 = flash_read(0x08007FF0);																												// ���������� ���� ����� �����������
  type2 = flash_read(0x08007FF4);																												// ���������� ���� ������ �����������
  VOLT = flash_read(0x08007FF8);																												// ���������� ��������� ���������� ���

  if(type1>1) type1=0;																																	// ���� ��������� ������ ����������� ��������������� �������� �� ���������
  if(type2>1) type2=0;																																	// ���� ��������� ������ ����������� ��������������� �������� �� ���������
  if(VOLT<700 || VOLT>1370) VOLT=1040;																								// ���� ��������� ������ ����������� ��������������� �������� �� ���������

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);																		// ���������� �������� �������� �������

	  if(sTime.Seconds%2)																																// ������������ ��������� ����� � ���������� � �������� 1 ���.
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
//-----------------------------------------------------------------------------------------------����� �������---------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_L == GPIO_PIN_RESET && BUT_R == GPIO_PIN_RESET && !setup)								// ���� ��������� ������������� ������� ������� ������ � �������� ������
	  {
		  double_count++;																																// ���������� �������� ��������� ������� ������
		  HAL_Delay(1);																																	// �������� 1 ��

	  	  if(double_count>1000)																													// ��� ���������� �������� �������� 1000
	  	  {
	  		  double_count=0;																															// ����� �������� ��������� ���� ������
			  uint8_t exit=1;																																// ���� ������ �� ������ �������
			  ResAll();																																		// ���������� ���� �������

			  for(int k=1; k<=4; k++) OPres(k);																							// ���������� ���� ������

			  HAL_TIM_Base_Stop_IT(&htim2);																								// ���������� ������������ ���������
			  uint8_t OP_cur=1;																														// ���������� ������ �����
			  uint8_t TR_cur=0;																														// ���������� ������ ������
			  OPset(OP_cur);																															// ��������� �������� �����
			  TRset(TYPE[type2][TR_cur]);																										// ��������� ������ ����� 0
			  HVset(2000, 1870);																														// ��������� ���������� ��� ���������� ��� �������

			  while(BUT_L == GPIO_PIN_RESET || BUT_R == GPIO_PIN_RESET);										// �������, ���� �� ����� �������� ���� �� ���� ������

			  HAL_Delay(50);																															// �������� ���������� ��������

			  while(exit)																																	// ������� ������ �������
			  {
				  if(BUT_L ==  GPIO_PIN_RESET)																								// ���� ��������� ������� ����� ������
				  {
					  OPres(OP_cur);																													// ���������� �������� �����
					  if(OP_cur<4) OP_cur++; else OP_cur=1;																		// ������������ ����������
					  ResAll();																																// ���������� ���� �������

					  if(OP_cur>2)																														// ��������� ������ � ����������� �� ���� ����������
						  TRset(TYPE[type1][TR_cur]);
					  else
						  TRset(TYPE[type2][TR_cur]);

					  OPset(OP_cur);																													// ��������� �������� �����
					  while(BUT_L ==  GPIO_PIN_RESET);																				// �������, ���� �� ����� �������� ������
					  HAL_Delay(50);																													// �������� ���������� ��������
				  }

				  if(BUT_R ==  GPIO_PIN_RESET)																							// ���� ��������� ������� ������ ������
				  {
					  ResAll();																																// ���������� ���� �������
					  if(TR_cur<11) TR_cur++; else TR_cur=0;																		// ������������ ������

					  if(OP_cur>2)																														// ��������� ������ � ����������� �� ���� ����������
						  TRset(TYPE[type1][TR_cur]);
					  else
						  TRset(TYPE[type2][TR_cur]);

					  while(BUT_R ==  GPIO_PIN_RESET);																				// �������, ���� �� ����� �������� ������
					  HAL_Delay(50);																													// �������� ���������� ��������
				  }

				  if(BUT_M == GPIO_PIN_RESET) exit=0;																				// ��� ������� �� ����������� ������ ����� �� ��������
			  }

			  ResAll();																																		// ���������� ���� �������
			  for(int k=1; k<=4; k++)OPres(k);																								// ���������� ���� ������
			  HAL_TIM_Base_Start_IT(&htim2);																								// ��������� ������������ ���������
	  	  }
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------����� ���������--------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_M == GPIO_PIN_RESET)																										// ���� ������ ������� ������
	  {
		  if(btn_count!=40000) btn_count++;																							// ���������� �������� ������� ������� ������
		  else																																					// ���� �������� �������� ������ 40000
		  {
			  setup=!setup;																																// ������������ ������ ��������� �������
			  if(setup) sel=1; else sel=0;																										// ��������� ������ ����������� ���������
			  btn_count++;																																// ���������� �������� ��������� ������ ��� �� �������
		  }
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_L == GPIO_PIN_RESET && setup)																						// ���� � ������ ��������� ������ ����� ������
	  {
		  if(sel==1)																																			// ���� ������ ������ ��������
			  if(sTime.Hours>0) sTime.Hours -= 1; else sTime.Hours += 23;											// ���������� �������� �����
		  else if(sel==2)																																// ���� ������ ������ ��������
			  if(sTime.Minutes>0) sTime.Minutes -= 1; else sTime.Minutes += 59;									// ���������� �������� �����
		  else if(sel==3)																																// ���� ������ ������ ��������
			  if(sTime.Seconds>0) sTime.Seconds -= 1; else sTime.Seconds += 59;								// ���������� �������� ������

		  while(BUT_L == GPIO_PIN_RESET && valM_count<200)														// ���� ������ ������������ ����� 200 ��
		  {
			  valM_count++; 																															// ���������� �������� ��������� ������
			  HAL_Delay(1);																																// �������� 1 ��
		  }

		  if(valM_count>=200) 																													// ���� ������ ������������ ����� 200 ��
			  HAL_Delay(100);																															// �������� 100 ��

		  if(setup) HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);												// ���������� ���������� �������
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_R == GPIO_PIN_RESET && setup)																						// ���� � ������ ��������� ������ ������ ������
	  {
		  if(sel==1)																																			// ���� ������ ������ ��������
			  if(sTime.Hours<23) sTime.Hours += 1; else sTime.Hours -= 23;											// ���������� �������� �����
		  else if(sel==2)																																// ���� ������ ������ ��������
			  if(sTime.Minutes<59) sTime.Minutes += 1; else sTime.Minutes -= 59;								// ���������� �������� �����
		  else if(sel==3)																																// ���� ������ ������ ��������
			  if(sTime.Seconds<59) sTime.Seconds += 1; else sTime.Seconds -= 59;								// ���������� �������� ������

		  while(BUT_R == GPIO_PIN_RESET && valP_count<200)															// ���� ������ ������������ ����� 200 ��
		  {
			  valP_count++;																																// ���������� �������� ��������� ������
			  HAL_Delay(1);																																// �������� 1 ��
		  }

		  if(valP_count>=200) 																														// ���� ������ ������������ ����� 200 ��
			  HAL_Delay(100);																															// �������� 100 ��

		  if(setup) HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);												// ���������� ���������� �������
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------����� ��������� ���� �����������---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_L== GPIO_PIN_RESET && BUT_R == GPIO_PIN_SET && !setup)									// ���� � �������� ������ ������ ����� ������ � �������� ������
	  {
		  if(valM_count<500)																														// ���� ������� ��������� �� ��������� �������� 500
		  {
			  HAL_Delay(1);																																// �������� 1 ��
			  valM_count++;																															// ���������� �������� ��������� ������
		  }
		  else																																					// ���� ������� ��������� ��������� �������� 500
		  {
			  dot=0;																																			// ����������� ��������� �����
			  if(type2<1) type2++; else type2=0;																						// ������������� ��� ���� ����� �����������
			  SIM[0] = 0;																																	// ����������� ����
			  SIM[1] = 0;																																	// ����������� ����
			  MASK[2] = 0; MASK[3] = 0;																										// ���������� ��������� ������ �����������
			  while(BUT_L == GPIO_PIN_RESET);																							// �������, ���� �� ����� �������� ������

			  HVset(1500, 0);																															// ��������������� ���������� ����������
			  flash_unlock();																																// ������������� FLASH
			  flash_erase_page(0x08007FF0);																								// �������� ��������
			  flash_write(0x08007FF0, type1);																								// ������ � ������ ������� ��� ������ �����������
			  flash_write(0x08007FF4, type2);																								// ������ � ������ ������� ��� ����� �����������
			  flash_write(0x08007FF8, VOLT);																								// ������ �������� �������� ���������� ���
			  flash_lock();																																	// ���������� FLASH
		  }
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_R == GPIO_PIN_RESET && BUT_L == GPIO_PIN_SET && !setup)									// ���� � �������� ������ ������ ������ ������ � �������� �����
	  {
		  if(valP_count<500)																															// ���� ������� ��������� �� ��������� �������� 500
		  {
			  HAL_Delay(1);																																// �������� 1 ��
			  valP_count++;																																// ���������� �������� ��������� ������
		  }
		  else																																					// ���� ������� ��������� ��������� �������� 500
		  {
			  dot=0;																																			// ����������� ��������� �����
			  if(type1<1) type1++; else type1=0;																						// ������������� ��� ���� ������ �����������
			  SIM[2] = 0;																																	// ����������� ����
			  SIM[3] = 0;																																	// ����������� ����
			  MASK[0] = 0; MASK[1] = 0;																										// ���������� ��������� ����� �����������
			  while(BUT_R == GPIO_PIN_RESET); 																						// �������, ���� �� ����� �������� ������

			  HVset(1500, 0);																															// ��������������� ���������� ����������
			  flash_unlock();																																// ������������� FLASH
			  flash_erase_page(0x08007FF0);																								// �������� ��������
			  flash_write(0x08007FF0, type1);																								// ������ � ������ ������� ��� ������ �����������
			  flash_write(0x08007FF4, type2);																								// ������ � ������ ������� ��� ����� �����������
			  flash_write(0x08007FF8, VOLT);																								// ������ �������� �������� ���������� ���
			  flash_lock();																																	// ���������� FLASH
		  }
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------��������� ���������� ������------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(BUT_M == GPIO_PIN_SET)																												// ���� �������� ������� ������
	  {
		  if(btn_count<40000 && btn_count>0 && setup)																		// ���� ���� ������������� ������ ������� ������ � ������ ��������� �������
			  if(sel<3)sel++; else sel=1;																										// ������������ ������ ����������� ���������

		  	  btn_count=0;																																// ����� �������� �������� ��������� ������
	  	 }

	  if(BUT_L == GPIO_PIN_SET)																												// ���� �������� ����� ������
	  {
		  valM_count=0;																																	// ����� �������� ��������� ������
		  if(double_count) VLTset=1;																											// ���� ���� �������������� ������ ������� ������
		  double_count=0;																																// ����� �������� ��������� ���� ������
	  }

	  if(BUT_R == GPIO_PIN_SET)																												// ���� �������� ������ ������
	  {
		  valP_count=0;																																	// ����� �������� ��������� ������
		  if(double_count) VLTset=1;																											// ���� ���� �������������� ������ ������� ������
		  double_count=0;																																// ����� �������� ��������� ���� ������
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------������ ����������� �������------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  if(sel==0)																																				// ���� ������ �������� ����� ����������� �������
	  {
		  CUR[3] = sTime.Minutes%10;																										// ������� ������� �����
		  CUR[2] = sTime.Minutes/10;																											// ������� ������� �����
		  CUR[1] = sTime.Hours%10;																											// ������� ������� �����
		  CUR[0] = sTime.Hours/10;																												// ������� ������� �����

		  MASK[1] = 1;																																	// ���������� ��������������� ������
		  MASK[0] = 1;																																	// ���������� ��������������� ������
		  MASK[3] = 1;																																	// ���������� ��������������� ������
		  MASK[2] = 1;																																	// ���������� ��������������� ������

		  	  	  	  	  	  	  	  	  	  	  	  //------��������������-----//

		  if(CUR[0]!=SIMs[0] || CUR[1]!=SIMs[1] || CUR[2]!=SIMs[2] || CUR[3]!=SIMs[3] )					// ���� �������� ������ �� ����������� ���������� �� �������� �������
		  {
			  dot=0;																																			// ���������� ��������� �����
			  for(int i=0; i<4; i++)																													// ���� ��� �����������
				  SIMs[i] = SIM[i];																														// ����������� �������� ����������� �� �������������� ������

			  for(int j=0; j<10; j++)																												// ���� �������� ��� ��������������
			  {
				  for(int i=0; i<4; i++)																												// ������� ������� �����������
					  if(CUR[i]!=SIMs[i]) 																												// ���� ������� �������� ������� �� ��������� �� ��������� �� ���������������� �������
						  SIM[i] = POS[j];																												// ��������������� �������� ����� ������ ������������ ���� ������ ����������
				  HAL_Delay(50);																														// �������� 50 ��
			  }
		  }
		  for(int i=0; i<4; i++)																														// ���� ��� �����������
			  SIM[i] = CUR[i];																															// ��������� ������� ��������
	  }
	  else if(sel<3)																																		// ���� ������ ����� ��������� ����� ��� �����
	  {
		  SIM[3] = sTime.Minutes%10;																											// ������ ������ �����
		  SIM[2] = sTime.Minutes/10;																											// ������ �������� �����
		  SIM[1] = sTime.Hours%10;																											// ������ ������ �����
		  SIM[0] = sTime.Hours/10;																												// ������ �������� �����
		  MASK[1] = 1;																																	// ���������� ��������������� ������
		  MASK[0] = 1;																																	// ���������� ��������������� ������
	  }
	  else																																						// ���� ������ ����� ��������� ������
	  {
		  SIM[3] = sTime.Seconds%10;																										// ������ ������ ������
		  SIM[2] = sTime.Seconds/10;																											// ������ �������� ������
		  MASK[1] = 0;																																	// �� ���������� ��������������� ������
		  MASK[0] = 0;																																	// �� ���������� ��������������� ������
	  }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------����� ��������� ���������� ���----------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	  while(VLTset)
	  {
		  uint16_t tmp_cnt;																															// ��������� ������� ��������� ������
		  dot=0;																																				// ����������� ��������� �����

		  SIM[0] = VOLT/1000;																														// ������ �����
		  SIM[1] = VOLT%1000/100;																											// ������ �����
		  SIM[2] = VOLT%100/10;																												// ������ ��������
		  SIM[3] = VOLT%10;																														// ������ ������

		  if(BUT_M == GPIO_PIN_RESET)																									// ���� ������ ������� ������
		  {
			  VLTset=0;																																		// ����� �� ������ ��������� ���
			  HVset(1500, 0);																															// ��������������� ���������� ����������
			  flash_unlock();																																// ������������� FLASH
			  flash_erase_page(0x08007FF0);																								// �������� ��������
			  flash_write(0x08007FF0, type1);																								// ������ � ������ ������� ��� ������ �����������
			  flash_write(0x08007FF4, type2);																								// ������ � ������ ������� ��� ����� �����������
			  flash_write(0x08007FF8, VOLT);																								// ������ �������� �������� ���������� ���
			  flash_lock();																																	// ���������� FLASH
		  }

		  if(BUT_L == GPIO_PIN_RESET)																										// ���� ������ ����� ������
		  {
			  HAL_Delay(10);																															// �������� 10 ��
			  if(VOLT>700) VOLT--;																												// ���������� �������� ���������� �� ����������
			  tmp_cnt=0;																																	// ��������� �������� ��������� ������
			  while(BUT_L == GPIO_PIN_RESET)																							// ���� ����� ������ ������
			  {
				  HAL_Delay(10);																														// �������� 10 ��
				  if(tmp_cnt<70) tmp_cnt++; else if(VOLT>700) VOLT--;													// ��������� ��������������� ���������� �������� ��� ��������� ����� ������
				  SIM[0] = VOLT/1000;																												// ������ �����
				  SIM[1] = VOLT%1000/100;																									// ������ �����
				  SIM[2] = VOLT%100/10;																										// ������ ��������
				  SIM[3] = VOLT%10;																												// ������ ������
			  }
		  }

		  if(BUT_R == GPIO_PIN_RESET)																										// ���� ������ ������ ������
		  {
			  HAL_Delay(10);																															// �������� 10 ��
			  if(VOLT<1370) VOLT++;																											// ���������� �������� ���������� �� ����������
			  tmp_cnt=0;																																	// ��������� �������� ��������� ������
			  while(BUT_R == GPIO_PIN_RESET)																							// ���� ������ ������ ������
			  {
				  HAL_Delay(10);																														// �������� 10 ��
				  if(tmp_cnt<70) tmp_cnt++; else if(VOLT<1370) VOLT++;												// ��������� ��������������� ���������� �������� ��� ��������� ������ ������
				  SIM[0] = VOLT/1000;																												// ������ �����
				  SIM[1] = VOLT%1000/100;																									// ������ �����
				  SIM[2] = VOLT%100/10;																										// ������ ��������
				  SIM[3] = VOLT%10;																												// ������ ������
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
