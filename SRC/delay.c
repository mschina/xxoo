#include "STC11F08XE.h"
#include "delay.h"

static unsigned short DelayMS,DelayUS;

void DelayInit(void)		//10微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = TL0_DEFAULT;		//设置定时初值
	TH0 = TH0_DEFAULT;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA=1;		//打开中断
	ET0=1;	//打开定时器中断
}

void DelayRestart(){
	DelayStop();
	TF0 = 0;
	TL0 = TL0_DEFAULT;		//设置定时初值
	TH0 = TH0_DEFAULT;		//设置定时初值
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
	TL0 = TL0_DEFAULT;		//设置定时初值
	TH0 = TH0_DEFAULT;		//设置定时初值
	DelayStart();
}



