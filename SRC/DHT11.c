#include<reg52.h>
#include <INTRINS.H>
#include "DHT11.H"





//状态变量
static unsigned char status;
//保存获取的数据
//[湿度整数，湿度小数，温度整数，温度小数，校检和]
static uchar value_array[SIZE];


void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}



 //延时函数
 void DHT11_Delay_1ms(unsigned int ms)
 {
	 
 	unsigned int x;//,y;
	for (x=ms;x>0;x--)
		Delay1ms();
	 //for (y=124;y>0;y--) ;
	 
	 
}



void DHT11_Delay_10us(void)		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}

/*

void DHT11_Delay_10us(void)
{
//6次自减大约10us
	unsigned char i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;

}
*/

 unsigned char DHT11_ReadValue(void)
{
	unsigned char count,value=0,i;
	status=DHT11_OK;
	for (i=8;i>0;i--)
	{
		//高位在先
		value<<=1;
		count=0;
		//每一位数据前面都有50us的低电平时间，等待低电平结束
		while (DHT11==0&&count++<NUMBER);
		if (count>=NUMBER)
		{
			status=DHT11_ERROR;	//设置状态错误
			return 0;
		}
		//26-28us为0,70us高电平表示1,
		DHT11_Delay_10us();
		DHT11_Delay_10us();
		DHT11_Delay_10us();
		//延时30us后检测点评
		if (DHT11!=0)
		{
			//高电平为1,1进位
			value++;
			//等待剩余的40us结束
			while (DHT11!=0&&count++<NUMBER) DHT11=1;
			if (count>=NUMBER)
			{
				status=DHT11_ERROR; //设定错误标志
				return 0;
			}
		}
	}
	//返回读取的一字节数据
	//P0=value;
	return value;
}

unsigned char DHT11_Read_data(void)
{
	unsigned char i=0,check_value=0,count=0;
	DHT11=0;	//拉低数据线大于18ms
	DHT11_Delay_1ms(20);
	DHT11=1;	//释放数据线
	//延时20-40us
	DHT11_Delay_10us();
	DHT11_Delay_10us();
	DHT11_Delay_10us();
	DHT11_Delay_10us();
	
	if (DHT11!=0)	//检测是否有应答信号
	{
		//没有应答信号
		return DHT11_ERROR;
	}
	else
	{
		//有应答信号
		while (DHT11==0&&count++<NUMBER); //等待应答信号结束
		if (count>=NUMBER)
		{
			//读取数据出错，
			DHT11=1;
			return DHT11_ERROR;
		}
		count=0; //复位计数
		DHT11=1;	//释放数据线
		//应答信号后有个80us的高电平，等待结束
		while (DHT11!=0&& count++<NUMBER);
		if (count>=NUMBER)
		{
			//发生错误，结束
			DHT11=1;
			return DHT11_ERROR;
		}
		//循环读出温度和湿度值
		for (i=0;i<SIZE;i++)
		{
			value_array[i]=DHT11_ReadValue();
			if (status==DHT11_ERROR)
			{
				DHT11=1;
				return DHT11_ERROR;
			}
			//计算校检值
			if (i!=SIZE-1) check_value+=value_array[i];
		}

		//最后把得到的数据校检
		if (check_value==value_array[SIZE-1])
		{
			return DHT11_OK;
		}
		else return DHT11_ERROR;	//校检出错

	}


}


/*+++++++++++++++++++++++++++++++++++++
	获取DHT11所读取出来的温度和湿度函数:
	说明:
		1.如果成功返回值为1，说明读取成功，数据通过指针传输到形参指向的地址
		2.如果失败返回值为0
		3.使用前，应该用if判断是否读取数据成功
+++++++++++++++++++++++++++++++++++++++*/
int DHT11_GET_DATA(int *Temp,int *HUMI)
{
	if (DHT11_Read_data())
	{
		*Temp=value_array[2];
		*HUMI=value_array[0];
		return 1;
	}
	else
	{
		*Temp=0;
		*HUMI=0;
		return 0;

	}		
}
