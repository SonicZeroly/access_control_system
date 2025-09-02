#ifndef __LCD_H__
#define __LCD_H__

#include "main.h"

/*************���ŷ���*****************/
#define LCD_CS_PORT		GPIOD
#define LCD_CS_PIN		GPIO_PIN_7

#define LCD_RS_PORT		GPIOD
#define LCD_RS_PIN		GPIO_PIN_13

#define LCD_WR_PORT		GPIOD
#define LCD_WR_PIN		GPIO_PIN_5

#define LCD_RD_PORT		GPIOD
#define LCD_RD_PIN		GPIO_PIN_4

#define LCD_RST_PORT	GPIOD
#define LCD_RST_PIN		GPIO_PIN_6

#define LCD_BL_PORT		GPIOD
#define LCD_BL_PIN		GPIO_PIN_8

#define LCD_DB_PORT		GPIOE
#define LCD_DB0_PORT	GPIOE
#define LCD_DB0_PIN		GPIO_PIN_0
#define LCD_DB1_PORT	GPIOE
#define LCD_DB1_PIN		GPIO_PIN_1
#define LCD_DB2_PORT	GPIOE
#define LCD_DB2_PIN		GPIO_PIN_2
#define LCD_DB3_PORT	GPIOE
#define LCD_DB3_PIN		GPIO_PIN_3
#define LCD_DB4_PORT	GPIOE
#define LCD_DB4_PIN		GPIO_PIN_4
#define LCD_DB5_PORT	GPIOE
#define LCD_DB5_PIN		GPIO_PIN_5
#define LCD_DB6_PORT	GPIOE
#define LCD_DB6_PIN		GPIO_PIN_6
#define LCD_DB7_PORT	GPIOE
#define LCD_DB7_PIN		GPIO_PIN_7
#define LCD_DB8_PORT	GPIOE
#define LCD_DB8_PIN		GPIO_PIN_8
#define LCD_DB9_PORT	GPIOE
#define LCD_DB9_PIN		GPIO_PIN_9
#define LCD_DB10_PORT	GPIOE
#define LCD_DB10_PIN	GPIO_PIN_10
#define LCD_DB11_PORT	GPIOE
#define LCD_DB11_PIN	GPIO_PIN_11
#define LCD_DB12_PORT	GPIOE
#define LCD_DB12_PIN	GPIO_PIN_12
#define LCD_DB13_PORT	GPIOE
#define LCD_DB13_PIN	GPIO_PIN_13
#define LCD_DB14_PORT	GPIOE
#define LCD_DB14_PIN	GPIO_PIN_14
#define LCD_DB15_PORT	GPIOE
#define LCD_DB15_PIN	GPIO_PIN_15

//PE0~15,��Ϊ������
#define DATAOUT(x) 	LCD_DB_PORT->ODR=x; //�������
#define DATAIN     	LCD_DB_PORT->IDR;   //��������

//����LCD�ĳߴ�
#define LCD_W 240
#define LCD_H 320

//������ɫ
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //��ɫ
#define BRRED 			0XFC07 //�غ�ɫ
#define GRAY  			0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	0X841F //ǳ��ɫ
#define LIGHTGRAY     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 		0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#define LCD_LED_ON		HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_SET);
#define LCD_LED_OFF		HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_RESET);

//GPIO��λ�����ߣ���BSRR�Ĵ�����16λ�������Ÿߵ�ƽ
#define	LCD_CS_SET 	LCD_CS_PORT->BSRR = LCD_CS_PIN
#define LCD_RS_SET	LCD_RS_PORT->BSRR = LCD_RS_PIN
#define LCD_RST_SET	LCD_RST_PORT->BSRR = LCD_RST_PIN
#define LCD_WR_SET	LCD_WR_PORT->BSRR = LCD_WR_PIN
#define LCD_RD_SET	LCD_RD_PORT->BSRR = LCD_RD_PIN

//GPIO��λ�����ͣ���BSRR�Ĵ�����16λ�������ŵ͵�ƽ
#define	LCD_CS_RESET 	LCD_CS_PORT->BSRR = (LCD_CS_PIN << 16)
#define LCD_RS_RESET	LCD_RS_PORT->BSRR = (LCD_RS_PIN << 16)
#define LCD_RST_RESET	LCD_RST_PORT->BSRR = (LCD_RST_PIN << 16)
#define LCD_WR_RESET	LCD_WR_PORT->BSRR = (LCD_WR_PIN << 16)
#define LCD_RD_RESET	LCD_RD_PORT->BSRR = (LCD_RD_PIN << 16)

//�û�����
#define USE_HORIZONTAL  	0 //����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת
#define USE_VERTICAL  	    1 
#define LCD_USE8BIT_MODEL   0	//�������������Ƿ�ʹ��8λģʽ 0,ʹ��16λģʽ.1,ʹ��8λģʽ

//LCD��Ҫ������
typedef struct  
{										    
	uint16_t 	width;			//LCD ���
	uint16_t 	height;			//LCD �߶�
	uint16_t 	id;				  //LCD ID
	uint8_t  	dir;			  //���������������ƣ�0��������1��������	
	uint16_t	wramcmd;	//��ʼдgramָ��
	uint16_t  	rramcmd;   //��ʼ��gramָ��
	uint16_t  	setxcmd;		//����x����ָ��
	uint16_t  	setycmd;		//����y����ָ��	 
}_lcd_dev; 	

extern _lcd_dev lcddev;
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ  
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
	    															  
void LCD_Init(void);
void LCD_write(uint16_t VAL);
uint16_t LCD_read(void);
void LCD_Clear(uint16_t Color);	 
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x,uint16_t y);//����
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); //����	   
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
uint16_t LCD_RD_DATA(void);//��ȡLCD����								    
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WR_REG(uint16_t data);
void LCD_WR_DATA(uint16_t data);
void LCD_ReadReg(uint16_t LCD_Reg,uint8_t *Rval,int n);
void LCD_WriteRAM_Prepare(void);
void LCD_ReadRAM_Prepare(void);   
void Lcd_WriteData_16Bit(uint16_t Data);
uint16_t Lcd_ReadData_16Bit(void);
void LCD_direction(uint8_t direction );
uint16_t Color_To_565(uint8_t r, uint8_t g, uint8_t b);
uint16_t LCD_Read_ID(void);

void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
void LCD_ColorFill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t* color_p);
void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend);
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);
void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode);
void LCD_ShowString(uint16_t x,uint16_t y,uint8_t size, uint8_t *p,uint8_t mode);
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);


#endif 

