#include "video_app.h"
#include "stm32f10x.h"               
#include "MP3_tf.h"
#include "Serial3.h"
#include "Serial2.h"
#include "Delay.h"  
#include "menu.h"
#include "W25Q128_SPI1.h"


extern uint8_t OLED_DisplayBuf[8][128];

//��Ϊ��ͬ��Ƶ�����߼�����һ����ֻ��ƫ�Ƶ�ַ���㲻һ��������д�˸�ͨ�õ�һ����
void SHOW_VIDEO(uint32_t Address ,uint16_t mpg)//��֡�� , ��ȡ������ʼ��ַ
{	
		int8_t p;		
	    p0=0,p1=0;
		
		uint8_t temp[3];   //�ļ�ͷ(3�ֽ�)
		uint32_t mydata=0; //��������(�������ļ�ͷ)
	    uint32_t datasite=3; //֡����ƫ�Ƶ�ַ

	
		OLED_Clear();
		OLED_Update(); 
		OLED_WriteCommand(0x81);	//���öԱȶ�
		OLED_WriteCommand(0xFF);	//0x00~0xFF
	
		W25Q128_ReadData( Address , temp , 3 );
	
		if(temp[0]!=0xAA) //�������ͷ�Ƿ����
		{	
			OLED_ShowString(0,20,"δ��⵽�ļ�",OLED_6X8);
			OLED_Update(); 
			
			delay_ms(500);
			GUODUsn=1;
			OLED_transition(0);
			return;
		}
		
		mydata= (temp[2]<< 8) |  temp[1];  //��ȡ��֡����
		
		OLED_ShowString(0,0,"֡��:30FPS",OLED_6X8);
		OLED_ShowString(0,13,"֡��:",OLED_6X8);
		OLED_ShowNum(38,15,mydata,4,OLED_6X8);
		
		mydata *= 1024; 				 //������������
		
		OLED_ShowString(0,26,"������(�ֽ�):",OLED_6X8);
		OLED_ShowNum(0,40,mydata,7,OLED_6X8);
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1){
			
			OLED_ShowString(0,52,"MP3�ѿ���",OLED_6X8);
			
			Serial2_Init();
			
			MP3CMD(  0x06 , 0x00 ,  mp3volume );
			delay_ms(50);
			MP3CMD(  0x0F , 0x00 ,  mpg );	
			
		}		
		else OLED_ShowString(0,52,"MP3δ����",OLED_6X8);
		
		OLED_ShowString(92,52,"��ȡ��",OLED_6X8);		//����ʵ������Ϊ���ڸ�mp3���ֵ��ӳ�
		
		OLED_Update(); 
		
			delay_ms(1800);				//���ֲ����ӳ٣�����ʵ��΢��
			delay_ms(1800);
			delay_ms(1800);
			delay_ms(1550);
		
		
		
	 uint16_t temm=15;

	while(1)
	{
		
		W25Q128_ReadData( Address+datasite , OLED_DisplayBuf[0] , 1024 );//��ȡһ֡���Դ�
		datasite+=1024;       //��ַƫ�Ƶ���һ֡
		
		delay_ms(temm);		//֡���������ʵ��΢��
		
		OLED_Update();  		
        OLED_transition(1);
		
		
		p=menu_Roll_event(); //΢��֡���
		if (p==1){			
			temm++;
		}	
		else if(p == -1){
		    temm--;
		}
		
			
		p=menu_Enter_event();  //�ֶ��˳�|ֹͣ
		if (p==1){
		
			do
			{ 
				OLED_Update();  		
				p=menu_Enter_event(); 
				
				if(p == 2){
			
					if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
					{	
						MP3_starts(0);
						Serial2_Power_OFF();
					
					}
					GUODUsn=1;
					OLED_transition(0);
					return;	
				}
				
			
			}
			while(p !=1 ); 
			
			
		}
		else if(p == 2){
			
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
			{	
				MP3_starts(0);
				Serial2_Power_OFF();
			
			}
			GUODUsn=1;
			OLED_transition(0);
			return;	
		}
		
		
		if(datasite > mydata) //�Ƿ񲥷����
		{	
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
			{	
				
				MP3_starts(0);				
				Serial2_Power_OFF();
			
			}
			OLED_Clear();
			OLED_ShowString(0,20,"�ļ��������",OLED_6X8);
			OLED_Update(); 
			delay_ms(500);
			GUODUsn=1;
			OLED_transition(0);
			return;
		}
	}

	
}

uint32_t VIDEO_SITE(uint32_t Address) //�����������Ƶ��ַ������һ����Ƶ��ַ
{
		uint8_t temp[3];   //�ļ�ͷ(3�ֽ�)
	    uint32_t mydata=0; //��������(�������ļ�ͷ)
		W25Q128_ReadData( Address , temp , 3 );

			mydata= (temp[2]<< 8) |  temp[1];  //��ȡ��֡����	
			mydata*=1024;			
			Address += (mydata+3) ;
		
	return Address;	
}


void VIDEO_Bad_apple(void)
{
		uint32_t Address=163840;//��Ƶ�ļ�������ʼ��ַ 0x28000	
	
		SHOW_VIDEO(Address,0x0101); //˳�㲥��tf����01�ļ��е�01����
}

