#ifndef uchar
#define uchar unsigned char
#endif

#ifndef __DHT11_H__
#define __DHT11_H__

#define SIZE 5				//数组长度，一共5字节数据
#define DHT11_OK 1			//设定标志，成功返回1，失败返回0
#define DHT11_ERROR 0
#define NUMBER 20			//防止与硬件通讯时候出现死循环

sbit DHT11=P1^0;			//DHT11的数据线



void DHT11_Delay_1ms(unsigned int ms); //1ms的延时


void DHT11_Delay_10us(void);//10us延时


unsigned char DHT11_ReadValue(void);//读取单个字节的数据


unsigned char DHT11_Read_data(void);//读取所有数据

int DHT11_GET_DATA(int *Temp,int *HUMI); //返回获得的温度和湿度


#endif

/*
	函数调用说明：
	直接调用DHT11_GET_DATA(温度变量地址，湿度变量地址)
	成功返回1，失败返回0
	如果成功则可以使用实参的数值
	[湿度整数，湿度小数，温度整数，温度小数，校检和]
*/
