
#include "fs.h"
#include "uartd.h"
#include "fs_usb.h"

void USART0_Handler(void)
{
	uint32_t status = USART0->US_CSR;
	if (status & US_CSR_TIMEOUT) 
	{
		if(isr[USART0_IRQn] != NULL)
		{
		   isr[USART0_IRQn](USART0_IRQn,NULL,0);
		}
	}
}
void USART1_Handler(void)
{
	uint32_t status = USART1->US_CSR;
	if (status & US_CSR_TIMEOUT) 
	{
		if(isr[USART1_IRQn] != NULL)
		{
		   isr[USART1_IRQn](USART1_IRQn,NULL,0);
		}
	}
}
void USART2_Handler(void)
{
	uint32_t status = USART2->US_CSR;
	if(status & US_CSR_TIMEOUT) 
	{
		if(isr[USART2_IRQn] != NULL)
		{
		   isr[USART2_IRQn](USART2_IRQn,NULL,0);
		}
	}
}

void UART0_Handler(void)
{
	uint32_t status = UART0->UART_SR;
	if (status & UART_SR_RXRDY) 
	{
		if(isr[UART0_IRQn] != NULL)
		{
		   isr[UART0_IRQn](UART0_IRQn,NULL,0);
		}
	}
}
void UART1_Handler(void)
{
	uint32_t status = UART1->UART_SR;
	if (status & UART_SR_RXRDY) 
	{
		if(isr[UART1_IRQn] != NULL)
		{
		   isr[UART1_IRQn](UART1_IRQn,NULL,0);
		}
	}
}
void UART2_Handler(void)
{
	uint32_t status = UART2->UART_SR;
	if(status & UART_SR_RXRDY) 
	{
		if(isr[UART2_IRQn] != NULL)
		{
		   isr[UART2_IRQn](UART2_IRQn,NULL,0);
		}
	}
}
void UART3_Handler(void)
{
	uint32_t status = UART3->UART_SR;
	if (status & UART_SR_RXRDY) 
	{
		if(isr[UART3_IRQn] != NULL)
		{
		   isr[UART3_IRQn](UART3_IRQn,NULL,0);
		}
	}
}
void UART4_Handler(void)
{
	uint32_t status = UART4->UART_SR;
	if(status & UART_SR_RXRDY) 
	{
		if(isr[UART4_IRQn] != NULL)
		{
		   isr[UART4_IRQn](UART4_IRQn,NULL,0);
		}
	}
}
//void SPI0_Handler(void)
//{
//		if(isr[SPI0_IRQn] != NULL)
//		{
//		   isr[SPI0_IRQn](SPI0_IRQn,NULL,0);
//		}
//}
/* for dma */
void XDMAC_Handler(void)
{
	  xdmac_handler();//usb mode
//	  NVIC_DisableIRQ(XDMAC_IRQn);
//	  if(isr[XDMAC_IRQn] != NULL)
//		{
//		   isr[XDMAC_IRQn](XDMAC_IRQn,NULL,0);
//		}
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{

//}
/*------------------*/




