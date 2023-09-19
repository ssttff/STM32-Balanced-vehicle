#include "control.h"	


int Balance_Pwm,Velocity_Pwm,Turn_Pwm;   //直立环、速度环的pwm输出
u8 Flag_Target;          //中断标志



//直立环
int balance(float Angle,float Gyro){
	float Bias;
	int balance;
	
	Bias=Angle-ZHONGZHI;
	balance=Bias*Kzp+Gyro*Kzd;
	return balance;
	
}

//速度环
int velocity(int Encoder_left,int Encoder_right){
	static float Velocity, Encoder_Least, Encoder;
	static float Encoder_Integral;

	Ksi=Ksp/200;
	
	Encoder_Least = (Encoder_Left+Encoder_Right)-0;
	Encoder=Encoder*0.7+Encoder_Least*0.3;
	Encoder_Integral=Encoder_Integral+Encoder;                      
	Encoder_Integral=Encoder_Integral-Target_Velocity; //改变Target_Velocity就是控制小车前进后退              
	if(Encoder_Integral>15000)  	Encoder_Integral=15000;        
	if(Encoder_Integral<-15000) 	Encoder_Integral=-15000; 
  	
	Velocity=Encoder*Ksp+Encoder_Integral*Ksi;           
	return Velocity;
	
	
}
//转向环
int Turn(float yaw,float Gyro)
{
	  float Turn;     
	  float Bias;//目标角度	  
      Ktd=Ktp/100;
	  Bias=yaw-0;
	  Turn=Bias*Ktp+Gyro*Ktd;
	  return Turn;
}

int EXTI9_5_IRQHandler(void) 
{    
	 if(PBin(5)==0)		
	{   
		 EXTI->PR=1<<5;                                               //清除LINE5上的中断标志位   
		 Flag_Target=!Flag_Target;
		  if(Flag_Target==1)                                        //5ms读取一次陀螺仪和加速度计的值，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果
			{  
			Get_Angle();                                           //===更新姿态	
			return 0;	                                               
			}                                                      //10ms控制一次，为了保证M法测速的时间基准，首先读取编码器数据
			Encoder_Left=-Read_Encoder(3);  			//===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
			Encoder_Right=Read_Encoder(4);
			if(Encoder_Right<0)  Encoder_Right=Encoder_Right+2;		
			if(Encoder_Right>0) Encoder_Right=Encoder_Right-2;  
	   
	  	Get_Angle();                                                        //===更新姿态	                                                            //===扫描按键状态 单击双击可以改变小车运行状态
 		Balance_Pwm =balance(Angle_Balance,Gyro_Balance);                   //===平衡PID控制	
		Velocity_Pwm=velocity(Encoder_Left,Encoder_Right);                  //===速度环PID控制	 记住，速度反馈是正反馈，就是小车快的时候要慢下来就需要再跑快一点
 	    Turn_Pwm=Turn(Yaw_target,Gyro_Turn);
		Moto1=Balance_Pwm+Velocity_Pwm-Turn_Pwm;                                     //===计算左轮电机最终PWM
 	  	Moto2=Balance_Pwm+Velocity_Pwm+Turn_Pwm;                                     //===计算右轮电机最终PWM
   		Xianfu_Pwm();                                         			//===PWM限幅
			
        if(Turn_Off(Angle_Balance)==0) {                                     //===如果不存在异常
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

//限幅
void Xianfu_Pwm(void)
{	
	int Amplitude=7000;    //===PWM满幅是7200 限制在7000
    if(Moto1<-Amplitude) Moto1=-Amplitude;	
    if(Moto1>Amplitude)  Moto1=Amplitude;	
	if(Moto2<-Amplitude) Moto2=-Amplitude;	
	if(Moto2>Amplitude)  Moto2=Amplitude;		
}

//角度过大关闭电机
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
	
//获取mpu6050角度值
void Get_Angle(void)
{ 
			Read_DMP();                      //===读取加速度、角速度、倾角
			Angle_Balance=Pitch;             //===更新平衡倾角
	         Angle_Turn=Yaw;
			Gyro_Balance=gyro[1];            //===更新平衡角速度
		    Gyro_Turn=gyro[2];                 //更新转向角速度
	
	      Acceleration_Z=accel[2];         //===更新Z轴加速度计
	Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //读取Z轴陀螺仪
}

//绝对值函数
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  temp=-a;  
	else temp=a;
	  return temp;
}

/**************************************************************************
函数功能：检测小车是否被拿起
入口参数：int
返回  值：unsigned int
**************************************************************************/
int Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right)
{ 		   
	 static u16 flag,count0,count1,count2;
	if(flag==0)                                                                   //第一步
	 {
	      if(myabs(encoder_left)+myabs(encoder_right)<30)                         //条件1，小车接近静止
				count0++;
        else 
        count0=0;		
        if(count0>10)				
		    flag=1,count0=0; 
	 } 
	 if(flag==1)                                                                  //进入第二步
	 {
		    if(++count1>200)       count1=0,flag=0;                                 //超时不再等待2000ms
	      if(Acceleration>26000&&(Angle>(-20+ZHONGZHI))&&(Angle<(20+ZHONGZHI)))   //条件2，小车是在0度附近被拿起
		    flag=2; 
	 } 
	 if(flag==2)                                                                  //第三步
	 {
		  if(++count2>100)       count2=0,flag=0;                                   //超时不再等待1000ms
	    if(myabs(encoder_left+encoder_right)>200)                                 //条件3，小车的轮胎因为正反馈达到最大的转速   
      {
				flag=0;                                                                                     
				return 1;                                                               //检测到小车被拿起
			}
	 }
	return 0;
}







