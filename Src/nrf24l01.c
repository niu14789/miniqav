/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : notify.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
	* BEEP TIM3 CHANNEL1 PWM Gerente
	* LED is TIM4 CH3 and CH4
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "fs.h"
#include "f_shell.h"
#include "state.h"
#include "main.h"
#include "cmsis_os.h"
#include "f_ops.h"
#include "crc16.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/
static struct file * nrf_fopen (FAR struct file *filp);
static int nrf_ioctrl(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data);
static unsigned int nrf_fread(FAR struct file *filp, FAR void * buffer, unsigned int buflen);
static int nrf_fwrite(FAR struct file *filp, FAR const void * buffer, unsigned int buflen);
/* USER CODE BEGIN Includes */
#include "nrf24l01.h"
/* USER CODE END Includes */
/* USER CODE BEGIN Includes */
FS_INODE_REGISTER("nrf.d",nrf,nrf_heap_init,0);
/* USER SPI HANDLER */
extern SPI_HandleTypeDef hspi1;
/* define */
SPI_HandleTypeDef * rf_spi_handle;
/* default channel and rx tx data */
static unsigned char  tr_addr_g[5] = {INIT_ADDR};
static unsigned char rf_ch_g = 60;
/*--------------------------------file system--------------------------------*/
/* defined functions */
static int nrf_heap_init(void)
{
	/* init as default */
	nrf.flip.f_inode = &nrf;
	nrf.config = nrf_config;
	/* file interface  */
	nrf.ops.read  = nrf_fread;
	nrf.ops.write = nrf_fwrite;
	nrf.ops.open  = nrf_fopen;
	nrf.ops.ioctl = nrf_ioctrl;
	/* spi handle */
	rf_spi_handle = &hspi1;
	/* return mpu9250 result */
	return nrf24L01_Init();
}
/* create a task for receiving the remote */
static int nrf_config(void)
{
	/* create a task run as 1ms per second */
	xTaskCreate(nrf_thread, "nrf_thread", 128 /* stack size */, 0 , 4 /* priority */ , 0 );
	/* return OK */
	return FS_OK;
}

unsigned char rc_data[32];
rcs_HandleTypeDef __rc;
rcs_HandleTypeDef *__rtc;
/* nrf task */
static void nrf_thread(void *p)
{
	/* Ignore compiler warnings */
	(void)p;
	/* init something */	
	unsigned int lasttime;
	unsigned char led_freq_ctrl = 0;
		/* loop */
	while(1)
	{
		 vTaskDelayUntil(&lasttime, 1 /* 10ms */ );
		 /* read nrf data */
		 if( nrf_fread(0,rc_data,sizeof(rc_data)) != 0 )
		 {
			 __rtc = ( rcs_HandleTypeDef * )rc_data;
			 /* check crc */
			 if( fs_crc16_read(__rtc,sizeof(rcs_HandleTypeDef) - 2 ) == __rtc->crc )
			 {
					/* copy data */
					memcpy((void *)&__rc,(const void *)rc_data,sizeof(__rc));				
          /* led_freq_ctrl */		
          if( led_freq_ctrl ++ >= 50 )
					{
						 /* clear */
						 led_freq_ctrl = 0;
						 /* toggle led green */
						 GPIOA->ODR ^= 1<<10;
						 /*------------------*/
					}						
			 }
			 /*-----------*/
		 }
	}
}
/* file & driver 's interface */
static struct file * nrf_fopen (FAR struct file *filp)
{
	/* return flip data */
	return &nrf.flip;
}
/* data read */
static unsigned int nrf_fread(FAR struct file *filp, FAR void * buffer, unsigned int buflen)
{
	/* read */
  return nrf_read(0,buffer,buflen);
}
/* write nrf data */
static int nrf_fwrite(FAR struct file *filp, FAR const void * buffer, unsigned int buflen)
{
	return nrf_write(0,buffer,buflen);
}
/* mpu9250 ioctrl */
static int nrf_ioctrl(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data)
{
	/* nothing diffrent */
	int ret = FS_OK;
	/* select a cmd */
	switch(cmd)
	{
		case 0:
			/* init nrf24l01 */
		  ret = nrf24L01_Init();
		  /* end if */
			break;
		default:
			break;
	}
	/* return */
	return ret;
}
/*--------------------------------low level----------------------------------*/
/* delay */
static void rf_delay_ms(unsigned int t)
{
  //unsigned int tmp = t;
  /* delay */
  //while(tmp--);
}
/* spi write */
unsigned char hal_spi_rw_byte(unsigned char data)
{
	unsigned char d[2];
	/* structet */
	d[0] = data;
	/* send and receive */
	HAL_SPI_TransmitReceive(rf_spi_handle,&d[0],&d[1],1,0xffff);
	/* return data */
	return d[1];
}	

