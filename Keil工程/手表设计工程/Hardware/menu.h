#ifndef __MENU_H
#define __MENU_H
#include "stm32f10x.h"                  // Device header
#include <string.h>
#include "OLED.h"

#define		MEHE 		16				//�˵��� Menu height
#define		WORD_H 		6				//�ָ�word height

#define     RETURN      0               //�˵�����

#define		Display 	0				//�˵�ģʽ�궨��
#define		Function    1 
#define		ON_OFF      2
#define		Number		3

//Display    ��չʾ 		  ��״̬�±�ִ��ʱ�������κζ���������λ���������ַλ����
//Function   �ɽ��뺯��:    ��״̬�±�ִ��ʱ, ����һ�α�ѡ�к�����������ַλ����
//ON_OFF     ����:         ��״̬�±�ִ��ʱ, ������ĵ�ַ�ı�ѡ�б���ȡ��һ�Σ��ٽ��뱻ѡ�к��� 
//Number     ����:		��״̬�±�ִ��ʱ, �������ڲ˵��ڲ��� ������������ÿ����һ�ξͽ��뱻ѡ�к���һ��

/**********************************************************/


struct option_class1
{
	char* Name;				//ѡ������
	
	void (*func)(void);		//����ָ��
	
	const uint8_t *Image;   //��Ҫ�����ͼ��32*32
	
	uint8_t NameLen;		//��������ռ�����ֽ�,��strlen�������ֿ�Ȳ���׼ȷ,������ⴢ�����ֿ��
};

struct option_class2
{
	char* Name;				//ѡ������
	
	uint8_t mode;           //mode=0,ֻ��ʾ��=1�Ǻ�����=2�ǿ��أ�=3�Ǳ���
	
	void (*func)(void);		//����ָ��
	
	uint8_t *Num;   		//��Ҫ����ı�����ַ
	
	uint8_t NameLen;		//��������ռ�����ֽ�,��strlen�������ֿ�Ȳ���׼ȷ,������ⴢ�����ֿ��
};

/**********************************************************/
int8_t menu_Roll_event(void);
int8_t menu_Enter_event(void);
int8_t menu_Back_event(void);

void run_menu1(struct option_class1* option);
void run_menu2(struct option_class2* option);

uint8_t Get_NameLen(char* String);

/**********************************************************/

void main_menu1(void);
void main_menu2(void);

void Detection_angle(void);
void System_settings(void);
void particulars(void);
void Game_Of_Life_Play(void);
void Power_OFF(void);
void mp3up(void);
void volt(void);

void error(void);

void OLED_ReverseArea2(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
void shoudt(void);
void dibug(void);
void cosjiaod(void);
void skmwx(void);
void skmzfb(void);

void W25Q128Flash(void);
void VIDEO(void);

void TAIWANLIANGP(void);


/**********************************************************/

#endif
