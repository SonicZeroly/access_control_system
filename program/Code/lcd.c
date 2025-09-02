/**
  ******************************************************************************
  * @file    lcd.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  */
  
#include "main.h"
#include "lcd.h"
#include "dwt.h"
#include <stdlib.h>
#include "font.h"
  
_lcd_dev lcddev;
uint16_t POINT_COLOR = BLACK,BACK_COLOR = WHITE;  

/**********�ײ���������**************/

void LCD_write(uint16_t VAL)
{
	LCD_CS_RESET;  	//�͵�ƽʹ��
	DATAOUT(VAL);	//��16λ����д��16��IO��
	LCD_WR_RESET; 
	LCD_WR_SET; 
	LCD_CS_SET;
}

uint16_t LCD_read(void)
{
	uint16_t data;
	LCD_CS_RESET;
	LCD_RD_RESET;
	bsp_Delayus(1);	//��ʱ1us	
	data = DATAIN;
	LCD_RD_SET;
	LCD_CS_SET;
	return data;
}
/************************************/

/**
 * @brief  LCDд�����
 * @param  ��д��16λ����
 * @retval void
 */
void LCD_WR_REG(uint16_t data)
{ 
	LCD_RS_RESET;     
	#if LCD_USE8BIT_MODEL
	LCD_write(data<<8);
	#else
	LCD_write(data);
	#endif
}

void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

void LCD_ReadRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.rramcmd);
}

/**
 * @brief  LCDд���ݺ���
 * @param  ��д��16λ����
 * @retval void
 */
void LCD_WR_DATA(uint16_t data)
{
	LCD_RS_SET;
	#if LCD_USE8BIT_MODEL
	LCD_write(data<<8);
	#else
	LCD_write(data);
	#endif
}

/**
 * @brief  LCD�����ݺ���
 * @param  void
 * @retval void
 */
uint16_t LCD_RD_DATA(void)
{
	LCD_RS_SET; 
	#if LCD_USE8BIT_MODEL
	return (LCD_read()>>8);
	#else
	return LCD_read();
	#endif
}

/**
 * @brief  LCDд�Ĵ�������
 * @param  ��д��16λ����
 * @param  ��д��16λ����
 * @retval void
 */
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}

/**
 * @brief  LCD���Ĵ�������
 * @param  16λ����
 * @param  ���ձ�����������Ϊʲô��uint8_t��
 * @param  ���մ���
 * @retval void
 */
void LCD_ReadReg(uint16_t LCD_Reg,uint8_t *Rval,int n)
{
	LCD_WR_REG(LCD_Reg); 
	//��������IO��ģʽ
	LCD_DB_PORT->MODER = 0x00000000;  // ������������ģʽ
	LCD_DB_PORT->PUPDR = 0x55555555;  // ��������������0x5 = 0101��01ΪPull_up
	LCD_DB_PORT->ODR = 0XFFFF;     //ȫ�������
	while(n--)
	{
		*(Rval++) = LCD_RD_DATA();
	}
	
	LCD_DB_PORT->MODER = 0x55555555;  // �����������ģʽ
	LCD_DB_PORT->PUPDR = 0x55555555;  // ��������������0x5 = 0101��01ΪPull_up
	LCD_DB_PORT->ODR = 0XFFFF;    		//ȫ�������  
	
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//    LCD_WR_REG(LCD_Reg);
//    
//    // ����Ϊ����ģʽ
//    GPIO_InitStruct.Pin = GPIO_PIN_All;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    HAL_GPIO_Init(LCD_DB_PORT, &GPIO_InitStruct);
//    
//    // ��ȡ����
//    while(n--)
//    {
//        *(Rval++) = LCD_RD_DATA();
//    }
//    
//    // ��������Ϊ���ģʽ
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(LCD_DB_PORT, &GPIO_InitStruct);
//    
//    // �������������
//    HAL_GPIO_WritePin(LCD_DB_PORT, GPIO_PIN_All, GPIO_PIN_SET);
}

void Lcd_WriteData_16Bit(uint16_t Data)
{	
	LCD_RS_SET; 
	#if LCD_USE8BIT_MODEL
	LCD_CS_CLR;
	DATAOUT(Data);
	LCD_WR_CLR; 
	LCD_WR_SET;
	DATAOUT(Data<<8);
	LCD_WR_CLR; 
	LCD_WR_SET;
	LCD_CS_SET;
	#else
	LCD_write(Data);
	#endif
}

/**
 * @brief  ����LCD��ʾ����
 * @param  ��ʾ����
 * @retval void
 */
void LCD_direction(uint8_t direction)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
	lcddev.rramcmd=0x2E;
	lcddev.dir = direction;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;			//240
			lcddev.height=LCD_H;		//320		
			LCD_WriteReg(0x36,(1<<3));
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<5)|(1<<6));
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<4)|(1<<6));
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5)|(1<<4));
		break;	
		default:break;
	}		
}

/**
 * @brief  LCD���ô��ں���
 * @param  x�����㡢�յ���y�����㡢�յ�
 * @retval void
 */
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
}

/**
 * @brief  LCD���ù�꺯��
 * @param  x���ꡢy����
 * @retval void
 */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/**
 * @brief  LCD������亯��
 * @retval void
 */
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{  	
	uint16_t i,j;			
	uint16_t width=ex-sx+1; 		//�õ����Ŀ��
	uint16_t height=ey-sy+1;		//�߶�
	LCD_SetWindows(sx,sy,ex,ey);//������ʾ����
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		Lcd_WriteData_16Bit(color);	//д������ 	 
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���������Ϊȫ��
}

