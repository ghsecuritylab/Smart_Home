#ifndef __ESP_H
#define __ESP_H
#include "sys.h"
#include "delay.h"

#define USART3_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART3_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART3_RX_EN 			1								//0,������;1,����.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 USART3_RX_STA;   											//��������״̬

/* ONENET SERVER INFO */
#define	ONENET_SERVER	"183.230.10.39"
#define ONENET_PORT		6002

/* esp8266��ǰ״̬ */
#define WIFI_INIT			1<<0		//WiFi�ѳ�ʼ��
#define WIFI_GOTIP		1<<1		//WiFi������AP�����IP��ַ
#define WIFI_CONNECT	1<<2		//�ѽ���TCP��UDP����
#define WIFI_CLOSED		1<<3		//�Ͽ���������
#define WIFI_NOIP			1<<4		//�Ͽ�AP����
#define WIFI_ERROR		1<<5		//��������

struct esp_message
{
		u8 stat;						//esp8266ģ��״̬
		char *ip;						//esp8266ģ��ip��ַ
		char *mac_addr;			//esp8266ģ��MAC��ַ
		
		char *ssid;					//esp8266ģ�����AP
		char *passwd;
	
		int socket;					//esp8266ģ��sockect�ӿ�
		char *remote_ip;		//esp8266ģ��Զ�˵�ַ
		char *remote_port;	//esp8266ģ��Զ�˶˿�
};

extern struct esp_message wifi_netdev;

void Usart3_Init(u32 bound);	//����3��ʼ��
void Esp8266_Init(void);			//esp8266��ʼ��

extern s8 sendAT(char *sendStr,char *searchStr,u32 outTime);//����ATָ���
extern void cleanReceiveData(void);    //�������������
extern char * my_strstr(char *FirstAddr,char *searchStr);	//strstr����

void u3_printf(char* fmt,...);		//����2�������ݺ���

u8 esp_connect_ap(char *ssid, char *passwd);		//����AP
u8 esp_tcp_connect(void);

void netdev_set_info(void);		//����������Ϣ
void esp_check_stat(void);		//check wifi stat
u8 send_connect_pkgs(void);		//�����������ݰ�
u8 send_data(char *stream, char *data);

#endif




