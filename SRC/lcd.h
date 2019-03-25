

#ifndef __LCD_H_
#define __LCD_H_

//IO������:
#define LCD_IO P2     //DATA	������
sbit LCD_RS = P1 ^ 6; //RX 		0ָ�1����
sbit LCD_RW = P1 ^ 5; //RW 		0д�룬1����
sbit LCD_E = P1 ^ 4;  //E  		ʹ�ܶˣ�1��Ч

void LCD_Init(void);      //�³�ʼ����д
void LCD_Delay_ms(int t); //һ�������ʱ

void LCD_Read(unsigned char *dat);
void LCD_ReadChars(unsigned char x, unsigned char y, unsigned char *buffer, int n);

//x�Ǵ����ң�y�Ǵ��ϵ���

/*
*	����LCD_Write()��wr_type��cmdΪд���dataΪд����
*/
#define WR_CMD 0
#define WR_DATA 1
void LCD_Write(unsigned char wr_type, unsigned char dat);     //LCDд�ӿ�
#define LCD_WriteData(LCD_DATA) LCD_Write(WR_DATA, LCD_DATA); //LCDд������
#define LCD_WriteCOM(LCD_CMD) LCD_Write(WR_CMD, LCD_CMD);     //LCDд������

void LCD_WriteString(unsigned char x, unsigned char y, unsigned char *String); //LCDд��һ���ַ���
void LCD_WriteChars(unsigned char x, unsigned char y, char *List, int n);      //LCDд��N���ַ�(����λ�ã���λ�ã��ַ���ָ��,д�����)

//���ù��
/*
	1602��DDR�Դ�
	��һ����0x00�ڶ��еĵ�ַ��0x40
	16x2����ʾ��Χ��0x00-0x0F,0x40-0x4F
	
*/
#define LCD_XY(x, y) LCD_WriteCOM(0x80 + x + ((y == 0) ? 0 : 0x40));

//LCD��y��x��д��һ���ֽ�
#define LCD_WriteChar(x, y, LCD_DATA) \
    {                                 \
        LCD_XY(x, y);                 \
        LCD_WriteData(LCD_DATA);      \
    }

/*����ģʽ�������*/

/*������ʽ����
001[DL]   [N][F][x][x];
    DL8 	8�߲���[4�߲���];
    N2      2����ʾ[1����ʾ];
    F5_8	5x10�ַ�����[5x8�ַ�����];
*/
#define COM_WORK_BASE 0x20

//DL8 	8�߲���[4�߲���]
#define COM_WORK_DL8 0x10
//N2 2����ʾ[1����ʾ]
#define COM_WORK_N2 0x08
//F5_8	5x10�ַ�����[5x8�ַ�����]
#define COM_WORK_F5_10 0x04

//����ģʽ[Work]Ĭ������:8�߲��� 2����ʾ 5x10����
#define COM_WORK_DEFAULT (COM_WORK_BASE | COM_WORK_DL8 | COM_WORK_N2 | COM_WORK_F5_10)

/*----------*/

/*��ʾ�������*/
//�������� 0000 0001
#define COM_SCR_CLEAR 0x01

/*
��Ļ������ã�������ʾ����ꡢ�����˸;
0000 1[d][c][b];
    D:��ʾ��[��];
    C:��꿪[��];
    B:�����˸[����˸];
*/
#define COM_SCR_BASE 0x08

//SCR_ON ��ʾ��
#define COM_SCR_ON 0x04
//SCR_CUR_ON ����
#define COM_SCR_CUR_ON 0x02
//SCR_CUR_FLASH �����˸
#define COM_SCR_CUR_FLASH 0x01

//Ĭ����ʾ:��ʾ[��] ���[��] �����˸[��]
#define COM_SCR_DEFAULT (COM_SCR_BASE | COM_SCR_ON)

//����λ 0000 001x
#define COM_CUR_RESET 0x02

/*----------*/

/*ģʽָ��*/
/*
    ����д�����ݺ󣬹���ƶ����������ƶ�;
    0000 01[I/D][s];
    I/D д�������ݺ���λ�Ʒ���[1:��,0:��];
    S ��ʾ�Ƿ��ƶ�[������ƶ������෴,1:����������ƶ�,0:�ұ�];
*/
#define COM_MD_BASE 0x40

//MD_CURMV_RIGHT д�������ݺ���[��]�ƶ�;
#define COM_MD_CURMV_RIGHT 0x20
//MD_CURMV_LEFT д�������ݺ���[��]�ƶ�;
#define COM_MD_CURMV_LEFT 0x00
//MD_SCR_SHITF ��Ļ�ƶ�;
#define COM_MD_SCR_SHITF 0x01
//Ĭ��:���[��]�ƶ�,��ʾ[��]�ƶ�;
#define COM_MD_DEFAULT (COM_MD_BASE | COM_MD_CURMV_RIGHT)

//move base
#define COM_MV_BASE 0x10
//�ƶ�����:0001 [S/C][R/L]xx
#define COM_MV_CUR 0x00
#define COM_MV_SCR 0x08

//�ƶ�����
#define MV_LEFT 0x00
#define MV_RIGHT 0x04

#define COM_setMV(CUR_SCR, DIR) (COM_MV_BASE | (CUR_SCR & 0x08) | (DIR | 0x04))

/*----------*/

#define COM_setCGRAM(addr) (0x40 | (0x3f & addr))
//����CGRAM��ַ 01aa aaaa
//             0011 1111

#define COM_setDDRAM(addr) (0x80 | (0x7f & addr))
//����CGRAM��ַ 1aaa aaaa
//             0111 1111

#endif

/*
ʹ��˵����:
1.���������ߣ�Ĭ��IO��P2,RS=P16,RW=P15,E=P14��
2.��ʼ��LCD��LCD_init();
3.����Ļ����һ����ά�������Դ������в���
	LCD_WriteChar(0,0,'a');	����һ��
	LCD_WriteChars(0,0,data,2); data�����Ǹ�����Ԫ�ص����飬��ʼλ���ǵ�һ�е�һ�д�����д
	�ܹ�����д�����ݣ���������д��������Ч��

4.ִ��������
����:LCD_Write(WR_CMD,COM_SCR_CLEAR);����ʹ�ñ�ݺ�LCD_WriteCOM(COM_SCR_CLEAR)


�������
���˵��

����ĸ�ʽ��3��
��ʽΪ
COM_����_[����]_[����]
����:COM_WORK_BASE  base������Ļ������롣��Ҫ��ѡ��λ��ʹ��
COM_WORK_DEFAULT 
default�Ǹ÷�֧��Ĭ������



1.����������ָ��
  �������� COM_SCR_CLEAR

2.baseλ��ѡ��
   ����:COM_WORK_BAESE | COM_WORK_DL8 | COM_WORK_N2

3.�꺯������
   ����:COM_setDDRAM(0x00)

����ģʽ     WORK
{
    +DL8:8�� Ĭ��4��
    +N2:2�� Ĭ��1��
    +F5_10   Ĭ��5x8
}

��ʾ����     SCR
{
    +ON  ��ʾ��   
    +CUR{
        ON ����
        FLASH �����˸
    }
    -CLEAR ����
}

�ƶ�����(�����Ļ���߹��)  MV
{
    
    
}

δ�����...


�����ƶ�����MV
��ʾģʽ���� MD

*/