void VIDEO_CXUK(void)
{			
		SHOW_VIDEO( VIDEO_SITE(163840) ,0x0102);
}


void VIDEO_custom(void)
{				
		SHOW_VIDEO( (VIDEO_SITE( VIDEO_SITE(163840) )+4096)  ,0x0103);
}										//��ü�1�������У���Ȼ���������


//ͬ�ϣ�д�˸�ͨ�õ�һ����
void WRITE_VIDEO(uint32_t Address)//������ʼ��ַ
{	
	
		int8_t p;
	
		Serial3_Init();
		
	    p0=0,p1=0 ,we=0;
		
		uint8_t temp[3]={0xAA,0,0};   //�ļ�ͷ(3�ֽ�)
	    uint32_t datasite=3;    //֡����ƫ�Ƶ�ַ
	
	
		OLED_WriteCommand(0x81);	//���öԱȶ�
		OLED_WriteCommand(0xFF);	//0x00~0xFF
	
		OLED_Clear();
		OLED_ShowString(0,20,"������¼��",OLED_6X8);
		OLED_Update(); 
	
		

	while(1)
	{
		if(we)//���������
		{	
			W25Q128_unlimited_Program( Address+datasite , OLED_DisplayBuf[0] , 1024 );//��¼һ֡���Դ�
			datasite+=1024;       //��ַƫ�Ƶ���һ֡
			we=0;
		}
		
		
		p=menu_Enter_event();
		if (p){
		
			if( (datasite-3) != 0 ) //���������
			{	
				temp[1]= ((datasite-3)/1024) & 0xFF;    
				temp[2]= ( ( (datasite-3) /1024 ) >> 8 ) & 0xFF;  //�Ѵ�֡��
				
				W25Q128_unlimited_Program( Address , temp , 3 );	//д���Ѵ�ͷ����
				
				OLED_Clear();
				OLED_ShowString(0,20,"�ļ���¼��",OLED_6X8);
				OLED_Update(); 
				delay_ms(500);
				GUODUsn=1;
				OLED_transition(0);
				return;	
			}
			else
			{
				OLED_Clear();
				OLED_ShowString(0,20,"δ��⵽��¼������",OLED_6X8);
				OLED_Update(); 
				delay_ms(500);
				GUODUsn=1;
				OLED_transition(0);
				return;	
			}
				
		}	
		
	}
	

}


void VIDEO_write_Bad_apple(void)
{
//		uint32_t Address=163840;//������ʼ��ַ 0x28000	
		WRITE_VIDEO(163840);	
}

void VIDEO_write_CXUK(void)
{	
		WRITE_VIDEO( VIDEO_SITE(163840) );	
}



void VIDEO_write_custom(void)
{	
		uint8_t temp[3];   //�ļ�ͷ(3�ֽ�)
		uint32_t data,pdata=0; 		
		double mydata=0; //��������(�������ļ�ͷ)
	
		data=( VIDEO_SITE( VIDEO_SITE(163840) )+4096 );
		
		
		W25Q128_ReadData( data , temp , 3 );
		
		if(temp[0] == 0xAA) //���������
		{
			mydata= (temp[2]<< 8) |  temp[1];  //��ȡ��֡����	
			mydata*=1024;				   	  //����������	

			mydata /= 4096;                 //�õ���ɾ��������
			mydata++;
			OLED_Clear();
			OLED_ShowString(0,20,"������",OLED_6X8);
			OLED_Update(); 
			
				OLED_WriteCommand(0x81);	//���öԱȶ�
				OLED_WriteCommand(0xFF);	//0x00~0xFF

			
			while( pdata < mydata )
			{	
				W25Q128_SectorErase( data+(pdata*4096) );
				pdata++;					
			}
		}
	
	    WRITE_VIDEO( data );	
}										

void VIDEO_erase(void)
{	
		int8_t p;
		GUODUsn=1;
			uint32_t data = 163840; 

		OLED_Clear();
		OLED_ShowString(0,20,"������",OLED_6X8);
		OLED_Update();
	
				
				OLED_WriteCommand(0x81);	//���öԱȶ�
				OLED_WriteCommand(0xFF);	//0x00~0xFF

		while(1)
		{
			
			
			W25Q128_SectorErase( data );
			
			OLED_ShowNum(0,40,data,8,OLED_6X8);
			
			OLED_Update();
			
			data+=4096;	
			
			
			
			p=menu_Enter_event();
			if(data>0xFFFFFF) p=1 ;
		
			if (p)
			{
					
				OLED_ShowString(0,20,"����ֹͣ",OLED_6X8);
				OLED_Update(); 
				delay_ms(1000);
				GUODUsn=1;
				OLED_transition(0);
				return;	
			}	
		}

}


void VIDEO_streaming(void)
{	
		int8_t p;
		GUODUsn=1;
	
		Serial3_Init();
		
	    p0=0,p1=0 ;
	
		OLED_Clear();

	
	while(1)
	{
		
		OLED_Update(); 
		
		OLED_transition(1);
		
		p=menu_Enter_event();
		if (p){
			
			Serial3_Power_OFF();
			GUODUsn=1;
			OLED_transition(0);
			return;	
		}	
		
	}
}

	
	

