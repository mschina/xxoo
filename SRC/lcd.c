#include <reg52.h>
#include "lcd.h"

/*
	代码更新时间：2018-11-30 下午
*/

//读忙线
sbit LCD_BUSY = LCD_IO ^ 7;

/*++++++++++++++++++++++
	延时函数：
	作用:t=1时，大约延时是1ms
+++++++++++++++++++++++*/
void LCD_Delay_ms(int t)
{
	unsigned char i = 111;
	while (t--)
		while (i--)
			;
}

/*++++++++++++++++++++++
	读忙函数:
	作用:等待LCD反应
	说明:根据时序，每次写命令或者数据时候应该先读忙，等待LCD空闲后再输入
++++++++++++++++++++++++*/
void Read_Busy(void)
{
	unsigned char i = 255;
	LCD_IO = 0xff;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_E = 1;
	while ((i--) && (LCD_BUSY))
		; //如果等待时间过长或者是LCD_BUSY位不为1说明LCD空闲，则不用继续循环
	LCD_E = 0;
}

/*+++++++++++++++++++++++++++++++++++++++++++++
	LCD_Write(类型，数据)
	作用:往LCD的RAM写入数据，写入前请设置地址
	返回值：无
++++++++++++++++++++++++++++++++++++++++++++++*/
void LCD_Write(unsigned char wr_type, unsigned char dat)
{
	Read_Busy();
	LCD_E = 1;
	LCD_RS = wr_type;
	LCD_RW = 0;
	LCD_IO = dat;
	LCD_E = 0;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++
	写入若干个字符（第几个,第几行，数组，个数）
	适用读取数组，例如日期，温度之类的字符数组缓冲
++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void LCD_WriteChars(unsigned char x, unsigned char y, char *List, int n)
{
	int i;
	LCD_XY(x, y);
	for (i = 0; i < n; i++)
	{
		LCD_WriteData(*List);
		List++;
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
写入字符串(起始地址（x是横着点，y是竖着点，00是第一行第一个，10是第一行第二个），字符串)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void LCD_WriteString(unsigned char x, unsigned char y, unsigned char *String)
{
	LCD_XY(x, y);
	while (*String != '\0')
	{
		LCD_WriteData(*String);
		String++;
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
读取光标位置字(实参指针)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void LCD_Read(unsigned char *dat)
{
	Read_Busy();
	LCD_IO = 0xff;
	LCD_E = 1;
	LCD_RS = 1;
	LCD_RW = 1;
	*dat = LCD_IO;
	LCD_E = 0;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
从光标位置开始读取n个字节
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void LCD_ReadChars(unsigned char x, unsigned char y, unsigned char *buffer, int n)
{
	int i = 0;
	for (; i < n; i++, x++)
	{
		LCD_XY(x, y);
		LCD_Read(buffer + i);
	}
}

void LCD_Init(void)
{
	LCD_WriteCOM(COM_WORK_DEFAULT);   //默认工作模式
	LCD_WriteCOM(COM_SCR_DEFAULT);	//屏幕默认设置
	LCD_WriteCOM(COM_SCR_CLEAR);	  //清屏
	LCD_WriteCOM(COM_MD_DEFAULT);	 //模式默认设置
	LCD_WriteCOM(COM_CUR_RESET);	  //光标重置
	LCD_WriteCOM(COM_setDDRAM(0x00)); //设置显存地址。从第一行第一个开始
}