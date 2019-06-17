
#include "dev.h"

static void vl53IICStart(void);			//����iic��ʼ�ź�
static void vl53IICStop(void);	  		//����iicֹͣ�ź�
static void vl53IICAck(void);			//iic����ACK�ź�
static void vl53IICNAck(void);			//iic������ACK�ź� 
static unsigned char vl53IICWaitAck(void);			//iic�ȴ�ACK�ź�
static void vl53IICSendByte(unsigned char txd);	//iic����һ���ֽ�
static unsigned char vl53IICReceiveByte(unsigned char ack);	//iic��ȡһ���ֽ�


/*IO��������*/
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ
/*IO��������*/	 
#define READ_SDA	  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9) 

void iic_delay(unsigned int us)
{
	us *= 2;
	while(us--);
}

//��ʼ��iic
void vl53IICInit(void)
{	
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
}
//����VL53��ʼ�ź�
static void vl53IICStart(void)
{
	SDA_OUT();     //sda�����
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);	  	  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	iic_delay(4);
 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	iic_delay(4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����VL53ֹͣ�ź�
static void vl53IICStop(void)
{
	SDA_OUT();//sda�����
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	iic_delay(4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);//����I2C���߽����ź�
	iic_delay(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
static unsigned char vl53IICWaitAck(void)
{
	unsigned char ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);iic_delay(1);	   
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);iic_delay(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			vl53IICStop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
static void vl53IICAck(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	SDA_OUT();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	iic_delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	iic_delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
}
//������ACKӦ��		    
static void vl53IICNAck(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	SDA_OUT();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	iic_delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	iic_delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
}					 				     
//VL53����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
static void vl53IICSendByte(unsigned char txd)
{                        
    unsigned char t;   
	SDA_OUT(); 	    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
			
        if((txd&0x80)>>7)
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
				}
				else
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
				}
			
        txd<<=1; 	  
				iic_delay(1);   
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
				iic_delay(1); 
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);	
				iic_delay(1);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
static unsigned char vl53IICReceiveByte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); 
        iic_delay(1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
        receive<<=1;
        if(READ_SDA)receive++;   
		iic_delay(1); 
    }					 
    if (!ack)
        vl53IICNAck();//����nACK
    else
        vl53IICAck(); //����ACK   
    return receive;
}

//��ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
unsigned char vl53IICReadByte(unsigned char devaddr,unsigned char addr, unsigned char* data)
{				  
	unsigned char temp=0;		  	    																 
	vl53IICStart();  
	vl53IICSendByte(devaddr);//��������д���� 	   
	vl53IICWaitAck(); 
	vl53IICSendByte(addr);   //���͵͵�ַ
	vl53IICWaitAck();	

	vl53IICStart();  	 	   
	vl53IICSendByte(devaddr|1);//��������������			   
	vl53IICWaitAck();	 
	temp=vl53IICReceiveByte(0);			   
	vl53IICStop();//����һ��ֹͣ����	 
	*data = temp;
	return temp;
}
//����������ֽ�
//addr:��ʼ��ַ
//rbuf:�����ݻ���
//len:���ݳ���
void vl53IICRead(unsigned char devaddr,unsigned char addr,unsigned char len,unsigned char *rbuf)
{
	int i=0;
	vl53IICStart();  
	vl53IICSendByte(devaddr);  
	vl53IICWaitAck();	
	vl53IICSendByte(addr);//��ַ����  
	vl53IICWaitAck();	

	vl53IICStart();  	
	vl53IICSendByte(devaddr|1);  	
	vl53IICWaitAck();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i] = vl53IICReceiveByte(0);//���һ���ֽڲ�Ӧ��
		}
		else
			rbuf[i] = vl53IICReceiveByte(1);
	}
	vl53IICStop( );	
}
//����������ֽ�
//addr:��ʼ��ַ
//rbuf:�����ݻ���
//len:���ݳ���
void vl53l1Read(unsigned char devaddr,unsigned short addr,unsigned char len,unsigned char *rbuf)
{
	int i=0;
	vl53IICStart();  
	vl53IICSendByte(devaddr);  
	vl53IICWaitAck();	
	vl53IICSendByte(addr>>8);  //��ַ��λ
	vl53IICWaitAck();
	vl53IICSendByte(addr&0x00FF);  //��ַ��λ
	vl53IICWaitAck();	

	vl53IICStart();  	
	vl53IICSendByte(devaddr|1);  	
	vl53IICWaitAck();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i] = vl53IICReceiveByte(0);//���һ���ֽڲ�Ӧ��
		}
		else
			rbuf[i] = vl53IICReceiveByte(1);
	}
	vl53IICStop( );	
}
//��ָ����ַд��һ������
//WriteAddr :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void vl53IICWriteByte(unsigned char devaddr,unsigned char addr,unsigned char data)
{				   	  	    																 
	vl53IICStart();  
	vl53IICSendByte(devaddr); //��������д���� 	 
	vl53IICWaitAck();	   
	vl53IICSendByte(addr);   //���͵͵�ַ
	vl53IICWaitAck(); 	 										  		   
	vl53IICSendByte(data); //�����ֽ�							   
	vl53IICWaitAck();  		    	   
	vl53IICStop();		//����һ��ֹͣ���� 	 
}

//����д����ֽ�
//addr:��ʼ��ַ
//wbuf:д���ݻ���
//len:���ݵĳ���
void vl53IICWrite(unsigned char devaddr,unsigned char addr,unsigned char len,unsigned char *wbuf)
{
	int i=0;
	vl53IICStart();  
	vl53IICSendByte(devaddr);  	
	vl53IICWaitAck();	
	vl53IICSendByte(addr);  //��ַ����
	vl53IICWaitAck();	
	for(i=0; i<len; i++)
	{
		vl53IICSendByte(wbuf[i]);  
		vl53IICWaitAck();		
	}
	vl53IICStop( );	
}
//����д����ֽ�
//addr:��ʼ��ַ
//wbuf:д���ݻ���
//len:���ݵĳ���
void vl53l1Write(unsigned char devaddr,unsigned short addr,unsigned char len,unsigned char *wbuf)
{
	int i=0;
	vl53IICStart();  
	vl53IICSendByte(devaddr);  	
	vl53IICWaitAck();	
	vl53IICSendByte(addr>>8);  //��ַ��λ
	vl53IICWaitAck();
	vl53IICSendByte(addr&0x00FF);  //��ַ��λ
	vl53IICWaitAck();	
	for(i=0; i<len; i++)
	{
		vl53IICSendByte(wbuf[i]);  
		vl53IICWaitAck();		
	}
	vl53IICStop( );	
}
//iic д��ĳ��λ
unsigned char vl53IICWriteBit(unsigned char devaddr,unsigned char addr, unsigned char bitNum, unsigned char data)
{
    unsigned char byte;
    vl53IICReadByte(devaddr, addr, &byte);
    byte = (data != 0) ? (byte | (1 << bitNum)) : (byte & ~(1 << bitNum));
    vl53IICWriteByte(devaddr, addr, byte);
	return 1;
}







