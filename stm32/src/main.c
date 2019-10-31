#include "stm32f10x.h"
#include "os.h"
#include <stdio.h>
#include "delay.h"
#include "misc.h"
#include "usart.h"

#define TASK_1_STK_SIZE 1024
#define TASK_2_STK_SIZE 1024
#define TASK_3_STK_SIZE 1024
unsigned int TASK_1_STK[TASK_1_STK_SIZE];
unsigned int TASK_2_STK[TASK_2_STK_SIZE];
unsigned int TASK_3_STK[TASK_3_STK_SIZE];
ECB * s_msg;			//�ź���
ECB * m_msg;			//�����ź���
ECB * q_msg;			//��Ϣ����
void * MsgGrp[255];     //��Ϣ���д洢��ַ,���֧��255����Ϣ

//��ΪTask1��Task2����ʼʱ������ϵͳ��ʱ������Task3��ʼ��ȡcpu��Task3�����˻����ź�����200ms��Task1��ռ��cpu
//Task3���ȼ���Task1�ͣ�Ϊ�˷�ֹ���ȼ���ת��Task3�����ȼ��ᱻ������ʹ��Task2��ʹ250ms����������ռTask3�����
//Task3�ͷŻ����ź�����ʹ��Task1�ܽ������״̬������Task2Ҳ����Task1�ó�cpuʱ��ȡcpu������Ϊled0��Ϩ��ʱled1��ͬʱ��ʼ��˸
//Task3�ͷŻ����ź������ɾ������Task2������Ϣ����Ϣ���У�Task1���ȡ��Ϣ��
//��������Ϊ����ʼֻ��LED0����(����3)��һ���LED0��Ϩ��(����1)ͬʱLED1�ƿ�ʼ��˸5��(����2)��Ȼ��������������1����һ����Ϣ��
//LED1��������Ϊ������һ���ź�������������10*400ms������1�ͷ�һ���ź�����������һ����Ϣ��ʹ����Ļ���ֽ��ܵ�����Ϣ��
//��ʱ������2��Ϊ�õ��ź������ָ���LED1�ƿ�ʼ��˸
void Task1(void)
{
	static unsigned char i=0;
	OSTimeDly(200);
	printf("thread 1 init \r\n");
	while(1)
	{
		printf("thread 1 loop \r\n");
		OSTimeDly(400);
	}
}
void Task2(void)
{
	unsigned char i=0;
	void *p;
	OSTimeDly(250);
	p=Z_malloc(8);
	sprintf(p,"By_KITE");
	printf("thread 2 init \r\n");
	while(1)
	{

		printf("thread 2 loop \r\n");
		OSTimeDly(150);
	}
}
void delay(unsigned int j )//��������ʱ  
{  
    unsigned int i = 0;  
    unsigned int k = j;  
    for(i=0;i<500;i++)  
    {  
        while(--j);  
        j=k;  
    }  
}
void Task3(void)
{
	printf("thread 3 init \r\n");
	while(1)
	{
		printf("thread 3 loop \r\n");
		OSTimeDly(250);
	}
}
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Task_Create(Task1,&TASK_1_STK[TASK_1_STK_SIZE-1],0);
	Task_Create(Task2,&TASK_2_STK[TASK_2_STK_SIZE-1],1);
	Task_Create(Task3,&TASK_3_STK[TASK_3_STK_SIZE-1],2);
	malloc_init();//�ڴ��ʼ��
	delay_init();
	uart_init(115200);
	printf("OS_init\r\n");
	s_msg=OS_SemCreate(0);
	m_msg =OS_MutexCreate();	//���������ź���
	q_msg=OS_QCreate(MsgGrp,255);//������Ϣ����
	printf("OS_create\r\n");
	OS_Start(); 
	
	while(1){
		
		
	}

}




