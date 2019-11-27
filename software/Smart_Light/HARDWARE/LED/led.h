#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

struct led_message
{
		u8 light_stat;		//�ƹ�״̬
		int light_num;		//�ƹ�����
		int last_num;
		u8 ds1_stat;			//1��״̬
		u8 ds2_stat;			//2��״̬
		u8 ds3_stat;			//3��״̬
		u8 ds4_stat;			//4��״̬
};
extern struct led_message ds_ms;

//LED�˿ڶ���
#define LED0 PEout(14)	// LED1
#define LED1 PEout(15)	// LED2
#define DS1	 PAout(0)
#define DS2  PAout(1)
#define DS3  PAout(2)
#define DS4  PAout(3)

void LED_Init(void);//��ʼ��
u8 get_led_num(void);
u8 led_contrl(void);

#endif
