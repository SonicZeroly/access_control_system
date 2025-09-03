#ifndef __FT6336_H
#define __FT6336_H	
#include "main.h"	

#define u8	uint8_t
#define u16 uint16_t

#define CTP_MAX_TOUCH 2

#define CTP_INT_PORT	GPIOB
#define CTP_INT_PIN		GPIO_PIN_7
#define CTP_RST_PORT	GPIOB
#define CTP_RST_PIN		GPIO_PIN_6

//����ݴ��������ӵ�оƬ����(δ����IIC����) 
//IO��������	 
#define FT_RST(val)    	HAL_GPIO_WritePin(CTP_RST_PORT, CTP_RST_PIN, val)	//FT6636��λ����
#define FT_INT()    	HAL_GPIO_ReadPin(CTP_INT_PORT, CTP_RST_PIN)			//FT6636�ж�����	

//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������
  
//FT5426 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT6336ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER   0x02		  //����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ

#define FT_ID_G_CIPHER_MID    0x9F      //оƬ���ţ����ֽڣ� Ĭ��ֵ0x26
#define FT_ID_G_CIPHER_LOW    0xA0      //оƬ���ţ����ֽڣ� 0x01: Ft6336G  0x02: Ft6336U 
#define	FT_ID_G_LIB_VERSION		0xA1		  //�汾		
#define FT_ID_G_CIPHER_HIGH   0xA3      //оƬ���ţ����ֽڣ� Ĭ��0x64 
#define FT_ID_G_MODE 			    0xA4   		//FT6636�ж�ģʽ���ƼĴ���
#define FT_ID_G_FOCALTECH_ID  0xA8      //VENDOR ID Ĭ��ֵΪ0x11
#define FT_ID_G_THGROUP			  0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���


u8 FT6336_WR_Reg(u16 reg,u8 *buf,u8 len);
void FT6336_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 FT6336_Init(void);
u8 FT6336_Scan(void);

#endif

















