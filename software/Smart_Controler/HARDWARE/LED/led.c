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


//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_8 | GPIO_Pin_9);//GPIOF9,F10���øߣ�����

}





