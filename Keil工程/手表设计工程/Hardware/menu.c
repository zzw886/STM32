#include "menu.h"
#include "sys.h"               
#include "stm32f10x.h"
#include <math.h>
#include "biaopan.h"
#include "voltage_mea.h"


//�˵���ܽ�����BվUP ����Ŷ����� ��menu�˵��޸Ķ������ڴ˱�ʾ��л


uint8_t d1=15,d2=1,yiup=0,xiup,ytup=0,xtup,s1;


extern uint8_t  Up, Down, Back, Enter;

int8_t menu_Roll_event(void)//�˵�����
{	
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
	{
		if(yiup)
		{
		  if(xiup>s1)
		  {
		    s1=d2;
		    xiup=0;
		    return 1;			  
		  }
		  else
		  {
		   xiup++;
		    return 0;			  
		  }
		}
		else
		{
		 yiup=1;
		 xiup=0;
		 s1=d1;
	       return 1;			  
		}
	}
    else
	{
	 yiup=0;
	}
	
	
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==1)
	{
		if(ytup)
		{
		  if(xtup>s1)
		  {
		    s1=d2;
		    xtup=0;
		    return -1;			  
		  }
		  else
		  {
		   xtup++;
		    return 0;			  
		  }
		}
		else
		{
		 ytup=1;
		 xtup=0;
		 s1=d1;
	       return -1;			  
		}
	}
    else
	{
	 ytup=0;
	}
	return 0;
}

uint8_t yu=0,xu,upp=0,s2;

int8_t menu_Enter_event(void)//�˵�ȷ��
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1)
	{
		if(upp==0)
		{
		  if(yu)
		  {
		    if(xu>s2)
		    {
				upp=1;
				return 2;
			 }		
			else
			{
				 xu++;
				return 0;
			}	
		  }
		  else
		  {
			  yu=1;
			  xu=0;
			  s2=d1;
		    return 0;			  
		  }
		}
		else
		{
	       return 0;			  
		}
	}
    else 
	{
		if(yu)
		{
			if(upp)
			{
				yu=0;
				upp=0;
				return 0;	
			}
			else
			{
				yu=0;
				return 1;	
			}
		}
		return 0;			
	}
}
int8_t menu_Back_event(void)//�˵�����
{
	if(Back)
	{
		Back=0;
		return 1;
	}
    else return 0;	
}

int8_t Speed_Factor = 12;				//��궯���ٶ�ϵ��;
float  Roll_Speed = 1.3;				   //���������ٶ�ϵ��;
/**
  * ��    �������˵�1
  * ��    ������
  * �� �� ֵ����
  * ˵    �������ո�ʽ���ѡ��
  *
  * ��    ʽ��{ ѡ��������Ҫ���еĺ���(�޲���,�޷���ֵ)��1��32x32������ͼƬ }
  **/

void main_menu1(void)
{
	
	struct option_class1 option_list[] = {
		
		{"���ر���"	  	  , RETURN	,APPBIPAN	 	},  
		{"��ѹ����"  	  , volt   , APPADDV	}, 
		{"����" 	      , mp3up  	, APPYYBF		},	
		{"������"   	      , Detection_angle  ,APPTLY },
		{"�տ�΢��"   	  , error  , APPWXZF },
		{"�տ�֧����"   	  , error  ,APPZFBZF },
		{"��Ƶ" 	      ,	VIDEO	 	,APPVIDEO	},
		{"Life_Play"	  , Game_Of_Life_Play ,APPGAME},  
		{"WiFiģ��" 	  , error		,    bug	},		
		{"cos(x)-1" 	  , cosjiaod		,bug	},  
		{"W25Q128�ֿ����" , W25Q128Flash  , bug},
		{"����"     	  , System_settings  ,APPsetting },  
		{".."}	//��β��־,�����������
		
	};
	
	run_menu1(option_list);  //����������˵�����ˢ����ʾ
}

