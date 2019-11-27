/************************************************************
 ** ��  Ŀ  ��  �ƣ����������������������ƶ�ϵͳ(���ܵƲ���)
 **
 ** �� Ƭ �� �� �ţ�STM32F407VGT6
 **
 ** ��  ��  ��  �ƣ�esp8266.c
 **
 ** ��    ��    �ˣ�ȫ���
 **
 ** ��          �������ܵ�ϵͳwifi��غ����ļ�
 **
 ** �ļ� ���� ���ڣ�2019��8��12��
 **
 ** ��־��
 ** Data							Author					Note
 @2019/8/12					  ȫ���					�������ļ�
************************************************************/
/*									Include Head File														*/
#include "esp8266.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "my_math.h"
#include "usart.h"
#include "timer.h"
#include "led.h"

/* wifi��Ϣ�ṹ */
struct esp_message wifi_netdev;

//���ڽ��ջ�����
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 			//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART3_RX_STA=0;

void Usart3_Init(u32 bound)
{
    NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;

	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��

		USART_DeInit(USART3);  //��λ����3
	
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3	
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB11��GPIOB10��ʼ��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
		GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��GPIOB11����GPIOB10
	
		USART_InitStructure.USART_BaudRate = bound;//������ 
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
		
		USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	 
		USART_Cmd(USART3, ENABLE);               //ʹ�ܴ��� 
		
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
		
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		TIM7_Int_Init(100-1,8400-1);		//10ms�ж�
		USART3_RX_STA=0;		//����
		TIM_Cmd(TIM7, DISABLE); //�رն�ʱ��7
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)
{
    u16 i,j;
    va_list ap;
    va_start(ap,fmt);
    vsprintf((char*)USART3_TX_BUF,fmt,ap);
    va_end(ap);
    i=strlen((const char*)USART3_TX_BUF);		//�˴η������ݵĳ���
    for(j=0; j<i; j++)							//ѭ����������
    {
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������
        USART_SendData(USART3,USART3_TX_BUF[j]);
    }
}


