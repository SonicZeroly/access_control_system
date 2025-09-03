#ifndef __CTPIIC_H
#define __CTPIIC_H
#include "main.h"	    

#define u8 uint8_t
#define u16 uint16_t

/****************��������****************/

#define CTP_SDA_PORT	GPIOB
#define CTP_SDA_PIN		GPIO_PIN_9
#define CTP_SCL_PORT	GPIOB
#define CTP_SCL_PIN		GPIO_PIN_8
  	   		   
//IIC IO��������
//#define CTP_SDA_IN()  {GPIOF->MODER&=~(3<<(11*2));GPIOF->MODER|=0<<11*2;}
//#define CTP_SDA_OUT() {GPIOF->MODER&=~(3<<(11*2));GPIOF->MODER|=1<<11*2;}

//IO��������	 
#define CTP_IIC_SCL(val)    HAL_GPIO_WritePin(CTP_SCL_PORT, CTP_SCL_PIN, val) 			//SCL     
#define CTP_IIC_SDA(val)    HAL_GPIO_WritePin(CTP_SDA_PORT, CTP_SDA_PIN, val) 			//SDA	 
#define CTP_READ_SDA()   	HAL_GPIO_ReadPin(CTP_SDA_PORT, CTP_SDA_PIN) 	  			//����SDA 

//IIC���в�������
void CTP_IIC_Init(void);                	//��ʼ��IIC��IO��				 
void CTP_IIC_Start(void);				//����IIC��ʼ�ź�
void CTP_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CTP_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 CTP_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 CTP_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void CTP_IIC_Ack(void);					//IIC����ACK�ź�
void CTP_IIC_NAck(void);					//IIC������ACK�ź�

#endif







