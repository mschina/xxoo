#ifndef __DELAY_H__
#define __DELAY_H__

//10us延时初值0xFF91
#define TH0_DEFAULT 0xFF
#define TL0_DEFAULT 0x91

#define INTERRUPT_NUMBER 1

void DelayInit(void);
void DelayStart();
#define DelayStop() {TR0=0;}
#define DelayStart() {TR0=1;}
void DelayRestart();

void Delay10ms();
void Delay100ms();

#endif




/*
	delay将占用一个定时器0
*/