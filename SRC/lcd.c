#include <reg52.h>
#include "lcd.h"

/*
	�������ʱ�䣺2018-11-30 ����
*/

//��æ��
sbit LCD_BUSY = LCD_IO ^ 7;

/*++++++++++++++++++++++
	��ʱ������
	����:t=1ʱ����Լ��ʱ��1ms
+++++++++++++++++++++++*/
void LCD_Delay_ms(int t)
{
	unsigned char i = 111;
	while (t--)
		while (i--)
			;
}

/*++++++++++++++++++++++
	��æ����:
	����:�ȴ�LCD��Ӧ
	˵��:����ʱ��ÿ��д�����������ʱ��Ӧ���ȶ�æ���ȴ�LCD���к�������
++++++++++++++++++++++++*/
void Read_Busy(void)
{
	unsigned char i = 255;
	LCD_IO = 0xff;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_E = 1;
	while ((i--) && (LCD_BUSY))
		; //����ȴ�ʱ�����������LCD_BUSYλ��Ϊ1˵��LCD���У����ü���ѭ��
	LCD_E = 0;
}

/*+++++++++++++++++++++++++++++++++++++++++++++
	LCD_Write(���ͣ�����)
	����:��LCD��RAMд�����ݣ�д��ǰ�����õ�ַ
	����ֵ����
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
	д�����ɸ��ַ����ڼ���,�ڼ��У����飬������
	���ö�ȡ���飬�������ڣ��¶�֮����ַ����黺��
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
д���ַ���(��ʼ��ַ��x�Ǻ��ŵ㣬y�����ŵ㣬00�ǵ�һ�е�һ����10�ǵ�һ�еڶ��������ַ���)
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
��ȡ���λ����(ʵ��ָ��)
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
�ӹ��λ�ÿ�ʼ��ȡn���ֽ�
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
	LCD_WriteCOM(COM_WORK_DEFAULT);   //Ĭ�Ϲ���ģʽ
	LCD_WriteCOM(COM_SCR_DEFAULT);	//��ĻĬ������
	LCD_WriteCOM(COM_SCR_CLEAR);	  //����
	LCD_WriteCOM(COM_MD_DEFAULT);	 //ģʽĬ������
	LCD_WriteCOM(COM_CUR_RESET);	  //�������
	LCD_WriteCOM(COM_setDDRAM(0x00)); //�����Դ��ַ���ӵ�һ�е�һ����ʼ
}