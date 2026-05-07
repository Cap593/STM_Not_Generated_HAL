#include "uart.h"
#include "stm32f407xx.h"

static const char *tx_buffer = 0;
static uint32_t tx_index = 0;
volatile char uart_rx_char;
volatile uint8_t uart_rx_ready;

void USART2_IRQHandler(void)
{
	if (USART2->SR & USART_SR_RXNE)
	{
		uart_rx_char = (char)(USART2->DR & 0xFF);		// Reading clears RX interrupt
		uart_rx_ready = 1;
	}

    if (USART2->SR & USART_SR_TXE)
    {
        if (tx_buffer[tx_index] != '\0')
        {
            USART2->DR = tx_buffer[tx_index++];
        }
        else
        {
            // Transmission complete
            USART2->CR1 &= ~USART_CR1_TXEIE; // Disable TX interrupt
            tx_buffer = 0;
        }
    }
}

void UART2_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* PA2, PA3 AF */
    GPIOA->MODER &= ~((3 << (2*2)) | (3 << (3*2)));
    GPIOA->MODER |=  ((2 << (2*2)) | (2 << (3*2)));

    GPIOA->AFR[0] |= (7 << (2*4)) | (7 << (3*4));

    USART2->CR1 = 0;	 								//Resetting the UART control register
    USART2->BRR = 0x08B;
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;			//Enable the transmit and receiver
    USART2->CR1 |= USART_CR1_UE;						//Unable the USART device

    USART2->CR1 |= USART_CR1_RXNEIE;   					// Enable RX interrupt
    NVIC_EnableIRQ(USART2_IRQn);						//Enable the Nested Vector Interrupt
}


void UART2_SendChar(char ch)
{
    // Wait until transmit buffer is empty
    while (!(USART2->SR & (1U << 7)))
    {
        // wait for TXE = 1
    }

    // Write data to DR
    USART2->DR = (uint8_t)ch;
}

void UART2_SendString(const char *str)
{
    while (*str)
    {
        UART2_SendChar(*str++);
    }
}

void UART2_SendString_IT(const char *str)
{
    tx_buffer = str;
    tx_index = 0;

    // Enable TXE interrupt
    USART2->CR1 |= USART_CR1_TXEIE;
}

char UART2_ReceiveChar(void)
{
    // Wait until data is received
    while (!(USART2->SR & USART_SR_RXNE))
    {
        // Blocking wait
    }

    // Read received character
    return (char)(USART2->DR & 0xFF);
}

void UART2_ReceiveString(char *buffer, uint32_t max_len)
{
    uint32_t i = 0;
    char ch;

    while (i < (max_len - 1))
    {
        ch = UART2_ReceiveChar();

        if (ch == '\r' || ch == '\n')
            break;

        buffer[i++] = ch;
    }

    buffer[i] = '\0';
}


