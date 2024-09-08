#include "stm32f10x.h"              
#include <stdio.h>
#include <stdarg.h>

#include "string.h"             //�ַ����жϺ�����
#include "OLED.h"    


char Seria1_RxPacket[100];	//����������ݰ����飬���ݰ���ʽ"@XX"
uint8_t Seria1_RxFlag;	   //����������ݰ���־λ

/**
  * ��    �������ڳ�ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void Seria1_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//����USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA9���ų�ʼ��Ϊ�����������
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//��PA10���ų�ʼ��Ϊ��������
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = 9600;		   //������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;           	  //ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		     //��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	    //ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ���ѡ��8λ
	USART_Init(USART1, &USART_InitStructure);				  //���ṹ���������USART_Init������USART1
	
	/*�ж��������*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		   //�������ڽ������ݵ��ж�

	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					       //����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		      //ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		    	 //ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		   //ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							  //���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART1, ENABLE);							   //ʹ��USART1�����ڿ�ʼ����
}

/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void Seria1_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void Seria1_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Seria1_SendByte(Array[i]);	  //���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void Seria1_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Seria1_SendByte(String[i]);	  //���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t Seria1_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	  //���ý����ֵΪ1
	while (Y --)			 //ִ��Y��
	{
		Result *= X;	   //��X�۳˵����
	}
	return Result;
}





/**
  * ��    ����USART1�жϺ���
  * ��    ������
  * �� �� ֵ����
  */
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;		 //�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)	 //�ж��Ƿ���USART1�Ľ����¼��������ж�
	{
		uint8_t RxData = USART_ReceiveData(USART1);		   //��ȡ���ݼĴ���������ڽ��յ����ݱ���		
		/*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
		
		/*��ǰ״̬Ϊ0���������ݰ���ͷ�������֤*/
		if (RxState == 0)
		{
			if (RxData == 0xFF && Seria1_RxFlag == 0) //�������ȷʵ�ǰ�ͷ��������һ�����ݰ��Ѵ������
			{
				RxState = 1;			//����һ��״̬
				pRxPacket = 0;		   //���ݰ���λ�ù���
			}

		}
		/*��ǰ״̬Ϊ1���������ݰ���һ������*/
		else if (RxState == 1)
		{
				Seria1_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 2;		              //����һ��״̬

		}
				else if (RxState == 2)
		{
				Seria1_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 3;		              //����һ��״̬

		}

				else if (RxState == 3)
		{
				Seria1_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 4;		              //����һ��״̬

		}

				else if (RxState == 4)
		{
				Seria1_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 5;		              //����һ��״̬

		}

		/*��ǰ״̬Ϊ2���������ݰ��ڶ�������*/
		else if (RxState == 5)
		{
				Seria1_RxPacket[pRxPacket] = RxData;	  //�����ݴ������ݰ������ָ��λ��
			    pRxPacket ++;			                 //���ݰ���λ������
				Seria1_RxPacket[pRxPacket] = '\0';		//���յ����ַ����ݰ����һ���ַ���������־
				Seria1_RxFlag = 1;		               //�������ݰ���־λ��1���ɹ�����һ�����ݰ�
				RxState = 0;			              //״̬��0
		}	

		
			
		if (Seria1_RxFlag == 1)		          //������յ����ݰ�
		{
			if (Seria1_RxPacket[1]== 0x3F)	
			{
				if (Seria1_RxPacket[4]== 0x02)	
				{
					
				}

			}
			else if (strcmp(Seria1_RxPacket, "ls") == 0)	
			{
			}

			Seria1_RxFlag = 0;			//������ɣ��������ݰ���־λ����
		}

		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//�����־λ
	}
	

}
