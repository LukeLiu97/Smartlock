#include "stm32f10x.h"

#define LED3_OFF() (GPIO_SetBits(GPIOB, GPIO_Pin_6))
#define LED3_ON() (GPIO_ResetBits(GPIOB, GPIO_Pin_6))

#define LED2_OFF() (GPIO_SetBits(GPIOB, GPIO_Pin_8))
#define LED2_ON() (GPIO_ResetBits(GPIOB, GPIO_Pin_8))

void LED_Init(void);
void Delay(u32 t);

int main(void)
{
	LED_Init();
	
	LED2_OFF();
	LED3_ON();
	while(1)
	{
		Delay(1000000);
		LED2_ON();
		LED3_ON();
		Delay(1000000);
		LED2_OFF();
		LED3_OFF();
	}
}

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* Configure PB6 and PB8 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Reset pin */
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	
	return ;
}

void Delay(u32 t)
{
	while(t--)
	{
		
	}
	
	return ;
}

