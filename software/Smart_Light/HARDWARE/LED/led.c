/************************************************************
 ** ��  Ŀ  ��  �ƣ����������������������ƶ�ϵͳ(���ܵƲ���)
 **
 ** �� Ƭ �� �� �ţ�STM32F407VGT6
 **
 ** ��  ��  ��  �ƣ�led.c
 **
 ** ��    ��    �ˣ�ȫ���
 **
 ** ��          �������ܵ�ϵͳled������غ����ļ�
 **
 ** �ļ� ���� ���ڣ�2019��8��12��
 **
 ** ��־��
 ** Data							Author					Note
 @2019/8/12					  ȫ���					�������ļ�
************************************************************/

#include "led.h"  

struct led_message ds_ms;

//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA| RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_14 | GPIO_Pin_15);//GPIOF9,F10���øߣ�����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);//GPIOA0,A1,A2,A3���øߣ�����
}

u8 get_led_num(void)
{
		u8 light_num = 0;
		ds_ms.ds1_stat = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0);
		ds_ms.ds2_stat = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1);
		ds_ms.ds3_stat = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2);
		ds_ms.ds4_stat = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_3);
		
		light_num = ds_ms.light_num = ds_ms.ds1_stat+ds_ms.ds2_stat+ds_ms.ds3_stat+ds_ms.ds4_stat;
		
		return light_num;
}

u8 led_contrl(void)
{
	 if(ds_ms.light_stat == 0)
	 {
				DS1 = DS2 = DS3 = DS4 = 0;
	 }
	 else
	 {
			if(ds_ms.light_num>4)
					ds_ms.light_num = 4;
			else if(ds_ms.light_num<0)
					ds_ms.light_num = 0;
		  switch(ds_ms.light_num)
			{
				case 1:
					DS1 = 1;DS2 = DS3 = DS4 = 0;
				break;
				case 2:
					DS1 = DS2 = 1;DS3 = DS4 = 0;
				break;
				case 3:
					DS1 = DS2 = DS3 = 1;DS4 = 0;
				break;
				case 4:
					DS1 = DS2 = DS3 = DS4 = 1;
				break;
				default :
					DS1 = DS2 = DS3 = DS4 = 0;
				break;
			}
	 }
}





