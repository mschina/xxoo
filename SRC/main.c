#include <reg52.h>
#include <INTRINS.H>
#include "lcd.h"
#include "delay.h"
#include "DHT11.h"




void main(void)
{
	SP=0x80;
	DelayInit();
	P0=0x00;
	while(1);
}