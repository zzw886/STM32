#include "stm32f10x.h"              
#include <stdio.h>
#include <stdarg.h>

#include "string.h"             //�ַ����жϺ�����
#include "OLED.h"    


char Serial2_RxPacket[100];	//����������ݰ����飬���ݰ���ʽ"@XX"
uint8_t Serial2_RxFlag;	   //����������ݰ���־λ

/**
  * ��    �������ڳ�ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void Serial2_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//����USART2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = 9600;		   //������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;           	  //ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		     //��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	    //ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ���ѡ��8λ
	USART_Init(USART2, &USART_InitStructure);				  //���ṹ���������USART_Init������USART2
	
	/*�ж��������*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		   //�������ڽ������ݵ��ж�
	

	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					       //����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		      //ѡ������NVIC��USART2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		    	 //ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		   //ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							  //���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART2, ENABLE);							   //ʹ��USART2�����ڿ�ʼ����
}

void Serial2_Power_OFF(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	USART_Cmd(USART2, DISABLE);
}


/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void Serial2_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void Serial2_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Serial2_SendByte(Array[i]);	  //���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void Serial2_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Serial2_SendByte(String[i]);	  //���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t Serial2_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	  //���ý����ֵΪ1
	while (Y --)			 //ִ��Y��
	{
		Result *= X;	   //��X�۳˵����
	}
	return Result;
}



#include "MP3_tf.h"
/**
  * ��    ����USART2�жϺ���
  * ��    ������
  * �� �� ֵ����
  */
void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;		 //�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����	

	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)	 //�ж��Ƿ���USART1�Ľ����¼��������ж�
	{
		uint8_t RxData = USART_ReceiveData(USART2);		   //��ȡ���ݼĴ���������ڽ��յ����ݱ���		
		/*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
		
		/*��ǰ״̬Ϊ0���������ݰ���ͷ�������֤*/
		if (RxState == 0)
		{
			if (RxData == 0xFF && Serial2_RxFlag == 0) //�������ȷʵ�ǰ�ͷ��������һ�����ݰ��Ѵ������
			{
				RxState = 1;			//����һ��״̬
				pRxPacket = 0;		   //���ݰ���λ�ù���
			}

		}
		/*��ǰ״̬Ϊ1���������ݰ���һ������*/
		else if (RxState == 1)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 2;		              //����һ��״̬

		}
				else if (RxState == 2)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 3;		              //����һ��״̬

		}

				else if (RxState == 3)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 4;		              //����һ��״̬

		}

				else if (RxState == 4)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 5;		              //����һ��״̬

		}
				else if (RxState == 5)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 6;		              //����һ��״̬

		}

				else if (RxState == 6)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 7;		              //����һ��״̬

		}

				else if (RxState == 7)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 8;		              //����һ��״̬

		}

				else if (RxState == 8)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 9;		              //����һ��״̬

		}

				else if (RxState == 9)
		{
				Serial2_RxPacket[pRxPacket] = RxData;	  //�����ݴ������ݰ������ָ��λ��
			    pRxPacket ++;			                 //���ݰ���λ������
				Serial2_RxPacket[pRxPacket] = '\0';		//���յ����ַ����ݰ����һ���ַ���������־
				Serial2_RxFlag = 1;		               //�������ݰ���־λ��1���ɹ�����һ�����ݰ�
				RxState = 0;			              //״̬��0
		}	

		
			
		if (Serial2_RxFlag == 1)		          //������յ����ݰ�
		{
				if (Serial2_RxPacket[1]== 0x3D)	
				{
					if(mp3PLSD)
					{
						mp3play1=0;
				    }
					else
					{
						if(mp3while)	//ѭ�����ű�־λ
						{	
//							MP3CMD(  0x0D , 0x00 ,  0x00 )
							mp3data=1;
						}
						else 
						{	
							MP3_chin(1);    //mp3chapter++;
//							if(mp3chapter > mp3chapterMAX) mp3chapter=0;
//							mp3data=1;
//							MP3CMD(  0x12 , 0x00 ,  mp3chapter );//��ʼ����
						}
					}
				}
			Serial2_RxFlag = 0;			//������ɣ��������ݰ���־λ����
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);		//�����־λ
	}
	

}

