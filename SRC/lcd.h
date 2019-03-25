

#ifndef __LCD_H_
#define __LCD_H_

//IO口设置:
#define LCD_IO P2     //DATA	数据线
sbit LCD_RS = P1 ^ 6; //RX 		0指令，1数据
sbit LCD_RW = P1 ^ 5; //RW 		0写入，1读出
sbit LCD_E = P1 ^ 4;  //E  		使能端，1有效

void LCD_Init(void);      //新初始化重写
void LCD_Delay_ms(int t); //一毫秒的延时

void LCD_Read(unsigned char *dat);
void LCD_ReadChars(unsigned char x, unsigned char y, unsigned char *buffer, int n);

//x是从左到右，y是从上到下

/*
*	定义LCD_Write()的wr_type，cmd为写命令，data为写数据
*/
#define WR_CMD 0
#define WR_DATA 1
void LCD_Write(unsigned char wr_type, unsigned char dat);     //LCD写接口
#define LCD_WriteData(LCD_DATA) LCD_Write(WR_DATA, LCD_DATA); //LCD写入数据
#define LCD_WriteCOM(LCD_CMD) LCD_Write(WR_CMD, LCD_CMD);     //LCD写入命令

void LCD_WriteString(unsigned char x, unsigned char y, unsigned char *String); //LCD写入一个字符串
void LCD_WriteChars(unsigned char x, unsigned char y, char *List, int n);      //LCD写入N个字符(行内位置，行位置，字符串指针,写入个数)

//设置光标
/*
	1602的DDR显存
	第一行是0x00第二行的地址是0x40
	16x2的显示范围是0x00-0x0F,0x40-0x4F
	
*/
#define LCD_XY(x, y) LCD_WriteCOM(0x80 + x + ((y == 0) ? 0 : 0x40));

//LCD往y行x列写入一个字节
#define LCD_WriteChar(x, y, LCD_DATA) \
    {                                 \
        LCD_XY(x, y);                 \
        LCD_WriteData(LCD_DATA);      \
    }

/*工作模式相关命令*/

/*工作方式设置
001[DL]   [N][F][x][x];
    DL8 	8线并行[4线并行];
    N2      2行显示[1行显示];
    F5_8	5x10字符点阵[5x8字符点阵];
*/
#define COM_WORK_BASE 0x20

//DL8 	8线并行[4线并行]
#define COM_WORK_DL8 0x10
//N2 2行显示[1行显示]
#define COM_WORK_N2 0x08
//F5_8	5x10字符点阵[5x8字符点阵]
#define COM_WORK_F5_10 0x04

//工作模式[Work]默认设置:8线并行 2行显示 5x10点阵
#define COM_WORK_DEFAULT (COM_WORK_BASE | COM_WORK_DL8 | COM_WORK_N2 | COM_WORK_F5_10)

/*----------*/

/*显示相关命令*/
//清屏命令 0000 0001
#define COM_SCR_CLEAR 0x01

/*
屏幕相关设置，设置显示、光标、光标闪烁;
0000 1[d][c][b];
    D:显示开[关];
    C:光标开[关];
    B:光标闪烁[不闪烁];
*/
#define COM_SCR_BASE 0x08

//SCR_ON 显示打开
#define COM_SCR_ON 0x04
//SCR_CUR_ON 光标打开
#define COM_SCR_CUR_ON 0x02
//SCR_CUR_FLASH 光标闪烁
#define COM_SCR_CUR_FLASH 0x01

//默认显示:显示[开] 光标[关] 光标闪烁[关]
#define COM_SCR_DEFAULT (COM_SCR_BASE | COM_SCR_ON)

//光标归位 0000 001x
#define COM_CUR_RESET 0x02

/*----------*/

/*模式指令*/
/*
    设置写入数据后，光标移动还是数据移动;
    0000 01[I/D][s];
    I/D 写入新数据后光标位移方向[1:右,0:左];
    S 显示是否移动[跟光标移动方向相反,1:文字往左边移动,0:右边];
*/
#define COM_MD_BASE 0x40

//MD_CURMV_RIGHT 写入新数据后光标[右]移动;
#define COM_MD_CURMV_RIGHT 0x20
//MD_CURMV_LEFT 写入新数据后光标[左]移动;
#define COM_MD_CURMV_LEFT 0x00
//MD_SCR_SHITF 屏幕移动;
#define COM_MD_SCR_SHITF 0x01
//默认:光标[右]移动,显示[不]移动;
#define COM_MD_DEFAULT (COM_MD_BASE | COM_MD_CURMV_RIGHT)

//move base
#define COM_MV_BASE 0x10
//移动命令:0001 [S/C][R/L]xx
#define COM_MV_CUR 0x00
#define COM_MV_SCR 0x08

//移动方向
#define MV_LEFT 0x00
#define MV_RIGHT 0x04

#define COM_setMV(CUR_SCR, DIR) (COM_MV_BASE | (CUR_SCR & 0x08) | (DIR | 0x04))

/*----------*/

#define COM_setCGRAM(addr) (0x40 | (0x3f & addr))
//设置CGRAM地址 01aa aaaa
//             0011 1111

#define COM_setDDRAM(addr) (0x80 | (0x7f & addr))
//设置CGRAM地址 1aaa aaaa
//             0111 1111

#endif

/*
使用说明书:
1.设置数据线（默认IO是P2,RS=P16,RW=P15,E=P14）
2.初始化LCD：LCD_init();
3.把屏幕看成一个二维数组来对待，进行操作
	LCD_WriteChar(0,0,'a');	往第一行
	LCD_WriteChars(0,0,data,2); data数组是个两个元素的数组，开始位置是第一行第一列从左到右写
	能够连续写的数据，尽量连续写，以提升效率

4.执行相关命令：
清屏:LCD_Write(WR_CMD,COM_SCR_CLEAR);或者使用便捷宏LCD_WriteCOM(COM_SCR_CLEAR)


命令分类
标记说明

命令的格式分3种
格式为
COM_分类_[分类]_[功能]
例如:COM_WORK_BASE  base是命令的基础代码。需要与选项位或使用
COM_WORK_DEFAULT 
default是该分支的默认设置



1.单独的完整指令
  例如清屏 COM_SCR_CLEAR

2.base位或选项
   例如:COM_WORK_BAESE | COM_WORK_DL8 | COM_WORK_N2

3.宏函数设置
   例如:COM_setDDRAM(0x00)

工作模式     WORK
{
    +DL8:8线 默认4线
    +N2:2行 默认1行
    +F5_10   默认5x8
}

显示命令     SCR
{
    +ON  显示打开   
    +CUR{
        ON 光标打开
        FLASH 光标闪烁
    }
    -CLEAR 清屏
}

移动命令(针对屏幕或者光标)  MV
{
    
    
}

未完待续...


光标和移动命令MV
显示模式设置 MD

*/