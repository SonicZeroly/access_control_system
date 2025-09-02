/**
  ******************************************************************************
  * @file    test.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  */
#include "lcd.h"
#include "touch.h"
#include "w25qxx.h"
#include "test.h"
#include "dwt.h"

void DrawTestPage(uint8_t *str)
{
	//绘制固定栏up
	LCD_Clear(WHITE);
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	//绘制固定栏down
	LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
	POINT_COLOR=BLACK;
}

void W25QXX_Test(void){
	//	uint32_t id_ret = W25QXX_ReadID();
//	uint8_t tx_buf[10]={1, 2, 3, 7, 9, 6, 7, 8, 9, 0};
//	uint8_t rx_buf[10]={0};
//	
//	W25QXX_SectorErase(0x000000);
//	W25QXX_BufferWrite(tx_buf, 0x000000, 10);
//	W25QXX_BufferRead(rx_buf, 0x000000, 10);
//	printf(" %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", 
//			rx_buf[0], rx_buf[1], rx_buf[2], rx_buf[3], rx_buf[4], 
//			rx_buf[5], rx_buf[6], rx_buf[7], rx_buf[8], rx_buf[9]);
}

void Touch_Adjust_Test(void){
	LCD_Init();
	Touch_Init();	//里面启动触摸校准
//	printf("%f, %d, %f, %d\n", Pen_Point.xfac, Pen_Point.xoff, Pen_Point.yfac, Pen_Point.yoff);
	bsp_Delayms(10);
	while(1){
		
		if(Pen_Point.Key_Sta == Key_Down){
			Convert_Pos();		//读取坐标并转换
			Pen_Point.Key_Sta=Key_Up;
			LCD_DrawPoint(Pen_Point.X, Pen_Point.Y);
		}
	}
}

void Touch_Test(void){
	LCD_Init();
	Touch_Init();
	
	while(1){
		if(Pen_Point.Key_Sta == Key_Down){
			uint16_t res = XPT_Read_AD(0xD0);
			Pen_Point.Key_Sta=Key_Up;
			printf("%d\n", res);
		}
	}
	//		if(Pen_Point.Key_Sta == Key_Down){
//			Convert_Pos();
//			Pen_Point.Key_Sta=Key_Up;
//			printf("%d, %d\n", Pen_Point.X0, Pen_Point.Y0);
//			Pen_Point.Key_Sta = Key_Up;
//			uint16_t res = XPT_Read_AD(0xD0);
//			uint16_t res = XPT_Read_XY(0xD0);
//			Pen_Point.Key_Sta=Key_Up;
//			if(res != 0 && res > 100){
//				printf("%d\n", res);
//			}
//		}
}

void LCD_Direction_Test(void){
	LCD_Init();
	LCD_direction(1);
	DrawTestPage(NULL);
	while(1){
		
	}
}

void LCD_Touch_Drawing(void){
	LCD_Init();
	Touch_Init();
	
	while(1){
		if(Pen_Point.Key_Sta == Key_Down){
			Convert_Pos();		//读取坐标并转换
			Pen_Point.Key_Sta=Key_Up;
			LCD_DrawPoint(Pen_Point.X, Pen_Point.Y);
		}
	}	
}