/**
 * @brief  LCD������ɫ������亯��
 * @retval void
 */
void LCD_ColorFill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t* color_p){
	uint16_t i,j;			
	uint16_t width=ex-sx+1; 		//�õ����Ŀ��
	uint16_t height=ey-sy+1;		//�߶�
	LCD_SetWindows(sx,sy,ex,ey);//������ʾ����
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++){
			Lcd_WriteData_16Bit(*color_p);	//д������ 
			color_p++;
//			Lcd_WriteData_16Bit(color_p[i * width + j]);
		}			
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���������Ϊȫ��	
}

/**
 * @brief  LCD���
 * @retval void
 */
void LCD_Clear(uint16_t Color)
{
	unsigned int i;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	for(i=0;i<lcddev.height*lcddev.width;i++)
	{
		Lcd_WriteData_16Bit(Color);
	}
} 

/**
 * @brief  LCD��λ
 * @retval void
 */
void LCD_RESET(void)
{
	LCD_RST_RESET;
	bsp_Delayms(100);	
	LCD_RST_SET;
	bsp_Delayms(50);
}

/**************Ӧ��*********************/

void LCD_DrawPoint(uint16_t xsta, uint16_t ysta)
{
	LCD_SetCursor(xsta,ysta);           //���ù��λ��  
//	LCD_WR_REG(0x22);                   //��ʼд��GRAM 
	LCD_WR_DATA(POINT_COLOR); 
}

void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
{
    uint16_t x, y, t;
	if((xsta==xend)&&(ysta==yend))	LCD_DrawPoint(xsta, ysta);
	else if(abs(yend-ysta)>abs(xend-xsta))                    //б�ʴ���1 
	{
		if(ysta>yend) 
		{
			t=ysta;
			ysta=yend;
			yend=t; 
			t=xsta;
			xsta=xend;
			xend=t; 
		}
		for(y=ysta;y<yend;y++)                                   //��y��Ϊ��׼ 
		{
			x=(uint32_t)(y-ysta)*(xend-xsta)/(yend-ysta)+xsta;
			LCD_DrawPoint(x, y);  
		}
	}
	else                                                       //б��С�ڵ���1 
	{
		if(xsta>xend)
		{
			t=ysta;
			ysta=yend;
			yend=t;
			t=xsta;
			xsta=xend;
			xend=t;
		}   
		for(x=xsta;x<=xend;x++)                                   //��x��Ϊ��׼
		{
			y =(uint32_t)(x-xsta)*(yend-ysta)/(xend-xsta)+ysta;
			LCD_DrawPoint(x,y); 
		}
	} 
}

void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                                        //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);                         //3           
		LCD_DrawPoint(x0+b,y0-a);                         //0           
		LCD_DrawPoint(x0-a,y0+b);                         //1       
		LCD_DrawPoint(x0-b,y0-a);                         //7           
		LCD_DrawPoint(x0-a,y0-b);                         //2             
		LCD_DrawPoint(x0+b,y0+a);                         //4               
		LCD_DrawPoint(x0+a,y0-b);                         //5
		LCD_DrawPoint(x0+a,y0+b);                         //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;

		/*ʹ��Bresenham�㷨��Բ*/     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 

void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode){  
    uint8_t temp;
    uint8_t pos,t;
	uint16_t colortemp=POINT_COLOR;      
		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	LCD_SetWindows(x,y,x+size/2-1,y+size-1);//���õ���������ʾ����
	if(!mode) //�ǵ��ӷ�ʽ
	{		
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)Lcd_WriteData_16Bit(fc); 
				else Lcd_WriteData_16Bit(bc); 
				temp>>=1; 
				
		    }
			
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)	temp=asc2_1206[num][pos];//����1206����
			else 			temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {
				POINT_COLOR=fc;              
		        if(temp&0x01)		LCD_DrawPoint(x+t,y+pos);//��һ����    
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��    	   	 	  
}

void LCD_ShowString(uint16_t x,uint16_t y,uint8_t size, uint8_t *p,uint8_t mode)
{         
	// ��ȫ��飺��ָ�����ַ���
    if(p == NULL || *p == '\0')
        return;
	
    while((*p!='\0') && (*p<='~') && (*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
		if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
		return;
        LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,size,mode);
        x+=size/2;
        p++;
    }  
} 

uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}

void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,0); 
	}
} 

/*************��ʼ��***********************/

void LCD_GPIOInit(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LCD_DB_PORT, GPIO_PIN_All, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, LCD_CS_PIN|LCD_RS_PIN|LCD_WR_PIN|LCD_RD_PIN
						  |LCD_RST_PIN|LCD_BL_PIN, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_All;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(LCD_DB_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_CS_PIN|LCD_RS_PIN|LCD_WR_PIN|LCD_RD_PIN
						  |LCD_RST_PIN|LCD_BL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);	
}

void LCD_Init(void)
{  
	LCD_GPIOInit();	//LCD GPIO��ʼ��	
 	LCD_RESET(); 	//LCD ��λ
//*************2.8inch ILI9341��ʼ��**********//	
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC9); //C1 
	LCD_WR_DATA(0X30); 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x00);   //SAP[2:0];BT[3:0] 01 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x08); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	bsp_Delayms(120);
	LCD_WR_REG(0x29); //display on	

	LCD_direction(USE_HORIZONTAL);//����LCD��ʾ����
	LCD_LED_ON;			//��������	 
	LCD_Clear(WHITE);	//��ȫ����ɫ
}

uint16_t LCD_Read_ID(void)
{
	uint8_t val[4] = {0};
	LCD_ReadReg(0xD3,val,4);
	return (val[2]<<8)|val[3];
}
