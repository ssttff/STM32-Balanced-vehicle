#ifndef __HC06_H
#define __HC06_H
#include "sys.h"

//#define LED0 PCout(13)

void HC06_Init(u16 arr);//���ڳ�ʼ��
void USART3_IRQHandler(void);
void Bluetooth_reception_control(void);
		 				    
#endif