/**
  * ��    �������˵�2
  * ��    ������
  * �� �� ֵ����
  * ˵    �������ո�ʽ���ѡ��
  *
  * ��    ʽ��{ ѡ���� �� ѡ������ ����Ҫ���еĺ���(�޲���,�޷���ֵ)(��ѡ) , &����(��ѡ) }
  *
  *	    ѡ������:  
  *		Display    ��չʾ 		  ��״̬�±�ִ��ʱ�������κζ���������λ���������ַλ����
  *		Function   �ɽ��뺯��:    ��״̬�±�ִ��ʱ, ����һ�α�ѡ�к�����������ַλ����
  *   	ON_OFF     ����:         ��״̬�±�ִ��ʱ, ������ĵ�ַ�ı�ѡ�б���ȡ��һ�Σ��ٽ��뱻ѡ�к��� 
  *		Number     ����:		��״̬�±�ִ��ʱ, �������ڲ˵��ڲ��� ������������ÿ����һ�ξͽ��뱻ѡ�к���һ��
  *     //ע�⣬��Ҫ���ع���ʱ����ʹ��Function����Ҫ���еĺ�����RETURN����������
  *     //������Ϊ���ػ����ʱ��RETURN�䵱�պ�������
  */
void main_menu2(void)
{
	struct option_class2 option_list[] = {
		
		{"- ����"        ,Function  , RETURN  			},
		{"- �ֵ�Ͳ"	     ,Function  , shoudt  			},
		{"- ���"	     ,Function  , error   			},                 
		{"- ����"	     ,Function  , error 			},
		{"- ��ʱ��"      ,Function  , error  			},
		{"- ʱ�����"    ,Function  , biaopan_Timer_data},
		{"- WiFi��ʱ"    ,Function  , error  			},		
		{".."}	//��β��־,�����������
		
	};
	 
	run_menu2(option_list);
}

uint8_t Cursor=0;
extern uint8_t GUODUsn;

/**
  * �� �����˵�����
  * �� ����ѡ���б�
  * ˵ ������ѡ���б���ʾ����,�����ݰ����¼�ִ����Ӧ����
  */
