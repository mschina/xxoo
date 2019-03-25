#include "STC11F08XE.h"
#include "delay.h"

static unsigned short DelayMS,DelayUS;

void DelayInit(void)		//10΢��@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = TL0_DEFAULT;		//���ö�ʱ��ֵ
	TH0 = TH0_DEFAULT;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	EA=1;		//���ж�
	ET0=1;	//�򿪶�ʱ���ж�
}

void DelayRestart(){
	DelayStop();
	TF0 = 0;
	TL0 = TL0_DEFAULT;		//���ö�ʱ��ֵ
	TH0 = TH0_DEFAULT;		//���ö�ʱ��ֵ
	DelayStart();
}


void Delay10ms()
{
	
}

void Delay100ms()
{
	
}

static void Delay_Rountine(void) interrupt INTERRUPT_NUMBER using 0
{
	DelayStop();
	TL0 = TL0_DEFAULT;		//���ö�ʱ��ֵ
	TH0 = TH0_DEFAULT;		//���ö�ʱ��ֵ
	DelayStart();
}



