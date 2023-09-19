#include "motor.h"
// TIM5 PA2 PA3
// TIM8 PC6 PC7



void TIM5_PWM_Init(u16 arr,u16 psc){

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //TIM5_CH3 //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period=arr;
	TIM_TimeBaseStructure.TIM_Prescaler=psc;
	TIM_TimeBaseStructure.TIM_ClockDivision=0;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);  
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);  
	
	TIM_CtrlPWMOutputs(TIM5,ENABLE);	//MOE �����ʹ��	

	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��	 
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM5, ENABLE); //ʹ��TIM5��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM5
	
}


void TIM8_PWM_Init(u16 arr,u16 psc){
	
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitstruct;
	
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8 ,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC  , ENABLE); //ʱ��ʹ��

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 
 
	TIM_OCInitstruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitstruct.TIM_OCNPolarity=TIM_OCNPolarity_High;
	TIM_OCInitstruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitstruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitstruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitstruct.TIM_OCNIdleState=TIM_OCNIdleState_Reset;

	TIM_OC1Init(TIM8,&TIM_OCInitstruct);
	TIM_OC2Init(TIM8,&TIM_OCInitstruct);
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE �����ʹ��	 
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIM3��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM3
	TIM_CtrlPWMOutputs(TIM8, ENABLE);// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ s
	
}




