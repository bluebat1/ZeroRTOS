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
ECB * s_msg;			//信号量
ECB * m_msg;			//互斥信号量
ECB * q_msg;			//消息队列
void * MsgGrp[255];     //消息队列存储地址,最大支持255个消息

//因为Task1和Task2任务开始时，都有系统延时，所以Task3开始获取cpu，Task3申请了互斥信号量，200ms后Task1抢占了cpu
//Task3优先级比Task1低，为了防止优先级反转，Task3的优先级会被提升，使得Task2即使250ms就绪后不能抢占Task3，最后
//Task3释放互斥信号量，使得Task1能进入就绪状态，接着Task2也会在Task1让出cpu时获取cpu，现象为led0灯熄灭时led1灯同时开始闪烁
//Task3释放互斥信号量后会删除自身，Task2发送消息到消息队列，Task1会获取消息。
//最终现象为：开始只有LED0亮着(任务3)，一会后，LED0灯熄灭(任务1)同时LED1灯开始闪烁5次(任务2)，然后常亮，并向任务1发送一条消息。
//LED1常亮是因为请求了一条信号量，自身被挂起。10*400ms后，任务1释放一条信号量，并接受一条消息，使得屏幕出现接受到的消息。
//此时，任务2因为得到信号量而恢复，LED1灯开始闪烁
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
void delay(unsigned int j )//无意义延时  
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
	malloc_init();//内存初始化
	delay_init();
	uart_init(115200);
	printf("OS_init\r\n");
	s_msg=OS_SemCreate(0);
	m_msg =OS_MutexCreate();	//创建互斥信号量
	q_msg=OS_QCreate(MsgGrp,255);//创建消息队列
	printf("OS_create\r\n");
	OS_Start(); 
	
	while(1){
		
		
	}

}