//����3�жϷ�����
void USART3_IRQHandler(void)
{
		u8 res;	    
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
		{
				res =USART_ReceiveData(USART3);
				if((USART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
				{
						if(USART3_RX_STA<USART3_MAX_RECV_LEN)		//�����Խ�������
						{
								TIM_SetCounter(TIM7,0);//���������
								if(USART3_RX_STA==0)
										TIM_Cmd(TIM7, ENABLE);  //ʹ�ܶ�ʱ��7 
								USART3_RX_BUF[USART3_RX_STA++]=res;		//��¼���յ���ֵ	 
						}else 
						{
								USART3_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
						}
				}
		}
}

//esp8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* esp_check_cmd(u8 *str)
{
		char *strx=0;
		if(USART3_RX_STA&0X8000)		//���յ�һ��������
		{
				USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
				strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
		}
		return (u8*)strx;
}

//��esp8266��������
//cmd:���͵������ַ���(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 esp_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
		u8 res=0; 
		USART3_RX_STA=0;
		u3_printf("%s\r\n",cmd);	//��������
		if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
		{
				while(--waittime)	//�ȴ�����ʱ
				{
						delay_ms(10);
						if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
						{
								if(esp_check_cmd(ack))break;//�õ���Ч���� 
								USART3_RX_STA=0;
						}
				}
				if(waittime==0)res=1; 
		}
		return res;
}

//���������
//��������
//����ֵ ��
void cleanReceiveData(void)
{
    USART3_RX_STA=0;			//���ռ���������
		memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
}

/* ���ӷ����� */
u8 esp_tcp_connect(void)
{
		u8 ret = 0;
		char past[50];
		memset(past, 0, sizeof(past));
		sprintf((char *)past, "AT+CIPSTART=\"TCP\",\"%s\",%s", wifi_netdev.remote_ip, wifi_netdev.remote_port);
		ret = esp_send_cmd((u8 *)past, (u8 *)"OK", 1000);
		if (ret == 0)
		{
			 wifi_netdev.stat = WIFI_CONNECT;
		}
		else wifi_netdev.stat  = WIFI_CLOSED;
		return ret;
}

void netdev_set_info(void)
{
		char *ret;
		esp_send_cmd((u8 *)"AT+CIFSR", (u8 *)"+CIFSR", 500);		/* ��ѯip��ַ */
		while((USART3_RX_STA&0X8000)==0);
//		printf("%c\r\n", USART3_RX_BUF[25]);
//		printf("%c\r\n", USART3_RX_BUF[35]);
//		ret = strstr((char *)USART3_RX_BUF, "\"");
//		printf("%s\r\n", ret);
//		ret++;
//		ret = strstr((char *)ret, "\r");
//		printf("%s\r\n", ret);
		delay_ms(100);
		
		/*	
		wifi_netdev.ip = 
		
		*/
}

void esp_check_stat(void)
{
		if(esp_send_cmd((u8 *)"AT+CIPSTATUS", (u8 *)"OK", 500) == 1)
		{
				wifi_netdev.stat = WIFI_ERROR;
				printf("״̬������\r\n");
		}
		else
		{
				printf("�õ�ģ��״̬\r\n");
				printf("%s\r\n", USART3_RX_BUF);
				switch(USART3_RX_BUF[22])
				{
						case '2':
								wifi_netdev.stat = WIFI_GOTIP;
								esp_tcp_connect();
						break;
						case '3':
								wifi_netdev.stat = WIFI_CONNECT;
						break;
						case '4':
								wifi_netdev.stat = WIFI_CLOSED;
								esp_tcp_connect();
						break;
						case '5':
								wifi_netdev.stat = WIFI_NOIP;
								esp_connect_ap(wifi_netdev.ssid, wifi_netdev.passwd);
								esp_tcp_connect();
						break;
						default:
								wifi_netdev.stat = WIFI_ERROR;
								Esp8266_Init();
						break;
				}
		}
		printf("wifi stat: %d\r\n", wifi_netdev.stat);
		delay_ms(100);
}

u8 esp_connect_ap(char *ssid, char *passwd)
{
		u8 ret = 0;
		char past[100];
		memset(past, 0, sizeof(past));
		sprintf((char *)past, "AT+CWJAP_DEF=\"%s\",\"%s\"",ssid,passwd);
		if(esp_send_cmd((u8 *)past, (u8 *)"OK",1000)==0)
    {
				ret = 0;
        printf("·�������ӳɹ�\r\n");
				wifi_netdev.stat = WIFI_GOTIP;
				wifi_netdev.ssid = ssid;
				wifi_netdev.passwd = passwd;
    }
		else
		{
				ret = 1;
				printf("·��������ʧ��\r\n");
				wifi_netdev.stat = WIFI_ERROR;
		}
		return ret;
}

/* CONNECT HEADER */
const char CONNECT_HEAD_PKG1[] = {0x10, 0x2A, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54};
const char CONNECT_HEAD_PKG2[] = {0x04, 0xC0, 0x00, 0x78, 0x00, 0x09};
const char CONNECT_HEAD_PKG3[] = {0x00, 0x06};
const char CONNECT_HEAD_PKG4[] = {0x00, 0x0B};

/* ��Ȩ��Ϣ������Ϣ */
const char DEVICE_ID[] = "549670756";
const char PRODUCT_ID[] = "278596";
const char AUTH_INFO[] = "52180720134";
const char USER_ID[] = "63024";

u8 send_connect_pkgs(void)
{
		u16 i = 0;
		u8 ret = 0;
		u8 data_buf[100];
		
//		esp_send_cmd((u8 *)"AT+CIPMODE=1", (u8 *)"OK", 500);
		
		memset(data_buf, 0, sizeof(data_buf));
		for(int j=0; j<sizeof(CONNECT_HEAD_PKG1); )
		{
				data_buf[i++] = CONNECT_HEAD_PKG1[j++];
		}
		for(int j=0; j<sizeof(CONNECT_HEAD_PKG2); )
		{
				data_buf[i++] = CONNECT_HEAD_PKG2[j++];
		}
		for(int j=0; j<sizeof(DEVICE_ID)-1; )
		{
				data_buf[i++] = DEVICE_ID[j++];
		}
		for(int j=0; j<sizeof(CONNECT_HEAD_PKG3); )
		{
				data_buf[i++] = CONNECT_HEAD_PKG3[j++];
		}
		for(int j=0; j<sizeof(PRODUCT_ID)-1; )
		{
				data_buf[i++] = PRODUCT_ID[j++];
		}
		for(int j=0; j<sizeof(CONNECT_HEAD_PKG4); )
		{
				data_buf[i++] = CONNECT_HEAD_PKG4[j++];
		}
		for(int j=0; j<sizeof(AUTH_INFO)-1; )
		{
				data_buf[i++] = AUTH_INFO[j++];
		}
		ret = esp_send_cmd((u8 *)"AT+CIPSEND=46", (u8 *)">", 500);
		for(i = 0; i<44; i++)
		{
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������
				USART_SendData(USART3, data_buf[i]);
				printf("%02x", data_buf[i]);
		}
		u3_printf("\r\n");
		return ret;
}

const char DATA_HEAD_PKG1[] = {0x30};
const char DATA_HEAD_PKG2[] = {0x24, 0x64, 0x70, 0x03};

u8 send_data(char *stream, char *data)
{
		int i = 0,j = 0;
		int n = 0;
		u8 ret = 0;
		u8 past[128];
		u8 data_buf[50];
		u8 stream_len = strlen(stream);
		u8 data_len = strlen(data);
		memset(data_buf, 0, sizeof(data_buf));
		printf("stream_len: %d\r\n", stream_len);
		printf("data_len: %d\r\n", data_len);
		
		if(wifi_netdev.stat != WIFI_CONNECT)
		{
				/* ���綪ʧ������ʧ�� */
				LED1 = 1;
				delay_ms(10);
				LED1 = 0;
				delay_ms(10);
				ret = 2;
				return ret;
		}
		
		data_buf[0] = 0x30;
		data_buf[1] = stream_len + data_len + 13;
		data_buf[2] = 0x00;
		data_buf[3] = 0x03;
		for(i=0;i<4;i++)
		{
				data_buf[i+4] = DATA_HEAD_PKG2[i];
		}
		data_buf[8] = (stream_len + data_len + 5) / 255;
		data_buf[9] = (stream_len + data_len + 5) % 255;
		data_buf[10] = '{';
		data_buf[11] = '"';
		for(i=0;i<stream_len;i++)
		{
			 data_buf[12+i] = stream[i];
		}
		data_buf[12+i] = '"';
		data_buf[13+i] = ':';
		for(j=0;j<data_len;j++)
		{
			 data_buf[14+i+j] = data[j];
		}
		data_buf[14+i+j] = '}';
		for (n = 0;n<stream_len+data_len+15;n++)
		{
				printf("%02x", data_buf[n]);
		}
		printf("\r\n");
		memset(past, 0, sizeof(past));
		sprintf((char *)past, "AT+CIPSEND=%d", stream_len+data_len+15);
		ret = esp_send_cmd(past, (u8 *)">", 500);
		for(i = 0; i<stream_len+data_len+15; i++)
		{
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������
				USART_SendData(USART3, data_buf[i]);
				printf("%02x", data_buf[i]);
		}
		u3_printf("\r\n");
		u3_printf("\r\n");
		LED1 = 1;
		delay_ms(10);
		LED1 = 0;
		delay_ms(10);
		LED1 = 1;
		delay_ms(10);
		LED1 = 0;
		delay_ms(10);
		delay_ms(100);
}

char SSID[] = {"Kiven"};		//·����SSID
char password[] = {"asd123456"};	//·��������
char ipaddr[]= {"183.230.40.39"};//IP��ַ
char port[]= {"6002"};				//�˿ں�

//ESP8266ģ���ʼ��
void Esp8266_Init(void)
{
    u8 i;
    u8 ret;
		char past[100];			//�������ݻ���
    Usart3_Init(115200);		//��ʼ������3������Ϊ115200
		printf("��ʼ��ʼ��\r\n");
    cleanReceiveData();		//��ս������ݻ���
    esp_send_cmd((u8 *)"AT+RST", (u8 *)"ready",3000);
    delay_ms(2000);
    u3_printf("AT+CWMODE=1\r\n");
		wifi_netdev.stat = WIFI_INIT;
    delay_ms(1000);		//�ȴ�ģ���ϵ��ȶ�
    
    printf("��ʼ���ɹ�\r\n");
		memset(past, 0, sizeof(past));
    sprintf((char *)past, "AT+CWJAP_DEF=\"%s\",\"%s\"",SSID,password);
    if(esp_send_cmd((u8 *)past, (u8 *)"OK",1000)==0)
    {
        printf("·�������ӳɹ�\r\n");
				wifi_netdev.stat = WIFI_GOTIP;
    }
		else
		{
				ret = 1;
				printf("·��������ʧ��\r\n");
				wifi_netdev.stat = WIFI_ERROR;
				goto __exit;
		}
		delay_ms(500);
    i = 0;
		memset(past, 0, sizeof(past));
		sprintf((char *)past, "AT+CWAUTOCONN=1");
		esp_send_cmd((u8 *)past, (u8 *)"OK", 500);
    if(esp_send_cmd((u8 *)"AT+CIPMUX=0", (u8 *)"OK",1000) ==0)		//���õ�����ģʽ
    {
        ret = 0;
        printf("������ģʽ���óɹ�\r\n");
    }
		else
		{
				ret = 1;
        printf("������ģʽ����ʧ��\r\n");
				goto __exit;
		}
		ret = 0;
		memset(past, 0, sizeof(past));
		sprintf((char *)past, "AT+CIPSTART=\"TCP\",\"%s\",%s", ipaddr, port);
    while((esp_send_cmd((u8 *)past, (u8 *)"OK", 1000)) != 0)
    {
				printf("��%d������������...\n", i++);
        if(i>=10)
				{
						ret = 1;
						goto __exit;
				}
				delay_ms(500);
    }
__exit:
		if(ret == 0)
		{
				printf("���ӷ������ɹ�\r\n");
				wifi_netdev.remote_ip = ipaddr;
				wifi_netdev.remote_port = port;
				wifi_netdev.stat = WIFI_CONNECT;
				send_connect_pkgs();
				LED0 = 1;
		}
		else printf("���ӷ�����ʧ��\r\n");
}


