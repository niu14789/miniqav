
#include "main.h"
#include "w25qxx.h" 

unsigned short W25QXX_TYPE=W25Q64;	//Ĭ����W25Q128

extern SPI_HandleTypeDef hspi2;

void spi_write(unsigned char data)
{
	unsigned char d = data;
	HAL_SPI_Transmit(&hspi2,&d,1,0xffff);
}

unsigned char spi_read(void)
{
	unsigned char d;
	HAL_SPI_Receive(&hspi2,&d,1,0xffff);
	return d;
}
//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q128
//����Ϊ16M�ֽ�,����128��Block,4096��Sector 
													 
//��ʼ��SPI FLASH��IO��
void W25QXX_Init(void)
{ 
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);			//SPI FLASH��ѡ��
	W25QXX_TYPE=W25QXX_ReadID();	//��ȡFLASH ID.
}  

//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
unsigned char W25QXX_ReadSR(void)   
{  
	unsigned char byte=0;   
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                            //ʹ������   
	
	spi_write(W25X_ReadStatusReg);	//���Ͷ�ȡ״̬�Ĵ�������    
	byte=spi_read();        	//��ȡһ���ֽ�  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);                            //ȡ��Ƭѡ     
	return byte;   
} 
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25QXX_Write_SR(unsigned char sr)   
{   
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                            //ʹ������   
	spi_write(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	spi_write(sr);              	//д��һ���ֽ�  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);                            //ȡ��Ƭѡ     	      
}   
//W25QXXдʹ��	
//��WEL��λ   
void W25QXX_Write_Enable(void)   
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                            //ʹ������   
    spi_write(W25X_WriteEnable); 	//����дʹ��  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);                            //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void W25QXX_Write_Disable(void)   
{  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                            //ʹ������   
    spi_write(W25X_WriteDisable); 	//����д��ָֹ��    
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);                            //ȡ��Ƭѡ     	      
} 		
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
unsigned short W25QXX_ReadID(void)
{
	unsigned short Temp = 0;	  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);				    
	spi_write(0x90);//���Ͷ�ȡID����	    
	spi_write(0x00); 	    
	spi_write(0x00); 	    
	spi_write(0x00); 	 			   
	Temp|=spi_read()<<8;  
	Temp|=spi_read();	 
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);				    
	return Temp;
}   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(unsigned char* pBuffer,unsigned int ReadAddr,unsigned short NumByteToRead)   
{ 
 	unsigned short i;   										    
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                            	//ʹ������   
    spi_write(W25X_ReadData);   		//���Ͷ�ȡ����   
    spi_write((unsigned char)((ReadAddr)>>16));	//����24bit��ַ    
    spi_write((unsigned char)((ReadAddr)>>8));   
    spi_write((unsigned char)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	  { 
        pBuffer[i]=spi_read();   	//ѭ������  
    }
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(unsigned char* pBuffer,unsigned int WriteAddr,unsigned short NumByteToWrite)
{
 	unsigned short i;  
    W25QXX_Write_Enable();                  //SET WEL 
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                            //ʹ������   
    spi_write(W25X_PageProgram);      //����дҳ����   
    spi_write((unsigned char)((WriteAddr)>>16)); //����24bit��ַ    
    spi_write((unsigned char)((WriteAddr)>>8));   
    spi_write((unsigned char)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)spi_write(pBuffer[i]);//ѭ��д��  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);                 			//ȡ��Ƭѡ 
	W25QXX_Wait_Busy();						//�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(unsigned char* pBuffer,unsigned int WriteAddr,unsigned short NumByteToWrite)   
{ 			 		 
	unsigned short pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535) 
#if W25QXX_USE_MALLOC==0 
unsigned char W25QXX_BUFFER[4096];		 
#endif
void W25QXX_Write(unsigned char* pBuffer,unsigned int WriteAddr,unsigned short NumByteToWrite)   
{ 
	unsigned int secpos;
	unsigned short secoff;
	unsigned short secremain;	   
 	unsigned short i;    
	unsigned char * W25QXX_BUF;	
#if	W25QXX_USE_MALLOC==1	//��̬�ڴ����
	W25QXX_BUF=mymalloc(SRAMIN,4096);//�����ڴ�
#else
   	W25QXX_BUF=W25QXX_BUFFER; 
#endif     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);	//�����������
			for(i=0;i<secremain;i++)	  	//����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	
#if	W25QXX_USE_MALLOC==1		 
	myfree(SRAMIN,W25QXX_BUF);	//�ͷ��ڴ�
#endif	
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                  //SET WEL 
    W25QXX_Wait_Busy();   
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                            //ʹ������   
    spi_write(W25X_ChipErase);		//����Ƭ��������  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);                            //ȡ��Ƭѡ     	      
	W25QXX_Wait_Busy();   				   	//�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void W25QXX_Erase_Sector(unsigned int Dst_Addr)   
{  
	//����falsh�������,������   	  
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                            //ʹ������   
    spi_write(W25X_SectorErase);  	//������������ָ�� 
    spi_write((unsigned char)((Dst_Addr)>>16));  //����24bit��ַ    
    spi_write((unsigned char)((Dst_Addr)>>8));   
    spi_write((unsigned char)Dst_Addr);  
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);                       		//ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				   	//�ȴ��������
}  
//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  


























