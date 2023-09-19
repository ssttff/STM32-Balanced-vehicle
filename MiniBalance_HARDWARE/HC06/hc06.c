#include "hc06.h"


u8 res;  				  //设置全局变量
void HC06_Init(u16 arr)  
{  
    GPIO_InitTypeDef GPIO_InitStrue;  
    USART_InitTypeDef USART_InitStrue;  
    NVIC_InitTypeDef NVIC_InitStrue;  
      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //GPIO端口使能   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口端口使能  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB,&GPIO_InitStrue);  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_11;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB,&GPIO_InitStrue);  
      
    USART_InitStrue.USART_BaudRate=arr;  
    USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  
    USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;  
    USART_InitStrue.USART_Parity=USART_Parity_No;  
    USART_InitStrue.USART_StopBits=USART_StopBits_1;  
    USART_InitStrue.USART_WordLength=USART_WordLength_8b;  
      
    USART_Init(USART3,&USART_InitStrue);
      
    USART_Cmd(USART3,ENABLE);					//ʹ�ܴ���2  
      
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//���������ж�  
      
    NVIC_InitStrue.NVIC_IRQChannel=USART3_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
      
}  
  
void USART3_IRQHandler(void)  
{  

     if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)  
 {  
     res= USART_ReceiveData(USART3); 	 
     USART_SendData(USART3,res);    //串口3发送数据给蓝牙模块接收，也就是手机app接收到的数据
  }  
}  

void Bluetooth_reception_control(void){ //蓝牙接收信号控制函数
	
	    extern 	u8 res;
	
		if(res=='1')//小车前进
		{
			Target_Velocity=100;
				LED0=0;
			res=0;
		}
		
		if(res=='2')//小车加速前进
		{
			Target_Velocity=600;
			res=0;
		}
		
		if(res=='3')//小车后退
		{
			Target_Velocity=-100;
				LED1=0;
			res=0;
		}
		
		if(res=='4')//小车加速后退
		{
			Target_Velocity=-600 ;
			res=0;
		}
		
		if(res=='5')//小车右转
		{
	    	Yaw_target=30;
			res=0;
		}
		
		if(res=='6')//小车左转
		{
			Yaw_target=-30;
			res=0;
		}
		
		if(res=='7')//
		{
			Yaw_target=60;
			res=0;
		}
		
		if(res=='8')//
		{
			Yaw_target=-60;
			res=0;
		}
		
		if(res=='0')//小车停止
		{
			Target_Velocity=0;
			Yaw_target=0;
			res=0;
		}
		
}




