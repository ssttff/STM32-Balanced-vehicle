#include "stm32f10x.h"
#include "sys.h"


int Encoder_Left,Encoder_Right;             //���ұ��������������
int Moto1,Moto2;                            //���PWM���� Ӧ��Motor�� ��Moto�¾�	
int PWM1,PWM2;
float Angle_Balance,Gyro_Balance,Gyro_Turn,Angle_Turn,Acceleration_Z,Gyro_Z; //ƽ����� ƽ�������� ת��������

float Kzp=588,Kzd=1.4,Ksp=130,Ksi;
//float Kzp=980*0.6,Kzd=1.4*0.6,Ksp=130,Ksi;
float Ktp=-40,Ktd;
int Yaw_target=0;//ת��
float Target_Velocity=0;//�ٶȣ�ǰ���ƶ�

int main(void){
	
	u8 string[10] = {0};
	
	Stm32_Clock_Init(9);            //=====ϵͳʱ������
	delay_init();                 //=====��ʱ��ʼ��
	JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
	LED_Init(); 
	KEY_Init();
	OLED_Init();                    //=====OLED��ʼ��
	uart_init(115200);         
	TIM5_PWM_Init(7199,0);   //=====��ʼ��PWM 10KHZ������������� �����ʼ������ӿ� ��ΪMiniBalance_PWM_Init(9999,35) 200HZ
	TIM8_PWM_Init(7199,0); 
	Encoder_Init_TIM3();            //=====�������ӿ�
	Encoder_Init_TIM4();            //=====��ʼ��������2 
	IIC_Init();                     //=====ģ��IIC��ʼ��
	MPU6050_initialize();           //=====MPU6050��ʼ��	
	DMP_Init();                     //=====��ʼ��DMP      
	MiniBalance_EXTI_Init(); 
	HC06_Init(9600);
	OLED_Clear(0);
	
	while(1)
	   {
 
		   Key_Set(); 				//��������pid����
		Bluetooth_reception_control();
		OLED_ShowString(40,0,"MPU6050",14);
		   sprintf((char *)string,"Pitch:%.2f",Angle_Balance);
		OLED_ShowString(0,1,string,12);
		   sprintf((char *)string,"Left%d",Encoder_Left);
		OLED_ShowString(0,2,string,12);
		   sprintf((char *)string,"Right%d",Encoder_Right);
		OLED_ShowString(60,2,string,12);
		   sprintf((char *)string,"Kzp:%.1f",Kzp);
		OLED_ShowString(0,4,string,12);
		   sprintf((char *)string,"Kzd:%.3f",Kzd);
		OLED_ShowString(0,5,string,12);
		   sprintf((char *)string,"Ksp:%.1f",Ksp);
		OLED_ShowString(0,6,string,12);
		   sprintf((char *)string,"Ksi:%.3f",Ksi);
		OLED_ShowString(0,7,string,12);
			 
		
		printf("ƽ�����  %f \r\nƽ����ٶ� %f \r\n���ֱ�����%d \r\n���ֱ�����%d \r\n",Angle_Balance,Gyro_Balance,Encoder_Left,Encoder_Right);//�򴮿ڵ������ַ�������
	  	printf("Kzp %f  \r\n Kzd %f  \r\n Ksp %f  \r\n Ksi %f \r\n",Kzp,Kzd,Ksp,Ksi);
		printf("���1��pwm :%d \r\n ���2��pwm :%d  \r\n",PWM1,PWM2);


		delay_ms(1000);	                //��ʱ�������ݴ���Ƶ�ʣ�ȷ��ͨ�ŵ��ȶ�

		 } 
	
}




