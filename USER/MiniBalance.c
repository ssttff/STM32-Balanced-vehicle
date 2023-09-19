#include "stm32f10x.h"
#include "sys.h"


int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
int Moto1,Moto2;                            //电机PWM变量 应是Motor的 向Moto致敬	
int PWM1,PWM2;
float Angle_Balance,Gyro_Balance,Gyro_Turn,Angle_Turn,Acceleration_Z,Gyro_Z; //平衡倾角 平衡陀螺仪 转向陀螺仪

float Kzp=588,Kzd=1.4,Ksp=130,Ksi;
//float Kzp=980*0.6,Kzd=1.4*0.6,Ksp=130,Ksi;
float Ktp=-40,Ktd;
int Yaw_target=0;//转向
float Target_Velocity=0;//速度，前后移动

int main(void){
	
	u8 string[10] = {0};
	
	Stm32_Clock_Init(9);            //=====系统时钟设置
	delay_init();                 //=====延时初始化
	JTAG_Set(JTAG_SWD_DISABLE);     //=====关闭JTAG接口
	JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
	LED_Init(); 
	KEY_Init();
	OLED_Init();                    //=====OLED初始化
	uart_init(115200);         
	TIM5_PWM_Init(7199,0);   //=====初始化PWM 10KHZ，用于驱动电机 如需初始化电调接口 改为MiniBalance_PWM_Init(9999,35) 200HZ
	TIM8_PWM_Init(7199,0); 
	Encoder_Init_TIM3();            //=====编码器接口
	Encoder_Init_TIM4();            //=====初始化编码器2 
	IIC_Init();                     //=====模拟IIC初始化
	MPU6050_initialize();           //=====MPU6050初始化	
	DMP_Init();                     //=====初始化DMP      
	MiniBalance_EXTI_Init(); 
	HC06_Init(9600);
	OLED_Clear(0);
	
	while(1)
	   {
 
		   Key_Set(); 				//按键设置pid参数
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
			 
		
		printf("平衡倾角  %f \r\n平衡角速度 %f \r\n左轮编码器%d \r\n右轮编码器%d \r\n",Angle_Balance,Gyro_Balance,Encoder_Left,Encoder_Right);//向串口调试助手发送数据
	  	printf("Kzp %f  \r\n Kzd %f  \r\n Ksp %f  \r\n Ksi %f \r\n",Kzp,Kzd,Ksp,Ksi);
		printf("电机1的pwm :%d \r\n 电机2的pwm :%d  \r\n",PWM1,PWM2);


		delay_ms(1000);	                //延时减缓数据传输频率，确保通信的稳定

		 } 
	
}




