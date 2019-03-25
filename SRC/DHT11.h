#ifndef uchar
#define uchar unsigned char
#endif

#ifndef __DHT11_H__
#define __DHT11_H__

#define SIZE 5				//���鳤�ȣ�һ��5�ֽ�����
#define DHT11_OK 1			//�趨��־���ɹ�����1��ʧ�ܷ���0
#define DHT11_ERROR 0
#define NUMBER 20			//��ֹ��Ӳ��ͨѶʱ�������ѭ��

sbit DHT11=P1^0;			//DHT11��������



void DHT11_Delay_1ms(unsigned int ms); //1ms����ʱ


void DHT11_Delay_10us(void);//10us��ʱ


unsigned char DHT11_ReadValue(void);//��ȡ�����ֽڵ�����


unsigned char DHT11_Read_data(void);//��ȡ��������

int DHT11_GET_DATA(int *Temp,int *HUMI); //���ػ�õ��¶Ⱥ�ʪ��


#endif

/*
	��������˵����
	ֱ�ӵ���DHT11_GET_DATA(�¶ȱ�����ַ��ʪ�ȱ�����ַ)
	�ɹ�����1��ʧ�ܷ���0
	����ɹ������ʹ��ʵ�ε���ֵ
	[ʪ��������ʪ��С�����¶��������¶�С����У���]
*/