/**
  * @brief :NRF24L01���Ĵ���
  * @param :
           @Addr:�Ĵ�����ַ
  * @note  :��ַ���豸����Ч
  * @retval:��ȡ������
  */
unsigned char NRF24L01_Read_Reg( unsigned char RegAddr )
{
    unsigned char btmp;
	
    RF24L01_SET_CS_LOW( );			//Ƭѡ
	
    hal_spi_rw_byte( NRF_READ_REG | RegAddr );	//������ ��ַ
    btmp = hal_spi_rw_byte( 0xFF );				//������
	
    RF24L01_SET_CS_HIGH( );			//ȡ��Ƭѡ
	
    return btmp;
}

/**
  * @brief :NRF24L01��ָ�����ȵ�����
  * @param :
  *			@reg:��ַ
  *			@pBuf:���ݴ�ŵ�ַ
  *			@len:���ݳ���
  * @note  :���ݳ��Ȳ�����255����ַ���豸����Ч
  * @retval:��ȡ״̬
  */
void NRF24L01_Read_Buf( unsigned char RegAddr, unsigned char *pBuf, unsigned char len )
{
    unsigned char btmp;
	
    RF24L01_SET_CS_LOW( );			//Ƭѡ
	
    hal_spi_rw_byte( NRF_READ_REG | RegAddr );	//������ ��ַ
    for( btmp = 0; btmp < len; btmp ++ )
    {
        *( pBuf + btmp ) = hal_spi_rw_byte( 0xFF );	//������
    }
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01д�Ĵ���
  * @param :��
  * @note  :��ַ���豸����Ч
  * @retval:��д״̬
  */
void NRF24L01_Write_Reg( unsigned char RegAddr, unsigned char Value )
{
    RF24L01_SET_CS_LOW( );		//Ƭѡ
	
    hal_spi_rw_byte( NRF_WRITE_REG | RegAddr );	//д���� ��ַ
    hal_spi_rw_byte( Value );			//д����
	
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01дָ�����ȵ�����
  * @param :
  *			@reg:��ַ
  *			@pBuf:д������ݵ�ַ
  *			@len:���ݳ���
  * @note  :���ݳ��Ȳ�����255����ַ���豸����Ч
  * @retval:д״̬
  */
void NRF24L01_Write_Buf( unsigned char RegAddr, unsigned char *pBuf, unsigned char len )
{
    unsigned char i;
	
    RF24L01_SET_CS_LOW( );		//Ƭѡ
	
    hal_spi_rw_byte( NRF_WRITE_REG | RegAddr );	//д���� ��ַ
    for( i = 0; i < len; i ++ )
    {
        hal_spi_rw_byte( *( pBuf + i ) );		//д����
    }
	
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ

}

/**
  * @brief :���TX������
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Flush_Tx_Fifo ( void )
{
    RF24L01_SET_CS_LOW( );		//Ƭѡ
	
    hal_spi_rw_byte( FLUSH_TX );	//��TX FIFO����
	
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :���RX������
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Flush_Rx_Fifo( void )
{
    RF24L01_SET_CS_LOW( );		//Ƭѡ
	
    hal_spi_rw_byte( FLUSH_RX );	//��RX FIFO����
	
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :����ʹ����һ������
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Reuse_Tx_Payload( void )
{
    RF24L01_SET_CS_LOW( );		//Ƭѡ
	
    hal_spi_rw_byte( REUSE_TX_PL );		//����ʹ����һ������
	
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01�ղ���
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Nop( void )
{
    RF24L01_SET_CS_LOW( );		//Ƭѡ
	
    hal_spi_rw_byte( NOP );		//�ղ�������
	
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01��״̬�Ĵ���
  * @param :��
  * @note  :��
  * @retval:RF24L01״̬
  */
unsigned char NRF24L01_Read_Status_Register( void )
{
    unsigned char Status;
	
    RF24L01_SET_CS_LOW( );		//Ƭѡ
	
    Status = hal_spi_rw_byte( NRF_READ_REG + STATUS );	//��״̬�Ĵ���
	
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
	
    return Status;
}

/**
  * @brief :NRF24L01���ж�
  * @param :
           @IRQ_Source:�ж�Դ
  * @note  :��
  * @retval:�����״̬�Ĵ�����ֵ
  */
unsigned char NRF24L01_Clear_IRQ_Flag( unsigned char IRQ_Source )
{
    unsigned char btmp = 0;

    IRQ_Source &= ( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT );	//�жϱ�־����
    btmp = NRF24L01_Read_Status_Register( );			//��״̬�Ĵ���
			
    RF24L01_SET_CS_LOW( );			//Ƭѡ
    hal_spi_rw_byte( NRF_WRITE_REG + STATUS );	//д״̬�Ĵ�������
    hal_spi_rw_byte( IRQ_Source | btmp );		//����Ӧ�жϱ�־
    RF24L01_SET_CS_HIGH( );			//ȡ��Ƭѡ
	
    return ( NRF24L01_Read_Status_Register( ));			//����״̬�Ĵ���״̬
}

/**
  * @brief :��RF24L01�ж�״̬
  * @param :��
  * @note  :��
  * @retval:�ж�״̬
  */
unsigned char RF24L01_Read_IRQ_Status( void )
{
    return ( NRF24L01_Read_Status_Register( ) & (( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT )));	//�����ж�״̬
}
 
 /**
  * @brief :��FIFO�����ݿ��
  * @param :��
  * @note  :��
  * @retval:���ݿ��
  */
unsigned char NRF24L01_Read_Top_Fifo_Width( void )
{
    unsigned char btmp;
	
    RF24L01_SET_CS_LOW( );		//Ƭѡ
	
    hal_spi_rw_byte( R_RX_PL_WID );	//��FIFO�����ݿ������
    btmp = hal_spi_rw_byte( 0xFF );	//������
	
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
	
    return btmp;
}

 /**
  * @brief :�����յ�������
  * @param :��
  * @note  :��
  * @retval:
           @pRxBuf:���ݴ�ŵ�ַ�׵�ַ
  */
unsigned char NRF24L01_Read_Rx_Payload( unsigned char *pRxBuf )
{
    unsigned char Width, PipeNum;
	
    PipeNum = ( NRF24L01_Read_Reg( STATUS ) >> 1 ) & 0x07;	//������״̬
    Width = NRF24L01_Read_Top_Fifo_Width( );		//���������ݸ���

    RF24L01_SET_CS_LOW( );		//Ƭѡ
    hal_spi_rw_byte( RD_RX_PLOAD );			//����Ч��������
	
    for( PipeNum = 0; PipeNum < Width; PipeNum ++ )
    {
        *( pRxBuf + PipeNum ) = hal_spi_rw_byte( 0xFF );		//������
    }
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
    NRF24L01_Flush_Rx_Fifo( );	//���RX FIFO
	
    return Width;
}

 /**
  * @brief :�������ݣ���Ӧ��
  * @param :
  *			@pTxBuf:�������ݵ�ַ
  *			@len:����
  * @note  :һ�β�����32���ֽ�
  * @retval:��
  */
void NRF24L01_Write_Tx_Payload_Ack( unsigned char *pTxBuf, unsigned char len )
{
    unsigned char btmp;
    unsigned char length = ( len > 32 ) ? 32 : len;		//���ݳ����Լ32 ��ֻ����32��

    NRF24L01_Flush_Tx_Fifo( );		//��TX FIFO
	
    RF24L01_SET_CS_LOW( );			//Ƭѡ
    hal_spi_rw_byte( WR_TX_PLOAD );	//��������
	
    for( btmp = 0; btmp < length; btmp ++ )
    {
        hal_spi_rw_byte( *( pTxBuf + btmp ) );	//��������
    }
    RF24L01_SET_CS_HIGH( );			//ȡ��Ƭѡ
}

 /**
  * @brief :�������ݣ�����Ӧ��
  * @param :
  *			@pTxBuf:�������ݵ�ַ
  *			@len:����
  * @note  :һ�β�����32���ֽ�
  * @retval:��
  */
void NRF24L01_Write_Tx_Payload_NoAck( unsigned char *pTxBuf, unsigned char len )
{
    if( len > 32 || len == 0 )
    {
        return ;		//���ݳ��ȴ���32 ���ߵ���0 ��ִ��
    }
	
    RF24L01_SET_CS_LOW( );	//Ƭѡ
    hal_spi_rw_byte( WR_TX_PLOAD_NACK );	//��������
    while( len-- )
    {
        hal_spi_rw_byte( *pTxBuf );			//��������
		pTxBuf++;
    }
    RF24L01_SET_CS_HIGH( );		//ȡ��Ƭѡ
}

 /**
  * @brief :�ڽ���ģʽ����TX FIFOд����(��ACK)
  * @param :
  *			@pData:���ݵ�ַ
  *			@len:����
  * @note  :һ�β�����32���ֽ�
  * @retval:��
  */
void NRF24L01_Write_Tx_Payload_InAck( unsigned char *pData, unsigned char len )
{
    unsigned char btmp;
	
	len = ( len > 32 ) ? 32 : len;		//���ݳ��ȴ���32����ֻд32���ֽ�

    RF24L01_SET_CS_LOW( );			//Ƭѡ
    hal_spi_rw_byte( W_ACK_PLOAD );		//����
    for( btmp = 0; btmp < len; btmp ++ )
    {
        hal_spi_rw_byte( *( pData + btmp ) );	//д����
    }
    RF24L01_SET_CS_HIGH( );			//ȡ��Ƭѡ
}

 /**
  * @brief :���÷��͵�ַ
  * @param :
  *			@pAddr:��ַ��ŵ�ַ
  *			@len:����
  * @note  :��
  * @retval:��
  */
void NRF24L01_Set_TxAddr( unsigned char *pAddr, unsigned char len )
{
	len = ( len > 5 ) ? 5 : len;					//��ַ���ܴ���5���ֽ�
    NRF24L01_Write_Buf( TX_ADDR, pAddr, len );	//д��ַ
}

 /**
  * @brief :���ý���ͨ����ַ
  * @param :
  *			@PipeNum:ͨ��
  *			@pAddr:��ַ����ŵ�ַ
  *			@Len:����
  * @note  :ͨ��������5 ��ַ���Ȳ�����5���ֽ�
  * @retval:��
  */
void NRF24L01_Set_RxAddr( unsigned char PipeNum, unsigned char *pAddr, unsigned char Len )
{
    Len = ( Len > 5 ) ? 5 : Len;
    PipeNum = ( PipeNum > 5 ) ? 5 : PipeNum;		//ͨ��������5 ��ַ���Ȳ�����5���ֽ�

    NRF24L01_Write_Buf( RX_ADDR_P0 + PipeNum, pAddr, Len );	//д���ַ
}

 /**
  * @brief :����ͨ���ٶ�
  * @param :
  *			@Speed:�ٶ�
  * @note  :��
  * @retval:��
  */
void NRF24L01_Set_Speed( nRf24l01SpeedType Speed )
{
	unsigned char btmp = 0;
	
	btmp = NRF24L01_Read_Reg( RF_SETUP );
	btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	
	if( Speed == SPEED_250K )		//250K
	{
		btmp |= ( 1<<5 );
	}
	else if( Speed == SPEED_1M )   //1M
	{
   		btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	}
	else if( Speed == SPEED_2M )   //2M
	{
		btmp |= ( 1<<3 );
	}

	NRF24L01_Write_Reg( RF_SETUP, btmp );
}

 /**
  * @brief :���ù���
  * @param :
  *			@Speed:�ٶ�
  * @note  :��
  * @retval:��
  */
void NRF24L01_Set_Power( nRf24l01PowerType Power )
{
    unsigned char btmp;
	
	btmp = NRF24L01_Read_Reg( RF_SETUP ) & ~0x07;
    switch( Power )
    {
        case POWER_F18DBM:
            btmp |= PWR_18DB;
            break;
        case POWER_F12DBM:
            btmp |= PWR_12DB;
            break;
        case POWER_F6DBM:
            btmp |= PWR_6DB;
            break;
        case POWER_0DBM:
            btmp |= PWR_0DB;
            break;
        default:
            break;
    }
    NRF24L01_Write_Reg( RF_SETUP, btmp );
}

 /**
  * @brief :����Ƶ��
  * @param :
  *			@FreqPoint:Ƶ�����ò���
  * @note  :ֵ������127
  * @retval:��
  */
void RF24LL01_Write_Hopping_Point( unsigned char FreqPoint )
{
    NRF24L01_Write_Reg(  RF_CH, FreqPoint & 0x7F );
}

/**
  * @brief :NRF24L01���
  * @param :��
  * @note  :��
  * @retval:��
  */ 
int NRF24L01_check( void )
{
	unsigned char i;
	unsigned char buf[5]={ 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	unsigned char read_buf[ 5 ] = { 0 };
	unsigned char try_time = 0;
	while( 1 )
	{
		NRF24L01_Write_Buf( TX_ADDR, buf, 5 );			//д��5���ֽڵĵ�ַ
		NRF24L01_Read_Buf( TX_ADDR, read_buf, 5 );		//����д��ĵ�ַ  
		for( i = 0; i < 5; i++ )
		{
			if( buf[ i ] != read_buf[ i ] )
			{
				break;
			}	
		} 
		
		if( 5 == i )
		{
			return 0;
		}
		else
		{
		  try_time ++;
		}
		/* timeout */
		if( try_time > 5 )
		{
			return (-1);
		}
		/*----------*/
		rf_delay_ms( 2000 );
	}
}

 /**
  * @brief :����ģʽ
  * @param :
  *			@Mode:ģʽ����ģʽ�����ģʽ
  * @note  :��
  * @retval:��
  */
void RF24L01_Set_Mode( nRf24l01ModeType Mode )
{
    unsigned char controlreg = 0;
	controlreg = NRF24L01_Read_Reg( CONFIG );
	
    if( Mode == MODE_TX )       
	{
		controlreg &= ~( 1<< PRIM_RX );
	}
    else 
	{
		if( Mode == MODE_RX )  
		{ 
			controlreg |= ( 1<< PRIM_RX ); 
		}
	}

    NRF24L01_Write_Reg( CONFIG, controlreg );
}

/**
  * @brief :NRF24L01����һ������
  * @param :
  *			@txbuf:�����������׵�ַ
  *			@Length:�������ݳ���
  * @note  :��
  * @retval:
  *			MAX_TX���ﵽ����ط�����
  *			TX_OK���������
  *			0xFF:����ԭ��
  */  
unsigned char NRF24L01_TxPacket( unsigned char *txbuf, unsigned char Length ,unsigned char * addr)
{
	unsigned char l_Status = 0;
	uint16_t l_MsTimes = 0;
	
	RF24L01_SET_CS_LOW( );		//Ƭѡ
	hal_spi_rw_byte( FLUSH_TX );
	RF24L01_SET_CS_HIGH( );
	
	RF24L01_SET_CE_LOW( );		
	NRF24L01_Write_Buf( WR_TX_PLOAD, txbuf, Length );	//д���ݵ�TX BUF 32�ֽ�  TX_PLOAD_WIDTH
	RF24L01_SET_CE_HIGH( );			//��������
	while( 0 != RF24L01_GET_IRQ_STATUS( ))
	{
		rf_delay_ms( 1 );
		if( 500 == l_MsTimes++ )						//500ms��û�з��ͳɹ������³�ʼ���豸
		{
      RF24L01_Init(addr,rf_ch_g);
			RF24L01_Set_Mode( MODE_TX );
			break;
		}
	}
	l_Status = NRF24L01_Read_Reg(STATUS);						//��״̬�Ĵ���
	NRF24L01_Write_Reg( STATUS, l_Status );						//���TX_DS��MAX_RT�жϱ�־
	
	if( l_Status & MAX_TX )	//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg( FLUSH_TX,0xff );	//���TX FIFO�Ĵ���
		return MAX_TX; 
	}
	if( l_Status & TX_OK )	//�������
	{
		return TX_OK;
	}
	
	return 0xFF;	//����ԭ����ʧ��
}

/**
  * @brief :NRF24L01��������
  * @param :
  *			@rxbuf:�������ݴ�ŵ�ַ
  * @note  :��
  * @retval:���յ������ݸ���
  */ 
unsigned char NRF24L01_RxPacket( unsigned char *rxbuf ,unsigned char *addr)
{
	unsigned char l_Status = 0, l_RxLength = 0;//, l_100MsTimes = 0;
	
	RF24L01_SET_CS_LOW( );		//Ƭѡ
	hal_spi_rw_byte( FLUSH_RX );
	RF24L01_SET_CS_HIGH( );
	
	while(0 != RF24L01_GET_IRQ_STATUS( ))
	{
    
	}
	
	l_Status = NRF24L01_Read_Reg( STATUS );		//��״̬�Ĵ���
	NRF24L01_Write_Reg( STATUS,l_Status );		//���жϱ�־
	if( l_Status & RX_OK)	//���յ�����
	{
		l_RxLength = NRF24L01_Read_Reg( R_RX_PL_WID );		//��ȡ���յ������ݸ���
		NRF24L01_Read_Buf( RD_RX_PLOAD,rxbuf,l_RxLength );	//���յ����� 
		NRF24L01_Write_Reg( FLUSH_RX,0xff );				//���RX FIFO
		return l_RxLength; 
	}	
	
	return 0;				//û���յ�����	
}

 /**
  * @brief :RF24L01ģ���ʼ��
  * @param :��
  * @note  :��
  * @retval:��
  */
void RF24L01_Init(unsigned char * addr ,unsigned char rf_ch)
{
    /* addr */
    RF24L01_SET_CE_HIGH( );
    NRF24L01_Clear_IRQ_Flag( IRQ_ALL );
#if DYNAMIC_PACKET == 1

    NRF24L01_Write_Reg( DYNPD, ( 1 << 0 ) ); 	//��1?�������̨�1?����?��y?Y3��?��
    NRF24L01_Write_Reg( FEATRUE, 0x07 );
    NRF24L01_Read_Reg( DYNPD );
    NRF24L01_Read_Reg( FEATRUE );
	
#elif DYNAMIC_PACKET == 0
    
    L01_WriteSingleReg( L01REG_RX_PW_P0, FIXED_PACKET_LEN );	//1��?����y?Y3��?��
	
#endif	//DYNAMIC_PACKET

    NRF24L01_Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/		
                                      ( 1 << EN_CRC ) |     
                                      ( 1 << PWR_UP ) );   
    NRF24L01_Write_Reg( EN_AA, ( 1 << ENAA_P0 ) );   		
    NRF24L01_Write_Reg( EN_RXADDR, ( 1 << ERX_P0 ) );		
    NRF24L01_Write_Reg( SETUP_AW, AW_5BYTES );     		
    NRF24L01_Write_Reg( SETUP_RETR, ARD_4000US |
                        ( REPEAT_CNT & 0x0F ) ); 
    NRF24L01_Write_Reg( RF_CH, rf_ch ); 
    NRF24L01_Write_Reg( RF_SETUP, 0x26 );

    NRF24L01_Set_TxAddr( addr, 5 );
    NRF24L01_Set_RxAddr( 0, addr, 5 ); 
		
    NRF24L01_Set_Speed(SPEED_1M);
}
/* set package */
int nrf_write(unsigned int addr,const void * data , unsigned int len)
{
	/* set TX mode */
  RF24L01_Set_Mode( MODE_TX );
	/* send data */
  NRF24L01_TxPacket( (unsigned char *)data , len ,tr_addr_g );
  /* return */
	return 0;
}
/* nrf read */
int nrf_read(unsigned int addr,void * data , unsigned int len)
{
	RF24L01_Set_Mode( MODE_RX );
	/* read */
	unsigned char read_len = NRF24L01_RxPacket( (unsigned char *)data ,tr_addr_g);
	/* return */
	return read_len;
}
#if 0
/* delay for a while , just for notify */
static void delay_ms_rf(unsigned int ms)
{
	unsigned int tick;
	/* get tick */
	tick = HAL_GetTick();
	/* wait */
	while( !((HAL_GetTick() - tick) > ms) );
}
#endif
#if 0
/* process */
void key_process(unsigned int pm1,unsigned int pm2,unsigned int pm3,unsigned int pm4,unsigned int pm5)
{
		static unsigned int power_cnt = 0;
	  static unsigned int entrance_cnt = 0;
		static unsigned char release_flag = 0;
    dev_HandleTypeDef * fl = (dev_HandleTypeDef *)pm1;	
		/* check the key */
#if RECEIVER_BOARD	
		if( HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1) == 0 )
#else
		if( HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1) == 1 )