void run_menu2(struct option_class2* option)
{
	
	int8_t  Catch_i = 1;		   //ѡ���±�
	int8_t  Cursor_i = 0;	      //����±�
	int8_t  Show_i = 0; 		 //��ʾ��ʼ�±�
	int8_t  Max = 0;			//ѡ������
	int8_t  Roll_Event = 0;	   //�����¼�
	uint8_t Prdl ;            //���������񳤶� 
	float  mubiao = 0 , mubiao_up = 0;   //Ŀ����������ȣ��ϴν���������

	
	
	while(option[++Max].Name[0] != '.');    	       //��ȡ��Ŀ����,����ļ�����ͷΪ'.'��Ϊ��β;
	Max--;											  //����ӡ".."
	
	for(int8_t i = 0; i <= Max; i++)				//����ѡ�������;
	{		
		option[i].NameLen = Get_NameLen(option[i].Name);
	}
	
	static float Cursor_len_d0 = 0, Cursor_len_d1 = 0, Cursor_i_d0 = 0, Cursor_i_d1 = 0;//���λ�úͳ��ȵ�����յ�
	
	int8_t Show_d = 0, Show_i_temp = Max;				        //��ʾ�������
		
	Prdl = 64/Max;	  // ��Ļ�߶� / ��Ŀ����
	
	GUODUsn=1;
	
	while(1)
	{
		OLED_Clear();
		
		Roll_Event = menu_Roll_event();				      //��ȡ�����¼�
		if(Roll_Event)							         //����а����¼�
		{
			Cursor_i += Roll_Event;					   //�����±�
			Catch_i += Roll_Event;
			
			if(Catch_i < 0) {Catch_i = 0;}			//����ѡ���±�
			if(Catch_i > Max) {Catch_i = Max;}
			
			if(Cursor_i < 0) {Cursor_i = 0;}     //���ƹ��λ��
			if(Cursor_i > 3) {Cursor_i = 3;}
			if(Cursor_i > Max) {Cursor_i = Max;}
		}
		
	/**********************************************************/
	/*��ʾ���*/
		
		Show_i = Catch_i - Cursor_i;			    //������ʾ��ʼ�±�
  			                                       
		
		if(Show_i - Show_i_temp)			     //����±���ƫ��
		{
			Show_d = (Show_i - Show_i_temp) * MEHE;
			Show_i_temp = Show_i;
		}
		if(Show_d) {Show_d /= Roll_Speed;}	//�����仯�ٶ�
		
		for(int8_t i = 0; i < 5; i++)	  //������ʾѡ�����Ͷ�Ӧ����
		{	
			if(Show_i + i > Max ) {break;}			
			OLED_ShowString(2, (i* MEHE)+Show_d +2, option[Show_i + i].Name, OLED_6X8);
			
			
			if( option[Show_i + i].mode == ON_OFF )  				//����ǿ�����
			{
				 OLED_DrawRectangle(96, (i* MEHE)+Show_d +2, 12, 12, OLED_UNFILLED);
				if( *( option[Show_i + i].Num) )    				//������ش�(����ֵΪ��)
				{
					OLED_DrawRectangle(99, (i* MEHE)+Show_d +5, 6, 6, OLED_FILLED);
				}
			}
			else if( option[Show_i + i].mode == 3 )   				//����Ǳ�����
			{
				//�����λ��Ϊ��
				if ( *( option[Show_i + i].Num) / OLED_Pow(10, 3 - 0 - 1) % 10 )              
				OLED_ShowNum(92, (i* MEHE)+Show_d +6, *( option[Show_i + i].Num), 3, OLED_6X8);
				//���ʮλ��Ϊ��
				else if ( *( option[Show_i + i].Num) / OLED_Pow(10, 3 - 1 - 1) % 10 )
				OLED_ShowNum(95, (i* MEHE)+Show_d +6, *( option[Show_i + i].Num), 2, OLED_6X8);
				//�����λ��Ϊ��
				else if ( *( option[Show_i + i].Num) / OLED_Pow(10, 3 - 2 - 1) % 10 )
				OLED_ShowNum(98, (i* MEHE)+Show_d +6, *( option[Show_i + i].Num), 1, OLED_6X8);
				
				else   
				OLED_ShowNum(98, (i* MEHE)+Show_d +6, 0, 1, OLED_6X8);
			}
		}
		
		
		
		
		
	/**********************************************************/
	/*������*/
		
	
		Cursor_i_d1 = Cursor_i * MEHE;						//��ѯ���Ŀ��λ��
		Cursor_len_d1 = option[Catch_i].NameLen * WORD_H + 4;	   //��ѯ���Ŀ����
		
		/*����˴�ѭ�����λ��*///�����ǰλ�ò���Ŀ��λ��,��ǰλ����Ŀ��λ���ƶ�
		if((Cursor_i_d1 - Cursor_i_d0) > 1) {Cursor_i_d0 += (Cursor_i_d1 - Cursor_i_d0) / Speed_Factor + 1;}		
		else if((Cursor_i_d1 - Cursor_i_d0) < -1) {Cursor_i_d0 += (Cursor_i_d1 - Cursor_i_d0) / Speed_Factor - 1;}
		else {Cursor_i_d0 = Cursor_i_d1;}
		
		/*����˴�ѭ�������*/
		if((Cursor_len_d1 - Cursor_len_d0) > 1) {Cursor_len_d0 += (Cursor_len_d1 - Cursor_len_d0) / Speed_Factor + 1;}
		else if((Cursor_len_d1 - Cursor_len_d0) < -1) {Cursor_len_d0 += (Cursor_len_d1 - Cursor_len_d0) / Speed_Factor - 1;}
		else {Cursor_len_d0 = Cursor_len_d1;}
	
	
	  //��ʾ���//
		if(Cursor==0)	OLED_ReverseArea2(0, Cursor_i_d0, Cursor_len_d0, 16);           //��ָ��λ��ȡ��

		else if(Cursor==1) OLED_DrawRectangle(0, Cursor_i_d0, Cursor_len_d0, 16, OLED_UNFILLED);
		
		else if(Cursor==2) OLED_ShowString(Cursor_len_d0, Cursor_i_d0+2, "<-", OLED_6X8);	//β�͹��
		
							
	/**********************************************************/
		//������//
			
		if(Catch_i== Max) {mubiao = 64 ;}	
		else{ mubiao = Prdl * Catch_i ;}
		
		if(mubiao_up<mubiao) { mubiao_up += 1.4 ;}
		if(mubiao_up>mubiao) { mubiao_up -= 1.4 ;}
		
		OLED_DrawLine(123, 0, 127, 0);
		OLED_DrawLine(125, 0, 125, 63);  
		OLED_DrawLine(123, 63, 127, 63);  

		OLED_DrawRectangle(123, 0, 5, mubiao_up, OLED_FILLED);
		
	/**********************************************************/
	   //ˢ����Ļ//	
		
		OLED_Update();
		
		OLED_transition(1);
		
	/**********************************************************/
	   //��ȡ����//
		if(menu_Enter_event())			                  	
		{
			if( option[Catch_i].mode == Function )          //����ǿɽ��뺯��
			{
				/*������ܲ�Ϊ����ִ�й���,���򷵻�*/
				if(option[Catch_i].func) 
				{
					
					GUODUsn=1;
					OLED_transition(0);
					
					option[Catch_i].func();
					
					GUODUsn=1;

				}
				else 
				{
				
					GUODUsn=1;
					OLED_transition(0);
					
					return;
				}
			}
			else if( option[Catch_i].mode == ON_OFF )                    //����ǿ���
			{
				
				*( option[Catch_i].Num) =  !*( option[Catch_i].Num);  //��ǰ����ȡ��
				
				if( option[Catch_i].func){option[Catch_i].func();} //ִ�й���һ��
				
			}
			else if( option[Catch_i].mode == Number )            //����Ǳ���
			{
				
				GUODUsn=1;
				OLED_transition(0);
				
				//�ڲ��ı���������//
				int8_t p;
				float NP=0 , NP_target = 96;               //��������С��Ŀ�����С
				float GP=0 , GP_target ;                 //�����������ȣ�Ŀ����������
//				float tap = 80/255;	     				 //��������	0.31
				
				
				GUODUsn=1;	
				
				while(1) 								
				{
					if( NP < NP_target )              //����������û����
					{
						if( NP_target - NP > 25 ) NP += 5 ;
						else if( NP_target - NP > 5 ) NP += 2.5 ;
						else if( NP_target - NP > 0 ) NP += 0.5 ;
						
						OLED_DrawRectangle(16, 8, NP, NP/2, OLED_UNFILLED);
						OLED_ClearArea(17, 9, NP-2, NP/2-2);
						
					   OLED_Update();												
					}
					else
					{
//						 GP_target = *( option[Catch_i].Num) * tap ;						
						 GP_target = *( option[Catch_i].Num) * 0.31 ;
						if(GP < GP_target)
						{ 
							if( GP_target - GP > 25 ) GP += 5 ;
							else if( GP_target - GP > 7 ) GP += 2.4 ;
							else if( GP_target - GP > 0 ) GP += 0.5 ;
						}
						if(GP > GP_target)
						{ 	
							if( GP - GP_target > 25 ) GP -= 5 ;
							else if( GP - GP_target > 7 ) GP -= 2.4 ;
							else if( GP - GP_target > 0 ) GP -= 0.5 ;
						}
						
						OLED_ClearArea(24, 43, 80, 3);
						OLED_DrawRectangle(24, 43, GP , 3, OLED_FILLED);												
						
						OLED_ClearArea(55, 20, 18, 8);
						//�����λ��Ϊ��
						if ( *( option[Catch_i].Num) / OLED_Pow(10, 3 - 0 - 1) % 10 )              
						OLED_ShowNum(55, 20, *( option[Catch_i].Num), 3, OLED_6X8);
						//���ʮλ��Ϊ��
						else if ( *( option[Catch_i].Num) / OLED_Pow(10, 3 - 1 - 1) % 10 )
						OLED_ShowNum(58, 20, *( option[Catch_i].Num), 2, OLED_6X8);
						//�����λ��Ϊ��
						else if ( *( option[Catch_i].Num) / OLED_Pow(10, 3 - 2 - 1) % 10 )
						OLED_ShowNum(61, 20, *( option[Catch_i].Num), 1, OLED_6X8);						
						else   
						OLED_ShowNum(61, 20, 0, 1, OLED_6X8);
						
					   OLED_Update();	
						
					}
					
						
					   OLED_transition(1);
					
					   p = menu_Roll_event();   
					   if( p == 1)
					   { 
							*( option[Catch_i].Num) += 1;
							if( option[Catch_i].func){option[Catch_i].func();} //ִ�й���һ��
							
					   }	 
					   else if( p == -1)
					   { 
							*( option[Catch_i].Num) -= 1;
							if( option[Catch_i].func){option[Catch_i].func();} //ִ�й���һ��
					   }
					   
					   p=menu_Enter_event();
					   if(p==1){
						   
						   GUODUsn=1;
						   OLED_transition(0);
						   OLED_Clear();
						   OLED_Update();	

						   GUODUsn=1;
						   break;											
					   }
					   else if ( p==2 ){
					
						   GUODUsn=1;
						   OLED_transition(0);
						   OLED_Clear();
						   OLED_Update();	

						   GUODUsn=1;
						   break;					
					}					   
				}
											
			}
			
			//����Ȳ��ǣ����������غͺ�����һ�ʵ���չʾ����
			
			
		}
		if(menu_Back_event())
		{
			
			GUODUsn=1;
			OLED_transition(0);
			
			return;
		
		}	             
	}
}

	/**********************************************************/




