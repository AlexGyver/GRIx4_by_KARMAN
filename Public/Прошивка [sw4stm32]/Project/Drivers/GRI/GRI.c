#include "GRI.h"


// Одновременное отключение всех катодов
void ResAll(void)
{
	C1_res; C2_res; C3_res; C4_res; C5_res; C6_res; C7_res; C8_res; C9_res; C10_res; C11_res; C12_res;
}

// Одновременное включение всех катодов
void SetAll(void)
{
	C1_set; C2_set; C3_set; C4_set; C5_set; C6_set; C7_set; C8_set; C9_set; C10_set; C11_set; C12_set;
}

// Включение указанного анода
void OPset(uint8_t n)
{
	if(n==1) A1_set;
	if(n==2) A2_set;
	if(n==3) A3_set;
	if(n==4) A4_set;
}

// Отключение указанного анода
void OPres(uint8_t n)
{
	if(n==1) A1_res;
	if(n==2) A2_res;
	if(n==3) A3_res;
	if(n==4) A4_res;
}

// Включение указанного катода
void TRset(uint8_t n)
{
	if(n==0) C1_set;
	if(n==1) C2_set;
	if(n==2) C3_set;
	if(n==3) C4_set;
	if(n==4) C5_set;
	if(n==5) C6_set;
	if(n==6) C7_set;
	if(n==7) C8_set;
	if(n==8) C9_set;
	if(n==9) C10_set;
	if(n==10) C11_set;
	if(n==11) C12_set;
}

// Установка режима генерации ШИМ
void HVset(uint32_t V1, uint32_t V2)
{
    TIM3->ARR =V1;
    TIM3->CCR1=V1-V2;
}

// Функция задержки
void wait(uint32_t cnt)
{
	while(cnt--);
}
