#include "stm32f10x.h"                  // Device header

/**
  * ��    ����AD��ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void AD_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//����ADC1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//����GPIOB��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
	
	/*����ADCʱ��*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);						//ѡ��ʱ��6��Ƶ
	
	/*GPIO��ʼ��*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��PB0��PB1���ų�ʼ��Ϊģ������
	
	/*���ڴ˴����ù��������У�������ÿ��ADת��ǰ���ã���������������ADת����ͨ��*/
	
	/*ADC��ʼ��*/
	ADC_InitTypeDef ADC_InitStructure;						//����ṹ�����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//ģʽ��ѡ�����ģʽ��������ʹ��ADC1
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//���ݶ��룬ѡ���Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ������ʹ���������������Ҫ�ⲿ����
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//����ת����ʧ�ܣ�ÿת��һ�ι��������к�ֹͣ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//ɨ��ģʽ��ʧ�ܣ�ֻת�������������1��һ��λ��
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//ͨ������Ϊ1������ɨ��ģʽ�£�����Ҫָ������1�������ڷ�ɨ��ģʽ�£�ֻ����1
	ADC_Init(ADC1, &ADC_InitStructure);						//���ṹ���������ADC_Init������ADC1
	
	/*ADCʹ��*/
	ADC_Cmd(ADC1, ENABLE);									//ʹ��ADC1��ADC��ʼ����
	
	/*ADCУ׼*/
	ADC_ResetCalibration(ADC1);								//�̶����̣��ڲ��е�·���Զ�ִ��У׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
  * ��    ������ȡADת����ֵ
  * ��    ����ADC_Channel ָ��ADת����ͨ������Χ��ADC_Channel_x������x������0/1
  * �� �� ֵ��ADת����ֵ����Χ��0~4095
  */
uint16_t AD_GetValue(uint8_t ADC_Channel)
{  
//	if(ADC_Channel==ADC_Channel_0)
//	{
		ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_239Cycles5);	//��ÿ��ת��ǰ�����ݺ����β������Ĺ������ͨ��1
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//�������ADת��һ��
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//�ȴ�EOC��־λ�����ȴ�ADת������
		return ADC_GetConversionValue(ADC1);					//�����ݼĴ������õ�ADת���Ľ��
//	}
//	return 0;
}



