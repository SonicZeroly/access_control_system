#include "ctpiic.h"
#include "dwt.h"	 

/*****************************************************************************
 * @name       :void CTP_Delay(void)
 * @date       :2020-05-13 
 * @function   :Delay in controlling IIC speed
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_Delay(void)
{
	bsp_Delayus(1);
} 

/*****************************************************************************
 * @name       :void CTP_IIC_Init(void)
 * @date       :2020-05-13 
 * @function   :Initialize IIC
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_IIC_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pins : PB8 PB9 */
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
	CTP_IIC_SCL(1);
	CTP_IIC_SDA(1);
}

/**
 * @brief SDA�����������ģʽ
 * @param  ��
 * @return ��
 */
static void CTP_SDA_OUT(void)
{
    GPIO_InitTypeDef SOFT_IIC_GPIO_STRUCT;
    SOFT_IIC_GPIO_STRUCT.Mode = GPIO_MODE_OUTPUT_PP;
    SOFT_IIC_GPIO_STRUCT.Pin = CTP_SDA_PIN;
    SOFT_IIC_GPIO_STRUCT.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(CTP_SDA_PORT, &SOFT_IIC_GPIO_STRUCT);
}

/**
 * @brief SDA������������ģʽ
 * @param  ��
 * @return ��
 */
static void CTP_SDA_IN(void)
{
    GPIO_InitTypeDef SOFT_IIC_GPIO_STRUCT;
    SOFT_IIC_GPIO_STRUCT.Mode = GPIO_MODE_INPUT;
    SOFT_IIC_GPIO_STRUCT.Pin = CTP_SDA_PIN;
    SOFT_IIC_GPIO_STRUCT.Speed = GPIO_SPEED_FREQ_HIGH;
	SOFT_IIC_GPIO_STRUCT.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(CTP_SDA_PORT, &SOFT_IIC_GPIO_STRUCT);
}

/*****************************************************************************
 * @name       :void CTP_IIC_Start(void)
 * @date       :2020-05-13 
 * @function   :Generating IIC starting signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_IIC_Start(void)
{
	CTP_SDA_OUT();     //sda�����
	CTP_IIC_SDA(1);	  	  
	CTP_IIC_SCL(1);
	CTP_Delay();
 	CTP_IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	CTP_Delay();
	CTP_IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	

/*****************************************************************************
 * @name       :void CTP_IIC_Stop(void)
 * @date       :2020-05-13 
 * @function   :Generating IIC stop signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/   
void CTP_IIC_Stop(void)
{ 
	CTP_SDA_OUT();//sda�����
	CTP_IIC_SCL(0);
	CTP_IIC_SDA(0);
	CTP_Delay();
	CTP_IIC_SCL(1);
	CTP_Delay();
	CTP_IIC_SDA(1);//STOP:when CLK is high DATA change form low to high 
}

/*****************************************************************************
 * @name       :u8 CTP_IIC_Wait_Ack(void)
 * @date       :2020-05-13 
 * @function   :Wait for the response signal
 * @parameters :None
 * @retvalue   :0-receive response signal successfully
								1-receive response signal unsuccessfully
******************************************************************************/ 
u8 CTP_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CTP_SDA_IN();      //SDA����Ϊ����  
	CTP_IIC_SDA(1);bsp_Delayus(1);	   
	CTP_IIC_SCL(1);bsp_Delayus(1);	 
	while(CTP_READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CTP_IIC_Stop();
			return 1;
		} 
	}
	CTP_IIC_SCL(0);//ʱ�����0 	   
	return 0;  
} 

/*****************************************************************************
 * @name       :void CTP_IIC_Ack(void)
 * @date       :2020-05-13 
 * @function   :Generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void CTP_IIC_Ack(void)
{
	CTP_IIC_SCL(0);
	CTP_SDA_OUT();
	CTP_IIC_SDA(0);
	CTP_Delay();
	CTP_IIC_SCL(1);
	CTP_Delay();
	CTP_IIC_SCL(0);
}

/*****************************************************************************
 * @name       :void CTP_IIC_NAck(void)
 * @date       :2020-05-13 
 * @function   :Don't generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	    
void CTP_IIC_NAck(void)
{
	CTP_IIC_SCL(0);
	CTP_SDA_OUT();
	CTP_IIC_SDA(1);
	CTP_Delay();
	CTP_IIC_SCL(1);
	CTP_Delay();
	CTP_IIC_SCL(0);
}	

/*****************************************************************************
 * @name       :void CTP_IIC_Send_Byte(u8 txd)
 * @date       :2020-05-13 
 * @function   :send a byte data by IIC bus
 * @parameters :txd:Data to be sent
 * @retvalue   :None
******************************************************************************/					 				     		  
void CTP_IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	CTP_SDA_OUT(); 	    
  CTP_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
  for(t=0;t<8;t++)
  {
    CTP_IIC_SDA((txd&0x80)>>7);
    txd<<=1; 	      
		CTP_IIC_SCL(1);
		CTP_Delay();
		CTP_IIC_SCL(0);	
		CTP_Delay();
  }	 
} 	

/*****************************************************************************
 * @name       :u8 CTP_IIC_Read_Byte(unsigned char ack)
 * @date       :2020-05-13 
 * @function   :read a byte data by IIC bus
 * @parameters :ack:0-send nACK
									  1-send ACK
 * @retvalue   :Data to be read
******************************************************************************/	    
u8 CTP_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CTP_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        CTP_IIC_SCL(0); 	    	   
		bsp_Delayus(3);
		CTP_IIC_SCL(1);  
		receive<<=1;
		if(CTP_READ_SDA())receive++;   
	}	  				 
	if (!ack)CTP_IIC_NAck();//����nACK
	else CTP_IIC_Ack(); //����ACK   
 	return receive;
}




























