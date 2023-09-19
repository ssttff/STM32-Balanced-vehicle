#include "control.h"	


int Balance_Pwm,Velocity_Pwm,Turn_Pwm;   //ֱ�������ٶȻ���pwm���
u8 Flag_Target;          //�жϱ�־



//ֱ����
int balance(float Angle,float Gyro){
	float Bias;
	int balance;
	
	Bias=Angle-ZHONGZHI;
	balance=Bias*Kzp+Gyro*Kzd;
	return balance;
	
}

//�ٶȻ�
int velocity(int Encoder_left,int Encoder_right){
	static float Velocity, Encoder_Least, Encoder;
	static float Encoder_Integral;

	Ksi=Ksp/200;
	
	Encoder_Least = (Encoder_Left+Encoder_Right)-0;
	Encoder=Encoder*0.7+Encoder_Least*0.3;
	Encoder_Integral=Encoder_Integral+Encoder;                      
	Encoder_Integral=Encoder_Integral-Target_Velocity; //�ı�Target_Velocity���ǿ���С��ǰ������              
	if(Encoder_Integral>15000)  	Encoder_Integral=15000;        
	if(Encoder_Integral<-15000) 	Encoder_Integral=-15000; 
  	
	Velocity=Encoder*Ksp+Encoder_Integral*Ksi;           
	return Velocity;
	
	
}
//ת��
int Turn(float yaw,float Gyro)
{
	  float Turn;     
	  float Bias;//Ŀ��Ƕ�	  
      Ktd=Ktp/100;
	  Bias=yaw-0;
	  Turn=Bias*Ktp+Gyro*Ktd;
	  return Turn;
}

int EXTI9_5_IRQHandler(void) 
{    
	 if(PBin(5)==0)		
	{   
		 EXTI->PR=1<<5;                                               //���LINE5�ϵ��жϱ�־λ   
		 Flag_Target=!Flag_Target;
		  if(Flag_Target==1)                                        //5ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ�����ߵĲ���Ƶ�ʿ��Ը��ƿ������˲��ͻ����˲���Ч��
			{  
			Get_Angle();                                           //===������̬	
			return 0;	                                               
			}                                                      //10ms����һ�Σ�Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
			Encoder_Left=-Read_Encoder(3);  			//===��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
			Encoder_Right=Read_Encoder(4);
			if(Encoder_Right<0)  Encoder_Right=Encoder_Right+2;		
			if(Encoder_Right>0) Encoder_Right=Encoder_Right-2;  
	   
	  	Get_Angle();                                                        //===������̬	                                                            //===ɨ�谴��״̬ ����˫�����Ըı�С������״̬
 		Balance_Pwm =balance(Angle_Balance,Gyro_Balance);                   //===ƽ��PID����	
		Velocity_Pwm=velocity(Encoder_Left,Encoder_Right);                  //===�ٶȻ�PID����	 ��ס���ٶȷ�����������������С�����ʱ��Ҫ����������Ҫ���ܿ�һ��
 	    Turn_Pwm=Turn(Yaw_target,Gyro_Turn);
		Moto1=Balance_Pwm+Velocity_Pwm-Turn_Pwm;                                     //===�������ֵ������PWM
 	  	Moto2=Balance_Pwm+Velocity_Pwm+Turn_Pwm;                                     //===�������ֵ������PWM
   		Xianfu_Pwm();                                         			//===PWM�޷�
			
        if(Turn_Off(Angle_Balance)==0) {                                     //===����������쳣
 			Set_Pwm(Moto1,Moto2); 
		}

	}       	
	 return 0;	  
} 

void Set_Pwm(int moto1,int moto2){
	
	     int cssd=400;
		PWM1=myabs(moto1)+cssd;
		PWM2=myabs(moto2)+cssd;
	
			if(moto1<0)	{		
				
			TIM_SetCompare1(TIM8,PWM1);
			TIM_SetCompare2(TIM8,0);
			}
			if(moto1>0){	          
		
			TIM_SetCompare1(TIM8,0);
			TIM_SetCompare2(TIM8,PWM1);
			}
	
		    if(moto2<0)	{
	
			TIM_SetCompare3(TIM5,PWM2);
			TIM_SetCompare4(TIM5,0);
			}
			if(moto2>0)  {
	
			TIM_SetCompare3(TIM5,0);
			TIM_SetCompare4(TIM5,PWM2);
			}
}

//�޷�
void Xianfu_Pwm(void)
{	
	int Amplitude=7000;    //===PWM������7200 ������7000
    if(Moto1<-Amplitude) Moto1=-Amplitude;	
    if(Moto1>Amplitude)  Moto1=Amplitude;	
	if(Moto2<-Amplitude) Moto2=-Amplitude;	
	if(Moto2>Amplitude)  Moto2=Amplitude;		
}

//�Ƕȹ���رյ��
u8 Turn_Off(float angle)
{
   u8 temp;
	if(angle<-40||angle>40)
	{	                      
        temp=1;                                   			
		TIM_SetCompare1(TIM8,0);
		TIM_SetCompare2(TIM8,0);
		TIM_SetCompare3(TIM5,0);
		TIM_SetCompare4(TIM5,0);
				
      }
	  else{
      temp=0;
	  }
      return temp;			
}
	
//��ȡmpu6050�Ƕ�ֵ
void Get_Angle(void)
{ 
			Read_DMP();                      //===��ȡ���ٶȡ����ٶȡ����
			Angle_Balance=Pitch;             //===����ƽ�����
	         Angle_Turn=Yaw;
			Gyro_Balance=gyro[1];            //===����ƽ����ٶ�
		    Gyro_Turn=gyro[2];                 //����ת����ٶ�
	
	      Acceleration_Z=accel[2];         //===����Z����ٶȼ�
	Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //��ȡZ��������
}

//����ֵ����
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  temp=-a;  
	else temp=a;
	  return temp;
}

/**************************************************************************
�������ܣ����С���Ƿ�����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right)
{ 		   
	 static u16 flag,count0,count1,count2;
	if(flag==0)                                                                   //��һ��
	 {
	      if(myabs(encoder_left)+myabs(encoder_right)<30)                         //����1��С���ӽ���ֹ
				count0++;
        else 
        count0=0;		
        if(count0>10)				
		    flag=1,count0=0; 
	 } 
	 if(flag==1)                                                                  //����ڶ���
	 {
		    if(++count1>200)       count1=0,flag=0;                                 //��ʱ���ٵȴ�2000ms
	      if(Acceleration>26000&&(Angle>(-20+ZHONGZHI))&&(Angle<(20+ZHONGZHI)))   //����2��С������0�ȸ���������
		    flag=2; 
	 } 
	 if(flag==2)                                                                  //������
	 {
		  if(++count2>100)       count2=0,flag=0;                                   //��ʱ���ٵȴ�1000ms
	    if(myabs(encoder_left+encoder_right)>200)                                 //����3��С������̥��Ϊ�������ﵽ����ת��   
      {
				flag=0;                                                                                     
				return 1;                                                               //��⵽С��������
			}
	 }
	return 0;
}







