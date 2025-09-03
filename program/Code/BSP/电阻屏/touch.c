/**
  ******************************************************************************
  * @file    touch.c
  * @brief   ��ʾ������������IC�ͺ�:xpt2046��
  ******************************************************************************
  */
#include "lcd.h"
#include "touch.h"
#include "dwt.h"
#include <math.h>
#include "stdlib.h"

#define SW_SPI		(1)
#define HW_SPI		(2)
#define SPI_VERSION		HW_SPI

SPI_HandleTypeDef hspi1;
Pen_Holder Pen_Point;
Touch_State tp_state = TOUCH_IDLE;
/***************�ײ�����******************/

/**
 * @brief  ��XPT2046д������
 * @retval void
 */
#if SPI_VERSION == HW_SPI
//void XPT_Write_Byte(uint8_t data){
//	CS_PIN_SET(0);
//	uint8_t tx_data = data;
//	uint8_t rx_data = 0;
//	HAL_SPI_TransmitReceive(&hspi1, &tx_data, &rx_data, 1, HAL_MAX_DELAY);
//	CS_PIN_SET(1);
//}
#elif SPI_VERSION == SW_SPI
void XPT_Write_Byte(uint8_t data){
	uint8_t count = 0;
	for(count=0;count<8;count++){
		if(data & 0x80)		MOSI_PIN_SET(1);
		else 				MOSI_PIN_SET(0);
		
		data<<=1;
//		SCLK_PIN_SET(1);
//		bsp_Delayus(1);
//		SCLK_PIN_SET(0);
//		bsp_Delayus(1);
//		SCLK_PIN_SET(1);
		
		// ʱ����������Ч
        SCLK_PIN_SET(0);
        SCLK_PIN_SET(1);
	}			
}
#endif
/**
 * @brief  ��XPT2046��������
 * @retval void
 */
#if SPI_VERSION == HW_SPI
uint16_t XPT_Read_AD(uint8_t CMD){
	if(INT_PIN_STATE == 1){
		return 0;
	}
	
	CS_PIN_SET(0);
	uint8_t rx_data[2] = {0};
	uint8_t tx_data = CMD;
	
	HAL_SPI_TransmitReceive(&hspi1, &tx_data, rx_data, 1, 1);
	
	//��ʱ�ȴ�ת��
	for(int i=50; i>0; i--);
	tx_data = 0;
	
	//��ȡ16λ����
	HAL_SPI_TransmitReceive(&hspi1, &tx_data, rx_data, 2, 1);
	
	//�������ݣ������м�12λ����ʱ��ͼ��֪����һλ�ͺ���λ��Ϊ��Ч��
	uint16_t val = rx_data[0]<<8 | rx_data[1];
	val &= ~(1<<15);
	val>>=3;
	
	CS_PIN_SET(1);
	return val;
}
#elif SPI_VERSION == SW_SPI
uint16_t XPT_Read_AD(uint8_t CMD){
	uint8_t count = 0;
	uint16_t data = 0;
	SCLK_PIN_SET(0);
//	MOSI_PIN_SET(0);
	CS_PIN_SET(0);
	
	XPT_Write_Byte(CMD);
//	bsp_Delayus(6);
//	SCLK_PIN_SET(0);
//	bsp_Delayus(1);
//	SCLK_PIN_SET(1);
//	bsp_Delayus(1);	
//	SCLK_PIN_SET(0);
	
//	for(count=0;count<16;count++){//����16λ����,ֻ�и�12λ��Ч 	  
//		data<<=1; 	 
//		SCLK_PIN_SET(0);	//�½�����Ч  	    	   
//		bsp_Delayus(1);    
//		SCLK_PIN_SET(1);
//		if(MISO_PIN_STATE)	data++; 		 
//	}

    for(int i=100;i>0;i--);
	SCLK_PIN_SET(1);
	SCLK_PIN_SET(0);
    // ��ȡ16λ���ݣ�12λ��Ч��
    for(uint8_t i = 0; i < 16; i++){
        
        data <<= 1;
        SCLK_PIN_SET(0);
		SCLK_PIN_SET(1);
		if(MISO_PIN_STATE) data |= 0x01;
    }
	
	data>>=4;   	//ֻ�и�12λ��Ч.
	CS_PIN_SET(1);		//�ͷ�Ƭѡ	 
	return data;  
}
#endif
/**********************************************/

#define READ_TIMES 15 //��ȡ����
#define LOST_VAL 5	  //����ֵ

/**
 * @brief  ��XPT2046�������겢����ȡƽ��
 * @param  xy������������
 * @retval void
 */
