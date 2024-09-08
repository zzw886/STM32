
/**
  *		��ݴ���Ŀǰ���кܶ�ط�ûд��
  *     �Լ��ܶ�ط�û�з����дע�ͣ���ֱ��һ��(��������)
  *		д��;�������˺ܶ�С����
  *		��֮����ʱ���ٸ��°�
  *     �汾V1.15
  **/
  
#include "stm32f10x.h"    
#include "OLED.h"     
#include "Delay.h"  
#include "menu.h"
#include "AD.h"

#include "Serial2.h"  
#include "string.h"                          //�ַ����жϺ�����
uint8_t  Up=0 , Down=0 , Back=0 , Enter=0 ; //�������ⰴ�����ϣ��£����أ�ȷ�� //0Ϊδ���£�1Ϊ����//��ʱδʹ��
uint8_t  connect=0 ;                       //���ӱ�־λ��1Ϊ���ӳɹ���0Ϊδ����

#include "inv_mpu.h"            //mpu6050�����Ǵ�����
float Pitch,Roll,Yaw;          //����3������Ƕȵı���
short gyroX,gyroY,gyroZ,accelX,accelY,accelZ; 
uint8_t TLYPW=0;             //�����ǿ���״̬��־λ


#include "key.h"

#include "MyRTC.h"
#include "biaopan.h"

#include "MP3_tf.h"
#include "W25Q128_SPI1.h"



void STM32_rouse(void);
void MP3_USART(void);

extern uint8_t GUODUsn;
int main(void)
{
	    Key_Init();
	  
		delay_init();												       //��ʱ������ʼ��
	
		OLED_Init();                                          		     //oled��ʾ����ʼ��
	
		OLED_WriteCommand(0x81);	//���öԱȶ�
		OLED_WriteCommand(0x00);	//0x00~0xFF
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);   //��ӳ������,��PB3��PB4��PA15����ͨIO��PA13&14����SWD����
			
		biaopan_io_init();
	
// 		MPU6050_DMP_Init();                       	  			//�����ǳ�ʼ��
		
     	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVICΪ����2
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
		AD_Init();				                      //ADת������ʼ��
		
        MyRTC_Init();							    //RTC��ʼ��

		STM32_rouse();                            //�����������õ��ж�����
		
		int8_t p;                               //�洢����ֵ����
		
		uint8_t XIUMIAN=0;                    //��ʱ������Ļ����ʱ�����
		
		W25Q128_Init();
		
		
		GUODUsn=1;
		   while(GUODUsn)			
		   { 
			   biaopan();
				delay_ms(4);		   
			   OLED_transition(1);	   
		   }
	extern uint8_t taiwan ,dgh;
	short gyro[3];
	unsigned long timestamp ;


   while(1)
   {  	
	   

		 biaopan();  //ˢ�±���
	   
   
	   p = menu_Roll_event();	//��ȡ����
	   
	   if(XIUMIAN>200){    //��ѭ�ȴ��ػ�ʱ��
			p = -1;
		    XIUMIAN=0;
		}
	   else{ XIUMIAN++; }
	   
	   	if(taiwan)//�淭��Ϩ��
		{
			mpu_get_gyro_reg(gyro,&timestamp);
			gyroX = gyro[0];
			if( gyroX < -4500 )
			{
				p = -1;
				XIUMIAN=0;
			}
		}
		
	   if( p == 1)
	   { 
		   XIUMIAN=0;
		   GUODUsn=1;
		   OLED_transition(0);
		   

		   main_menu2(); //����˵�2
		   
		   
		   GUODUsn=1;
       }	 
       else if( p == -1)
	   { 
		   XIUMIAN=0;
		   GUODUsn=1;
		   OLED_transition(0);
		   
		   if(taiwan)
		   {
				TAIWANLIANGP();
				
				GUODUsn=1;
		   }
		   else if(dgh)
		   {
			   
				GUODUsn=1;
				OLED_transition(0);
				
					/*����ֹͣ*/
					GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);		
				
					OLED_Clear();				//����Դ�����
					OLED_Update();				//������ʾ������
					
					OLED_WriteCommand(0xAE);	//0xAF������ʾ��0xAEΪ����
					
					OLED_WriteCommand(0xAD);	//���ó���
					OLED_WriteCommand(0x8A);    //0x8BΪ������0x8AΪ�رճ���
					
					/*ʹ��WKUP����*/
					PWR_WakeUpPinCmd(ENABLE);						//ʹ��λ��PA0��WKUP���ţ�WKUP���������ػ��Ѵ���ģʽ
					PWR_EnterSTANDBYMode();						//STM32����ֹͣģʽ�����ȴ�ָ���Ļ����¼���WKUP�����ػ�RTC���ӣ�
		   }	
	       else{
			   
			   Power_OFF();//�ػ�
		   }
		   
		   GUODUsn=1;	   			 		   
	   }	   
		   while(GUODUsn)			//��һ����Ϊ�˷�ֹ������������ȥ�жϰ�������ֹ�󴥷�
		   { 
			   biaopan();
				delay_ms(4);		   
			   OLED_transition(1);	   
		   }
	   
	   
	   
	   p = menu_Enter_event();	 
	   if(p)
	   { 
		   XIUMIAN=0;
		   GUODUsn=1;
		   OLED_transition(0);
		   
		   main_menu1(); //����˵�1
		   
		   GUODUsn=1;
	   }
   }


}



void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)		//�ⲿ�ж�0���ߴ������ж�
	{
		
		EXTI_ClearITPendingBit(EXTI_Line0);		//����жϱ�־λ
																								
	}
}




void STM32_rouse(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//����AFIO��ʱ�ӣ��ⲿ�жϱ��뿪��AFIO��ʱ��
		
	/*AFIOѡ���ж�����*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//���ⲿ�жϵ�0����ӳ�䵽GPIOA����ѡ��PA0Ϊ�ⲿ�ж�����
	
	/*EXTI��ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;						//����ṹ�����
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;					//ѡ�������ⲿ�жϵ�0����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//ָ���ⲿ�ж���ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ָ���ⲿ�ж���Ϊ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//ָ���ⲿ�ж���Ϊ�����ش���
	EXTI_Init(&EXTI_InitStructure);								//���ṹ���������EXTI_Init������EXTI����
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		    //ѡ������NVIC��0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//ָ��NVIC��·����ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStructure);		
}


