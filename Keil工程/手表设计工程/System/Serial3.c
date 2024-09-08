#include "stm32f10x.h"              
#include <stdio.h>
#include <stdarg.h>

#include "string.h"             //�ַ����жϺ�����


char Serial3_RxPacket[100];	//����������ݰ�����
uint8_t Serial3_RxFlag;	   //����������ݰ���־λ

/**
  * ��    �������ڳ�ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void Serial3_Init(void)
{
	
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//����USART3��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOB��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);				
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = 921600;		   //������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;           	  //ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		     //��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	    //ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ���ѡ��8λ
	USART_Init(USART3, &USART_InitStructure);				  //���ṹ���������USART_Init������USART3
	
	/*�ж��������*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		   //�������ڽ������ݵ��ж�

	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					       //����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		      //ѡ������NVIC��USART3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		    	 //ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		   //ָ��NVIC��·����Ӧ���ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStructure);							  //���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART3, ENABLE);							   //ʹ��USART3�����ڿ�ʼ����
}


void Serial3_Power_OFF(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	USART_Cmd(USART3, DISABLE);
	
}


/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void Serial3_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void Serial3_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Serial3_SendByte(Array[i]);	  //���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void Serial3_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Serial3_SendByte(String[i]);	  //���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t Serial3_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	  //���ý����ֵΪ1
	while (Y --)			 //ִ��Y��
	{
		Result *= X;	   //��X�۳˵����
	}
	return Result;
}


extern uint8_t OLED_DisplayBuf[8][128];

uint8_t p0 = 0, p1 = 0 ,we = 0;
/**
  * ��    ����USART3�жϺ���
  * ��    ������
  * �� �� ֵ����
  */
void USART3_IRQHandler(void)
{
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		/*����һ���ֽ�����*/
		uint8_t RxData = USART_ReceiveData(USART3);
		
		/*��һ���ֽڷŵ�OLED�Դ������ָ��λ��*/
		OLED_DisplayBuf[p0][p1] = RxData;
		
		/*λ������*/
		p1 ++;
		if (p1 >= 128)
		{
			p1 = 0;
			p0 ++;
			if (p0 >= 8)
			{
				p0 = 0;
				we = 1;
			}
		}
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