uint16_t XPT_Read_XY(uint8_t xy_cmd){
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++){				 
		buf[i]=XPT_Read_AD(xy_cmd);	    
	}
	
	for(i=0;i<READ_TIMES-1; i++){
		for(j=i+1;j<READ_TIMES;j++){
			if(buf[i]>buf[j]){
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++){
		sum+=buf[i];
	}
	temp=(uint16_t)(sum/(READ_TIMES-2*LOST_VAL));
	return temp;
}

/**
 * @brief  ���˲��������ȡ����Сֵ��������100. 
 * @retval void
 */
uint8_t XPT_Read(uint16_t *x,uint16_t *y){
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=XPT_Read_XY(CMD_RDX);
	ytemp=XPT_Read_XY(CMD_RDY);	  					
	
	if(xtemp<100||ytemp<100)
		return 0;              	//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;
}

#define ERR_RANGE 50 //��Χ 
/**
 * @brief  2�ζ�ȡxpt2046���������ε�ƫ��ܳ���50
 * @retval void
 */
uint8_t XPT_Read_2(uint16_t *x,uint16_t *y){
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag=XPT_Read(&x1,&y1);   
    if(flag==0){
		return(0);
	}
	
    flag = XPT_Read(&x2,&y2);	   
    if(flag==0){
		return(0);
	}
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else{
		return 0;
	}
}

void Pen_Int_Set(uint8_t en_val)
{
	if(en_val)		EXTI->IMR|=1<<4;        //����line4�ϵ��ж�	  	
	else 			EXTI->IMR&=~(1<<4);     //�ر�line4�ϵ��ж�	   
}

/****************************************/
/****************************************/

/**
 * @brief  ��ȡһ������ֵ,ֱ��PEN�ɿ��ŷ���
 * @retval void
 */
uint8_t Read_TP_Once(void)
{
	uint8_t t=0;	    
//	Pen_Int_Set(0);                            //�ر��ж�
	HAL_NVIC_DisableIRQ(EXTI4_IRQn);
	XPT_Read_2(&Pen_Point.X0, &Pen_Point.Y0);
	Pen_Point.Key_Sta=Key_Up;				//����������Ҫ�ֶ�ֹͣ
	while(INT_PIN_STATE == 0 && t <= 250)
	{
		t++;
		bsp_Delayms(10);
	};
//	Pen_Int_Set(1);                             //�����ж�
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);	
	if(t>=250)	return 0;                         //����2.5s ��Ϊ��Ч
	else return 1;	
}

void Drow_Touch_Point(uint8_t x,uint16_t y)
{
	LCD_DrawLine(x-12,y,x+13,y);              //����
	LCD_DrawLine(x,y-12,x,y+13);              //����
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	LCD_Draw_Circle(x,y,6);                       //������Ȧ
}	 

extern uint8_t it_cnt;
/**
 * @brief  У׼�������õ��ĸ�У׼����
 * @retval void
 */
void Touch_Adjust(void){
	uint16_t pos_temp[4][2];                    //���껺��ֵ
	uint8_t  cnt=0;	
	uint16_t d1,d2;
	uint32_t tem1,tem2;
	float fac;
	cnt=0;			
	
	LCD_direction(0);	//����
	LCD_Clear(WHITE);                           //����   
	Drow_Touch_Point(20,20);                    //����1 
	Pen_Point.Key_Sta=Key_Up;                   //���������ź� 
	Pen_Point.xfac=0;		//xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������	 
	while(1)
	{
//		printf("%d,%d\n", cnt,it_cnt);
		printf("%d,%d,%d\r\n", Pen_Point.X0, Pen_Point.Y0, tp_state);
		if(Pen_Point.Key_Sta==Key_Down)           //����������
		{
			if(Read_TP_Once())                      //�õ����ΰ���ֵ
			{  								   
				pos_temp[cnt][0]=Pen_Point.X0;
				pos_temp[cnt][1]=Pen_Point.Y0;
				cnt++;
			}			 
			switch(cnt)
			{			   
				case 1:
					LCD_Clear(WHITE);                    //���� 
					Drow_Touch_Point(220,20);            //����2
					break;
				case 2:
					LCD_Clear(WHITE);                    //���� 
					Drow_Touch_Point(20,300);            //����3
					break;
				case 3:
					LCD_Clear(WHITE);                    //���� 
					Drow_Touch_Point(220,300);           //����4
					break;
				case 4:	                               //ȫ���ĸ����Ѿ��õ�
	    		    	                                            //�Ա����
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);          //x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);          //y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);                               //�õ�1,2�ľ���
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);          //x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);          //y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);                               //�õ�3,4�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)              //���ϸ�
					{
						cnt=0;
						LCD_Clear(WHITE);                               //���� 
						Drow_Touch_Point(20,20);
						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);          //x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);          //y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);                               //�õ�1,3�ľ���
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);          //x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);          //y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);                               //�õ�2,4�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)                            //���ϸ�
					{
						cnt=0;
						LCD_Clear(WHITE);                               //���� 
						Drow_Touch_Point(20,20);
						continue;
					}                                                 //��ȷ��
								   
					//�Խ������
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);          //x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);          //y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);                               //�õ�1,4�ľ���
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);          //x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);          //y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);                               //�õ�2,3�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)                            //���ϸ�
					{
						cnt=0;
						LCD_Clear(WHITE);                                //���� 
						Drow_Touch_Point(20,20);
						continue;
					}                                                  //��ȷ��
					                                                   //������
					Pen_Point.xfac=(float)200/(pos_temp[1][0]-pos_temp[0][0]);            //�õ�xfac		 
					Pen_Point.xoff=(240-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//�õ�xoff
						  
					Pen_Point.yfac=(float)280/(pos_temp[2][1]-pos_temp[0][1]);            //�õ�yfac
					Pen_Point.yoff=(320-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//�õ�yoff  
					
					LCD_Clear(WHITE);                                                     //����
					LCD_ShowString(35,110,12,"Touch Screen Adjust OK!",1);           //��ʾУ�����
					bsp_Delayms(1000);
					LCD_Clear(WHITE);                                                     //����   
					return;                                                               //У�����				 
			}
		}
	} 	
}