void run_menu1(struct option_class1* option)
{
	int8_t  Catch_i = 1;		   //ѡ���±�
	int8_t  Show_i = 0; 		 //��ʾ��ʼ�±�
	int8_t  Max = 0;			//ѡ������
	int8_t  Roll_Event = 0;	   //�����¼�
	
	
	while(option[++Max].Name[0] != '.');    	       //��ȡ��Ŀ����,����ļ�����ͷΪ'.'��Ϊ��β;
	Max--;											  //����ӡ".."
	
	for(int8_t i = 0; i <= Max; i++)				//����ѡ�������;
	{		
		option[i].NameLen = Get_NameLen(option[i].Name);
	}
		
	int8_t Show_d = 0, Show_i_temp = Max , yi=40;				        //��ʾ�������
			
	GUODUsn=1;
	
	float yp=0,ypt=0 ;
	
	while(1)
	{
		OLED_Clear();
		
		Roll_Event = menu_Roll_event();				      //��ȡ�����¼�
		if(Roll_Event)							         //����а����¼�
		{
			Catch_i += Roll_Event;
			
			if(Catch_i < 0) {Catch_i = 0;}			//����ѡ���±�
			if(Catch_i > Max) {Catch_i = Max;}
			
		}
		
	/**********************************************************/
	/*��ʾ���*/
		
		Show_i = Catch_i ;			    //������ʾ��ʼ�±�
  			                                       
		
		if(Show_i - Show_i_temp)			     //����±���ƫ��
		{
			Show_d = (Show_i - Show_i_temp) * 40;
			Show_i_temp = Show_i;
			
			yp = option[Catch_i].NameLen * WORD_H ;
		}
		if(Show_d) {Show_d /= 1.15;}	//�����仯�ٶ�
		if(yi) {yi /= 1.3;}	//�����仯�ٶ�

		
		if(ypt < yp)
		{ 
			if( yp - ypt > 10 ) ypt += 5 ;
			else if( yp - ypt > 5 ) ypt += 2.5 ;
			else if( yp - ypt > 0 ) ypt += 0.5 ;
		}
		if(ypt > yp)
		{ 	
			if( ypt - yp > 10 ) ypt -= 5 ;
			else if( ypt - yp > 5 ) ypt -= 2.5 ;
			else if( ypt - yp > 0 ) ypt -= 0.5 ;
		}

		
		for(int8_t i = -2; i < 3; i++)	  //������ʾѡ�����Ͷ�Ӧ����
		{	
			int8_t temp = i * 40 + Show_d + 48;
			
			if(Show_i + i < 0) {continue;}
			if(Show_i + i > Max ) {break;}			
			
			OLED_ShowImage(temp , 8 - yi , 32 , 32 , option[Show_i + i].Image);
			
			uint8_t tp = abs(Show_d/3);
			
			OLED_ShowString( 64 - yp/2, 50+tp, option[Catch_i].Name, OLED_6X8);
			
			OLED_DrawRectangle( 64 - (ypt/2) -2 ,63, ypt+4 ,1 ,OLED_UNFILLED);
			
		}
								
		
	/**********************************************************/
	   //ˢ����Ļ//	
		
		OLED_Update();
		
		OLED_transition(1);
		
	/**********************************************************/
	   //��ȡ����//
		if(menu_Enter_event())			                  	
		{
			/*������ܲ�Ϊ����ִ�й���,���򷵻�*/
			if(option[Catch_i].func) 
			{
				
				GUODUsn=1;
				OLED_transition(0);
				
				option[Catch_i].func();
				
				GUODUsn=1;
				yi=40;
				Show_d=-40;

			}
			else 
			{
			
				GUODUsn=1;
				OLED_transition(0);
				
				return;
			}
			
		}
		if(menu_Back_event())
		{
			
			GUODUsn=1;
			OLED_transition(0);
			
			return;
		
		}	             
	}

}



    //����ѡ�������;//
uint8_t Get_NameLen(char* String)
{
	uint8_t i = 0, len = 0;
	while(String[i] != '\0')			        //�����ַ�����ÿ���ַ�
	{
		if(String[i] > '~'){len += 2; i += 2;}//���������Ӣ���ַ�����ΪGB2312�����ʽ���ȼ�2��i+=2����UTF-8�����ʽ���3
		else{len += 1; i += 1;}				 //����Ӣ���ַ����ȼ�1
	}
	return len;
}


void OLED_ReverseArea2(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	OLED_ReverseArea( X+1, Y, Width-2, 1);
	OLED_ReverseArea( X, Y+1, Width, Height-2);
	OLED_ReverseArea( X+1, Y+Height-1, Width-2, 1);
}
