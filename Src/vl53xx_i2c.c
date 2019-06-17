
#include "dev.h"

static void vl53IICStart(void);			//发送iic开始信号
static void vl53IICStop(void);	  		//发送iic停止信号
static void vl53IICAck(void);			//iic发送ACK信号
static void vl53IICNAck(void);			//iic不发送ACK信号 
static unsigned char vl53IICWaitAck(void);			//iic等待ACK信号
static void vl53IICSendByte(unsigned char txd);	//iic发送一个字节
static unsigned char vl53IICReceiveByte(unsigned char ack);	//iic读取一个字节


/*IO方向设置*/
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式
/*IO操作函数*/	 
#define READ_SDA	  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9) 

void iic_delay(unsigned int us)
{
	us *= 2;
	while(us--);
}

//初始化iic
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
//产生VL53起始信号
static void vl53IICStart(void)
{
	SDA_OUT();     //sda线输出
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);	  	  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	iic_delay(4);
 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	iic_delay(4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);//钳住I2C总线，准备发送或接收数据 
}	  
//产生VL53停止信号
static void vl53IICStop(void)
{
	SDA_OUT();//sda线输出
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	iic_delay(4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);//发送I2C总线结束信号
	iic_delay(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
static unsigned char vl53IICWaitAck(void)
{
	unsigned char ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//VL53发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
static void vl53IICSendByte(unsigned char txd)
{                        
    unsigned char t;   
	SDA_OUT(); 	    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);//拉低时钟开始数据传输
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
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
static unsigned char vl53IICReceiveByte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        vl53IICNAck();//发送nACK
    else
        vl53IICAck(); //发送ACK   
    return receive;
}

//从指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
unsigned char vl53IICReadByte(unsigned char devaddr,unsigned char addr, unsigned char* data)
{				  
	unsigned char temp=0;		  	    																 
	vl53IICStart();  
	vl53IICSendByte(devaddr);//发送器件写命令 	   
	vl53IICWaitAck(); 
	vl53IICSendByte(addr);   //发送低地址
	vl53IICWaitAck();	

	vl53IICStart();  	 	   
	vl53IICSendByte(devaddr|1);//发送器件读命令			   
	vl53IICWaitAck();	 
	temp=vl53IICReceiveByte(0);			   
	vl53IICStop();//产生一个停止条件	 
	*data = temp;
	return temp;
}
//连续读多个字节
//addr:起始地址
//rbuf:读数据缓存
//len:数据长度
void vl53IICRead(unsigned char devaddr,unsigned char addr,unsigned char len,unsigned char *rbuf)
{
	int i=0;
	vl53IICStart();  
	vl53IICSendByte(devaddr);  
	vl53IICWaitAck();	
	vl53IICSendByte(addr);//地址自增  
	vl53IICWaitAck();	

	vl53IICStart();  	
	vl53IICSendByte(devaddr|1);  	
	vl53IICWaitAck();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i] = vl53IICReceiveByte(0);//最后一个字节不应答
		}
		else
			rbuf[i] = vl53IICReceiveByte(1);
	}
	vl53IICStop( );	
}
//连续读多个字节
//addr:起始地址
//rbuf:读数据缓存
//len:数据长度
void vl53l1Read(unsigned char devaddr,unsigned short addr,unsigned char len,unsigned char *rbuf)
{
	int i=0;
	vl53IICStart();  
	vl53IICSendByte(devaddr);  
	vl53IICWaitAck();	
	vl53IICSendByte(addr>>8);  //地址高位
	vl53IICWaitAck();
	vl53IICSendByte(addr&0x00FF);  //地址低位
	vl53IICWaitAck();	

	vl53IICStart();  	
	vl53IICSendByte(devaddr|1);  	
	vl53IICWaitAck();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i] = vl53IICReceiveByte(0);//最后一个字节不应答
		}
		else
			rbuf[i] = vl53IICReceiveByte(1);
	}
	vl53IICStop( );	
}
//从指定地址写入一个数据
//WriteAddr :写入数据的目的地址    
//DataToWrite:要写入的数据
void vl53IICWriteByte(unsigned char devaddr,unsigned char addr,unsigned char data)
{				   	  	    																 
	vl53IICStart();  
	vl53IICSendByte(devaddr); //发送器件写命令 	 
	vl53IICWaitAck();	   
	vl53IICSendByte(addr);   //发送低地址
	vl53IICWaitAck(); 	 										  		   
	vl53IICSendByte(data); //发送字节							   
	vl53IICWaitAck();  		    	   
	vl53IICStop();		//产生一个停止条件 	 
}

//连续写多个字节
//addr:起始地址
//wbuf:写数据缓存
//len:数据的长度
void vl53IICWrite(unsigned char devaddr,unsigned char addr,unsigned char len,unsigned char *wbuf)
{
	int i=0;
	vl53IICStart();  
	vl53IICSendByte(devaddr);  	
	vl53IICWaitAck();	
	vl53IICSendByte(addr);  //地址自增
	vl53IICWaitAck();	
	for(i=0; i<len; i++)
	{
		vl53IICSendByte(wbuf[i]);  
		vl53IICWaitAck();		
	}
	vl53IICStop( );	
}
//连续写多个字节
//addr:起始地址
//wbuf:写数据缓存
//len:数据的长度
void vl53l1Write(unsigned char devaddr,unsigned short addr,unsigned char len,unsigned char *wbuf)
{
	int i=0;
	vl53IICStart();  
	vl53IICSendByte(devaddr);  	
	vl53IICWaitAck();	
	vl53IICSendByte(addr>>8);  //地址高位
	vl53IICWaitAck();
	vl53IICSendByte(addr&0x00FF);  //地址低位
	vl53IICWaitAck();	
	for(i=0; i<len; i++)
	{
		vl53IICSendByte(wbuf[i]);  
		vl53IICWaitAck();		
	}
	vl53IICStop( );	
}
//iic 写入某个位
unsigned char vl53IICWriteBit(unsigned char devaddr,unsigned char addr, unsigned char bitNum, unsigned char data)
{
    unsigned char byte;
    vl53IICReadByte(devaddr, addr, &byte);
    byte = (data != 0) ? (byte | (1 << bitNum)) : (byte & ~(1 << bitNum));
    vl53IICWriteByte(devaddr, addr, byte);
	return 1;
}







