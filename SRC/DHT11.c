#include<reg52.h>
#include <INTRINS.H>
#include "DHT11.H"





//״̬����
static unsigned char status;
//�����ȡ������
//[ʪ��������ʪ��С�����¶��������¶�С����У���]
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



 //��ʱ����
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
//6���Լ���Լ10us
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
		//��λ����
		value<<=1;
		count=0;
		//ÿһλ����ǰ�涼��50us�ĵ͵�ƽʱ�䣬�ȴ��͵�ƽ����
		while (DHT11==0&&count++<NUMBER);
		if (count>=NUMBER)
		{
			status=DHT11_ERROR;	//����״̬����
			return 0;
		}
		//26-28usΪ0,70us�ߵ�ƽ��ʾ1,
		DHT11_Delay_10us();
		DHT11_Delay_10us();
		DHT11_Delay_10us();
		//��ʱ30us�������
		if (DHT11!=0)
		{
			//�ߵ�ƽΪ1,1��λ
			value++;
			//�ȴ�ʣ���40us����
			while (DHT11!=0&&count++<NUMBER) DHT11=1;
			if (count>=NUMBER)
			{
				status=DHT11_ERROR; //�趨�����־
				return 0;
			}
		}
	}
	//���ض�ȡ��һ�ֽ�����
	//P0=value;
	return value;
}

unsigned char DHT11_Read_data(void)
{
	unsigned char i=0,check_value=0,count=0;
	DHT11=0;	//���������ߴ���18ms
	DHT11_Delay_1ms(20);
	DHT11=1;	//�ͷ�������
	//��ʱ20-40us
	DHT11_Delay_10us();
	DHT11_Delay_10us();
	DHT11_Delay_10us();
	DHT11_Delay_10us();
	
	if (DHT11!=0)	//����Ƿ���Ӧ���ź�
	{
		//û��Ӧ���ź�
		return DHT11_ERROR;
	}
	else
	{
		//��Ӧ���ź�
		while (DHT11==0&&count++<NUMBER); //�ȴ�Ӧ���źŽ���
		if (count>=NUMBER)
		{
			//��ȡ���ݳ���
			DHT11=1;
			return DHT11_ERROR;
		}
		count=0; //��λ����
		DHT11=1;	//�ͷ�������
		//Ӧ���źź��и�80us�ĸߵ�ƽ���ȴ�����
		while (DHT11!=0&& count++<NUMBER);
		if (count>=NUMBER)
		{
			//�������󣬽���
			DHT11=1;
			return DHT11_ERROR;
		}
		//ѭ�������¶Ⱥ�ʪ��ֵ
		for (i=0;i<SIZE;i++)
		{
			value_array[i]=DHT11_ReadValue();
			if (status==DHT11_ERROR)
			{
				DHT11=1;
				return DHT11_ERROR;
			}
			//����У��ֵ
			if (i!=SIZE-1) check_value+=value_array[i];
		}

		//���ѵõ�������У��
		if (check_value==value_array[SIZE-1])
		{
			return DHT11_OK;
		}
		else return DHT11_ERROR;	//У�����

	}


}


/*+++++++++++++++++++++++++++++++++++++
	��ȡDHT11����ȡ�������¶Ⱥ�ʪ�Ⱥ���:
	˵��:
		1.����ɹ�����ֵΪ1��˵����ȡ�ɹ�������ͨ��ָ�봫�䵽�β�ָ��ĵ�ַ
		2.���ʧ�ܷ���ֵΪ0
		3.ʹ��ǰ��Ӧ����if�ж��Ƿ��ȡ���ݳɹ�
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
