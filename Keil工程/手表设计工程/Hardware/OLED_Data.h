#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*�����ַ��ֽڿ��*/
#define OLED_CHN_CHAR_WIDTH			2		//UTF-8�����ʽ��3��GB2312�����ʽ��2

/*��ģ������Ԫ*/
typedef struct 
{
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//��������
	uint8_t Data[24];						//��ģ����
} ChineseCell_t;

/*ASCII��ģ��������*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*������ģ��������*/
extern const ChineseCell_t OLED_CF12x12[];

/*ͼ����������*/
extern const uint8_t Diode[];

extern const uint8_t t1[];
extern const uint8_t t2[];
extern const uint8_t t3[];
extern const uint8_t t4[];
extern const uint8_t t5[];
extern const uint8_t t6[];
extern const uint8_t t7[];
extern const uint8_t t8[];
extern const uint8_t t9[];
extern const uint8_t t0[];

extern const uint8_t i1[];
extern const uint8_t i2[];
extern const uint8_t i3[];
extern const uint8_t i4[];
extern const uint8_t i5[];
extern const uint8_t i6[];
extern const uint8_t i7[];
extern const uint8_t i8[];
extern const uint8_t i9[];
extern const uint8_t i0[];

extern const uint8_t mp3repetition[];
extern const uint8_t mp3play[];
extern const uint8_t mp3stop[];
extern const uint8_t mp3left[];
extern const uint8_t mp3right[];

extern const uint8_t bug[];

extern const uint8_t APPsetting[];
extern const uint8_t APPWXZF[];
extern const uint8_t APPZFBZF[];
extern const uint8_t APPYYBF[];
extern const uint8_t APPADDV[];
extern const uint8_t APPBIPAN[];
extern const uint8_t APPTLY[];
extern const uint8_t APPGAME[];
extern const uint8_t APPVIDEO[];


extern const uint8_t APPYYBFsmall[];
extern const uint8_t APPTLYsmall[];
extern const uint8_t APP8266small[];

/*��������ĸ�ʽ�������λ�ü����µ�ͼ����������*/
//...

#endif


/*****************��Э�Ƽ�|��Ȩ����****************/
/*****************jiangxiekeji.com*****************/
