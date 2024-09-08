#include "MP3_tf.h"
#include "biaopan.h"
#include "stm32f10x.h"               
#include "menu.h"
#include "Delay.h"  
#include "Serial2.h"

uint8_t mp3repetition1 = 0;

uint8_t mp3play1 = 0;	//���ű�־λ
uint8_t mp3while = 0;	//ѭ�����ű�־λ
uint8_t mp3PLSD = 0;	//���꼴ͣ��־λ

uint8_t mp3volume= 10;   //MP3����

uint8_t mp3chapter= 0 ;   //MP3����λ

uint8_t mp3chapterMAX= 28;   //MP3����λ�����

uint8_t mp3data= 0 ;   //MP3�跢������λ



void MP3_starts(uint8_t e) //MP3�Ƿ�ʼ����
{	
	if(e)
	{	
		MP3CMD(  0x06 , 0x00 ,  mp3volume );  //��������
		delay_ms(10);
//		MP3CMD(  0x12 , 0x00 ,  mp3chapter );
		MP3CMD(  0x0D , 0x00 ,  0x00 );     //��ʼ����
		delay_ms(200);
		mp3play1 = 1;
	}
	else
	{
		MP3CMD(  0x0E , 0x00 ,  0x00 );  //ֹͣ����
		delay_ms(200);
		mp3play1 = 0;
	}
}

void MP3_chin(uint8_t e) //MP3��Ŀ���ӣ�����
{
	if(e)
	{
		mp3chapter++;
		if(mp3chapter > mp3chapterMAX) mp3chapter=0;		
		mp3play1 = 1;
		mp3data=1;
	}
	else
	{
		mp3chapter--;
		if(mp3chapter > mp3chapterMAX) mp3chapter=mp3chapterMAX;
		mp3play1 = 1;
		mp3data=1;
	}	
}





void MP3_Init(void)
{
	//������ʹ��//
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_SET);
	
	Serial2_Init();


}

void MP3_Power_OFF(void)
{		
	GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_SET);
	Serial2_Power_OFF();
	
}


uint8_t Send_buf[8];
void MP3CMD(uint8_t CMD ,uint8_t feedback , uint16_t data)
{
    Send_buf[0] = 0x7e;    //ͷ
    Send_buf[1] = 0xff;    //�����ֽ� 
    Send_buf[2] = 0x06;    //����
    Send_buf[3] = CMD;     //����ָ��
    Send_buf[4] = feedback;//�Ƿ���Ҫ����
    Send_buf[5] = (uint8_t)(data >> 8);//data
    Send_buf[6] = (uint8_t)(data);     //data
    Send_buf[7] = 0xef;    //β
	
	for (u8 i = 0; i < 8; i ++)		//��������
	{
		Serial2_SendByte(Send_buf[i]);	  //���ε���Serial_SendByte����ÿ���ֽ�����
	}
}


extern uint8_t GUODUsn;

void MP3os(void)
{
	GUODUsn=1;
	int8_t p;
	uint16_t XIUMIAN=2;                    //��ʱ����Ļ����ʱ�����
	while(1)
	{
		OLED_Clear();	
			
		  biaopan_byd(90,0);
		  biaopan_Timer_small(0,0);	
			
		  if(mp3play1)OLED_ShowImage(40, 27, 16, 16,mp3play);
		  else OLED_ShowImage(40, 27, 16, 16,mp3stop);
			
		  OLED_ShowImage(18, 27, 16, 16,mp3right);
		  OLED_ShowImage(62, 27, 16, 16,mp3left);
		
		 OLED_ShowNum(0, 56, mp3chapter, 3, OLED_6X8);
		 OLED_ShowNum(20, 56, mp3chapterMAX, 3, OLED_6X8);

		if(mp3PLSD) OLED_ShowString(104,52,"��ͣ",OLED_6X8);
		else if(mp3while) OLED_ShowString(104,52,"ѭ��",OLED_6X8);
		else OLED_ShowString(104,52,"˳��",OLED_6X8);

		
		
	    OLED_Update();	 
		
		OLED_transition(1);
	
		
		if(XIUMIAN>400){    //��ѭ�ȴ��ػ�ʱ��
			
			GUODUsn=1;
			OLED_transition(0);
			OLED_Clear();				//����Դ�����
			OLED_Update();				//������ʾ������
			
			OLED_WriteCommand(0xAE);	//0xAF������ʾ��0xAEΪ����
			
			OLED_WriteCommand(0xAD);	//���ó���
			OLED_WriteCommand(0x8A);    //0x8BΪ������0x8AΪ�رճ���
			
			XIUMIAN=0;
	   }
	   else if(XIUMIAN==1)  
		{ 	
			OLED_WriteCommand(0xAD);	//���ó���
			OLED_WriteCommand(0x8B);    //0x8BΪ������0x8AΪ�رճ���
			
			OLED_WriteCommand(0xAF);	//0xAF������ʾ��0xAEΪ����
				
			XIUMIAN++;
			
			GUODUsn=1;
		}
		else if(XIUMIAN>1) XIUMIAN++;
				
		
		p = menu_Roll_event(); 
	   if( p == 1){ 
		   
		   if(XIUMIAN){
			   MP3_chin(1);
		       XIUMIAN=2;
		   }
		   else XIUMIAN++;
	   }	 
       else if( p == -1){
		   
		   if(XIUMIAN){
		       MP3_chin(0);
			   XIUMIAN=2;
		   }
		   else XIUMIAN++;
	   }	
	   else if(XIUMIAN>25 | XIUMIAN==0) 
	   {
			if(	mp3data)
			{
				   MP3CMD(  0x06 , 0x00 ,  mp3volume );  //��������
					delay_ms(50);
				   MP3CMD(  0x12 , 0x00 ,  mp3chapter );//��ʼ����
				   mp3data=0;
			}
	   }
	   
	   p=menu_Enter_event();
		if(p==1)
		{   
		   if(XIUMIAN)
		   {
			    XIUMIAN=2;
			    if(mp3play1)
				{
					MP3_starts(0);
				}
				else
				{
					MP3_starts(1);
				}
			}
		   else XIUMIAN++;
		}
		else if ( p==2 )
		{
			XIUMIAN++;			
			if(XIUMIAN==1)
			{			
				OLED_WriteCommand(0xAF);	//0xAF������ʾ��0xAEΪ����
				OLED_WriteCommand(0xAD);	//���ó���
				OLED_WriteCommand(0x8B);    //0x8BΪ������0x8AΪ�رճ���
			}
			else
			{
				MP3CMD(  0x19 , 0x00 ,  0x00 );
				GUODUsn=1;
				OLED_transition(0);
				return;
			}	
		}
	}
	
}



void MP3ping(void)
{
	  MP3CMD(  0x06 , 0x00 ,  mp3volume );
}