#endif		
		{
			 if( release_flag )
			 {
					power_cnt ++ ;
					/* power off */
					if( power_cnt > 1000 )
					{
#if !RECEIVER_BOARD
						HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET);	
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
						/* infinite loop */
						while(1);
#endif
					}
			 }
			 else
			 {
				  if( ++entrance_cnt > 5000 )
					{
						 /* erase the flash zone */
					   fl->ioctrl(ERASE_FLASH,FEBASE_ADDR,0,0);
						 /* close the led and waiting reset */
#if RECEIVER_BOARD
						 /* close LEDs */
						 GPIOB->ODR |= 1 << 1;		
						 GPIOF->ODR |= 1 << 0;			
             /* waitting for release the key */
             while(!(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1) == 1));		
						 /* and reset system */
						 HAL_NVIC_SystemReset();						
#else
						 /* and reset system */
						 HAL_NVIC_SystemReset();
#endif
						 /* end of files */
				  }
			 }
		}
		else
		{
			/* clear */
			power_cnt = 0;
			/* enable the reciver */
			release_flag = 1;
		}
}
#endif
#if 0
/* nrf 24l01 ioctrl */
int nrf_ioctrl(unsigned int cmd,unsigned int param,void * data,unsigned len)
{
	int ret = 0;
	rcs_HandleTypeDef * rcs;
	unsigned char * p;
	unsigned short tmp;
	/* channel 6 */
	unsigned char trcs;
	/* get parse */
	switch(cmd)
	{
		case CREATE_DSM:
			/* rcs_HandleTypeDef */
		  rcs = (rcs_HandleTypeDef *)data;
		  p   = (unsigned char *)param;
		  /* transfer */
		  if( len != 16 )
			{
				return (-1); // cannot supply this format , dsm 
			}
		  tmp = rcs->channel[3] + 12;
		  tmp = (tmp&0x3ff)|(0x0<<10);  //channel 1 byte 0 
		  p[14] = tmp>>8;
		  p[15] = tmp&0xff;
		 
		  tmp =  rcs->channel[0] + 12;//yaw
		  tmp = (tmp&0x3ff)|(0x3<<10);  //channel 4 byte 3 
		  p[10] = tmp>>8;
		  p[11] = tmp&0xff;
		 
		  tmp =  rcs->channel[2] + 12;//roll
		  tmp = (tmp&0x3ff)|(0x1<<10);  //channel 2 byte 1 
		  p[2] = tmp>>8;
		  p[3] = tmp&0xff;
		 
		  tmp =  rcs->channel[1] + 12;//pitch
		  tmp = (tmp&0x3ff)|(0x2<<10);  //channel 3 byte 2 
		  p[6] = tmp>>8;
		  p[7] = tmp&0xff;
			/* transfer */
			trcs = rcs->channel567 >> 14;	
		  tmp = ( trcs == 0x01)?171:((trcs==0x3)?512:853);
		  tmp = (tmp&0x3ff)|(0x5<<10);  //channel 6 byte 5 
		  p[4] = tmp>>8;
		  p[5] = tmp&0xff;
			
		  tmp = (rcs->channel567 & (1<<13))?171:853;
		  tmp = (tmp&0x3ff)|(0x4<<10);  //channel 6 byte 5 
		  p[8] = tmp>>8;
		  p[9] = tmp&0xff;		

		  tmp = (rcs->channel567 & (1<<12))?853:171;
		  tmp = (tmp&0x3ff)|(0x6<<10);  //channel 6 byte 5 
		  p[12] = tmp>>8;
		  p[13] = tmp&0xff;	
			/* head */
			p[0] = 0x00;
			p[1] = 0x02;
		  /* end if function */
			break;
		default:
			break;
	}
	/* return ret */
	return ret;
}
#endif
/* dev init */
static int nrf24L01_Init( void )
{
	  /* transfer interface */
	  if( rf_spi_handle == 0 )
		{
			return FS_ERR;//please init spi handle first
		}
		/*----- nrf24l01 check ---*/
		if( NRF24L01_check() != 0 )
		{
			return FS_ERR;/* check failed */
		}
#if 0
		/* get and check pm */
		unsigned char unique_d[4];
		/* check it out */
		if( unique_d[2] == 0xAA && unique_d[0] != 0 && unique_d[1] != 0 &&
			(unsigned char)(unique_d[0]+unique_d[1]) == unique_d[3] )
		{
			/* got correct unique ID */
			tr_addr_g[0] = unique_d[0];
			tr_addr_g[1] = unique_d[1];
			/* set flag */
			dev->i_flag = 1;
			dev->i_pri  = unique_d[0] << 8 | unique_d[1]; 
			/* set channel */
			rf_ch_g = ( unique_d[0] + unique_d[1] ) & 0x7f;
		}
#endif		
		/* init rf */
		RF24L01_Init(tr_addr_g,rf_ch_g);
		/* ok */
		return 0;
}