void Save_Adjdata(void){
	
}

uint8_t Get_Adjdata(void){
//	Pen_Point.xfac = 0.076658; 
//	Pen_Point.xoff = -27;
//	Pen_Point.yfac = 0.099220;
//	Pen_Point.yoff = -25;
//	return 1;
	
//	Pen_Point.xfac = 0.079114; 
//	Pen_Point.xoff = -33;
//	Pen_Point.yfac = 0.104361;
//	Pen_Point.yoff = -37;
//	return 1;
	
	return 0;
}
	
/**
 * @brief  ����ת��
 * @retval void
 */
void Convert_Pos(void)
{		 	  
	uint16_t temp_x, temp_y;
	if(XPT_Read_2(&Pen_Point.X0,&Pen_Point.Y0))
	{
		temp_x=Pen_Point.xfac*Pen_Point.X0+Pen_Point.xoff;
		temp_y=Pen_Point.yfac*Pen_Point.Y0+Pen_Point.yoff;  
	}
	
	if(lcddev.dir == 0){
		Pen_Point.X = temp_x;
		Pen_Point.Y = temp_y;
	}
	else if(lcddev.dir == 1){
		Pen_Point.Y = temp_x;
		Pen_Point.X = temp_y;		
	}
}

/**
 * @brief  �����жϼ��
 * @retval void
 */
uint32_t start_time, release_time = 0;

void Touch_Falling_PIT(void){
	if(INT_PIN_STATE == 0 && tp_state==TOUCH_IDLE){
		start_time = HAL_GetTick();
		tp_state = TOUCH_PRESSED;
	}
}

void Touch_Process(void){
	uint8_t pin_sta = INT_PIN_STATE;
	switch(tp_state){
		
		case TOUCH_PRESSED:
			if(pin_sta == 0 && HAL_GetTick()-start_time > 50){
				Pen_Point.Key_Sta = Key_Down;
				tp_state = TOUCH_HELD;
			}
			else if(pin_sta == 1){
				tp_state = TOUCH_IDLE;
			}
			break;
		case TOUCH_HELD:
			if(pin_sta == 0 && HAL_GetTick()-start_time > 500){
				Pen_Point.Key_Sta = Key_Down;
			}
			else if(pin_sta == 1){
				Pen_Point.Key_Sta = Key_Up;
				tp_state = TOUCH_RELEASED;
				release_time = HAL_GetTick();
			}
			break;
		case TOUCH_RELEASED:
//			if(HAL_GetTick() - release_time > 50){
				tp_state = TOUCH_IDLE;
//			}
			break;
		default:
			Pen_Point.Key_Sta = Key_Up;
			break;
	}
	
}

void Touch_MspInit(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	
	/*Configure GPIO pin : PA4 */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;  // �е��ٶȣ���һ���˲���
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
//	GPIO_InitStruct.Pin = GPIO_PIN_4;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  // �е��ٶȣ���һ���˲���
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PA3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

#if SPI_VERSION == HW_SPI
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*SPI��ʼ��*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
//	__HAL_SPI_ENABLE(&hspi1);
#elif SPI_VERSION == SW_SPI
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
#endif
	/*EXTI�ж�����*/
	HAL_NVIC_SetPriority(EXTI4_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

void Touch_Init(){
	Touch_MspInit();
	XPT_Read(&Pen_Point.X0, &Pen_Point.Y0);                     //��һ�ζ�ȡ��ʼ��			 
	
	LCD_Clear(WHITE);     //����
	if(Get_Adjdata())
		return;           //�Ѿ�У׼
	else{ 		
		Touch_Adjust();   //��ĻУ׼ 
		Save_Adjdata();	  //����У׼ֵ
	}
	
	Get_Adjdata();        //��ȡУ׼ֵ	
}

//uint16_t touch_res1, touch_res2;
//void Touch_Test(){
//	Touch_MspInit();
//	touch_res1 = XPT_Read_AD(XPT_CMD_TEST_1);
//	touch_res2 = XPT_Read_AD(XPT_CMD_TEST_2);
//	
//}