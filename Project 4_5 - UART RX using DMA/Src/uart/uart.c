#include "uart.h"
#include "stm32f407xx.h"
#include "string.h"
#include "gpio.h"

#define UARTCMD

#ifdef UARTCMD
#define CMD_LINE_MAX_LEN 64

static char cmd_line[CMD_LINE_MAX_LEN];
static uint32_t cmd_index = 0;
#endif

static const char *tx_buffer = 0;
static uint32_t tx_index = 0;
volatile char uart_rx_char;
volatile uint8_t uart_rx_ready;

volatile uint8_t dma_tx_done;
volatile uint8_t uart_rx_done;
uint8_t uart_rx_buf[RX_BUF_SIZE];

static UART_RxBuffer_t uart2_rx_buffer = {
    .head = 0,
    .tail = 0
};

void USART2_IRQHandler(void)
{
	if (USART2->SR & USART_SR_RXNE)
	{
	  uint8_t data = (uint8_t)USART2->DR;

	  uint16_t next_head = (uart2_rx_buffer.head + 1) % UART_RX_BUFFER_SIZE;

	  if (next_head != uart2_rx_buffer.tail)
	  {
	      uart2_rx_buffer.buffer[uart2_rx_buffer.head] = data;
	      uart2_rx_buffer.head = next_head;
	  }
	  else
	  {
	    // Buffer full → drop byte (or set overflow flag)
	  }
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

void DMA1_Stream6_IRQHandler(void)
{
    // Check Transfer Complete flag
    if (DMA1->HISR & DMA_HISR_TCIF6)
    {
        // 1. Clear interrupt flag
        DMA1->HIFCR |= DMA_HIFCR_CTCIF6;

        // 2. Disable DMA stream
        DMA1_Stream6->CR &= ~DMA_SxCR_EN;

        // 3. Disable UART DMA TX request
        USART2->CR3 &= ~USART_CR3_DMAT;

        // Optional:
        // Set a flag like dma_tx_done = 1;
        dma_tx_done = 1;
    }
}

void DMA1_Stream5_IRQHandler(void)
{
    if (DMA1->HISR & DMA_HISR_TCIF5)
    {
        // Clear flag
        DMA1->HIFCR |= DMA_HIFCR_CTCIF5;

        uart_rx_done = 1;
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

    USART2->CR1 = 0;
    USART2->BRR = 0x08B;
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;			//Enable the transmit and receiver
    USART2->CR1 |= USART_CR1_UE;						//Unable the USART device

    //Disabled with DMA
    //USART2->CR1 |= USART_CR1_RXNEIE;   					// Enable RX interrupt
    //NVIC_EnableIRQ(USART2_IRQn);						//Enable the Nested Vector Interrupt
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

void UART2_SendBuffer(uint8_t *buf, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        // Wait until TX data register is empty
        while (!(USART2->SR & USART_SR_TXE));

        // Send byte
        USART2->DR = buf[i];
    }

    // Optional: wait for transmission complete
    while (!(USART2->SR & USART_SR_TC));
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

uint8_t UART2_RxAvailable(void)
{
    return (uart2_rx_buffer.head != uart2_rx_buffer.tail);
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

uint8_t UART2_ReadChar(void)
{
    uint8_t data = 0;

    if (uart2_rx_buffer.head != uart2_rx_buffer.tail)
    {
        data = uart2_rx_buffer.buffer[uart2_rx_buffer.tail];
        uart2_rx_buffer.tail =
            (uart2_rx_buffer.tail + 1) % UART_RX_BUFFER_SIZE;
    }

    return data;
}

void UART_ProcessInput(void)
{
    while (UART2_RxAvailable())
    {
        char ch = UART2_ReadChar();

        // ENTER key
        if (ch == '\r' || ch == '\n')
        {
            cmd_line[cmd_index] = '\0';   // terminate string
            cmd_index = 0;

            UART2_SendString("\r\n");     // move to next line
            UART_HandleCommand(cmd_line);
        }
        // BACKSPACE
        else if (ch == '\b' || ch == 0x7F)
        {
            if (cmd_index > 0)
            {
                cmd_index--;
                UART2_SendString("\b \b"); // erase char on terminal
            }
        }
        // NORMAL CHAR
        else
        {
            if (cmd_index < (CMD_LINE_MAX_LEN - 1))
            {
                cmd_line[cmd_index++] = ch;
                UART2_SendChar(ch);  // echo
            }
        }
    }
}

void UART_HandleCommand(char *cmd)
{
    if (strcmp(cmd, "Help") == 0)
    {
        UART2_SendString("Commands:\r\n");
        UART2_SendString("1) Help\r\n");
        UART2_SendString("2) Status\r\n");
        UART2_SendString("3) LED Blue Light ON\r\n");
        UART2_SendString("4) LED Blue Light OFF\r\n");
        UART2_SendString("Input Command : ");
    }
    else if (strcmp(cmd, "Status") == 0)
    {
        UART2_SendString("System OK\r\n");
        UART2_SendString("\r\n");
        UART2_SendString("Commands:\r\n");
        UART2_SendString("1) Help\r\n");
        UART2_SendString("2) Status\r\n");
        UART2_SendString("3) LED Blue Light ON\r\n");
        UART2_SendString("4) LED Blue Light OFF\r\n");
        UART2_SendString("Input Command : ");
    }
    else if (strcmp(cmd, "LED Blue Light ON") == 0)
    {
        UART2_SendString("LED ON\r\n");
        GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);
        UART2_SendString("\r\n");
        UART2_SendString("Commands:\r\n");
        UART2_SendString("1) Help\r\n");
        UART2_SendString("2) Status\r\n");
        UART2_SendString("3) LED Blue Light ON\r\n");
        UART2_SendString("4) LED Blue Light OFF\r\n");
        UART2_SendString("Input Command : ");
    }
    else if (strcmp(cmd, "LED Blue Light OFF") == 0)
    {
        UART2_SendString("LED OFF\r\n");
        GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);
        UART2_SendString("\r\n");
        UART2_SendString("Commands:\r\n");
        UART2_SendString("1) Help\r\n");
        UART2_SendString("2) Status\r\n");
        UART2_SendString("3) LED Blue Light ON\r\n");
        UART2_SendString("4) LED Blue Light OFF\r\n");
        UART2_SendString("Input Command : ");
    }
    else
    {
        UART2_SendString("Unknown command\r\n");
        UART2_SendString("\r\n");
        UART2_SendString("Commands:\r\n");
        UART2_SendString("1) Help\r\n");
        UART2_SendString("2) Status\r\n");
        UART2_SendString("3) LED Blue Light ON\r\n");
        UART2_SendString("4) LED Blue Light OFF\r\n");
        UART2_SendString("Input Command : ");
    }
}

void UART2_DMA_TX_Init(void)
{
    /* Enable DMA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    /* Disable stream before config */
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    while (DMA1_Stream6->CR & DMA_SxCR_EN);

    /* Clear flags */
    DMA1->HIFCR =
        DMA_HIFCR_CTCIF6  |
        DMA_HIFCR_CHTIF6  |
        DMA_HIFCR_CTEIF6  |
        DMA_HIFCR_CDMEIF6 |
        DMA_HIFCR_CFEIF6;

    /* Peripheral address */
    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;

    /* Stream configuration */
    DMA1_Stream6->CR =
        (4U << DMA_SxCR_CHSEL_Pos) |   // Channel 4
        DMA_SxCR_DIR_0             |   // Memory → Peripheral
        DMA_SxCR_MINC              |   // Increment memory
        (0U << DMA_SxCR_PSIZE_Pos) |   // Peripheral size = 8-bit
        (0U << DMA_SxCR_MSIZE_Pos) |   // Memory size = 8-bit
        (2U << DMA_SxCR_PL_Pos);       // High priority

    /* Direct mode */
    DMA1_Stream6->FCR = 0;

    /* Enable USART DMA request */
    USART2->CR3 |= USART_CR3_DMAT;

    //Enable NVIC Interrupt
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void UART2_SendString_DMA(const char *str)
{
    uint16_t len = 0;

    while (str[len] != '\0')
        len++;

    // 1. Disable DMA stream
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    while (DMA1_Stream6->CR & DMA_SxCR_EN);

    // 2. Set memory address
    DMA1_Stream6->M0AR = (uint32_t)str;

    // 3. Set number of bytes
    DMA1_Stream6->NDTR = len;

    // 4. Clear flags again (important!)
    DMA1->HIFCR =
        DMA_HIFCR_CTCIF6  |
        DMA_HIFCR_CHTIF6  |
        DMA_HIFCR_CTEIF6  |
        DMA_HIFCR_CDMEIF6 |
        DMA_HIFCR_CFEIF6;

    // 5. Enable DMA stream
    DMA1_Stream6->CR |= DMA_SxCR_EN;

    // 6. Poll transfer complete
    while (!(DMA1->HISR & DMA_HISR_TCIF6));

    // 7. Clear TC flag
    DMA1->HIFCR = DMA_HIFCR_CTCIF6;
}

void UART2_SendString_DMA_IT(const char *str)
{
    uint16_t len = 0;

    // Calculate string length
    while (str[len] != '\0')
    {
        len++;
    }

    if (len == 0)
        return;

    dma_tx_done = 0;

    // Disable DMA stream
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    while (DMA1_Stream6->CR & DMA_SxCR_EN);

    // Configure DMA
    DMA1_Stream6->M0AR = (uint32_t)str;
    DMA1_Stream6->NDTR = len;

    // Enable UART DMA TX request
    USART2->CR3 |= USART_CR3_DMAT;

    // Enable transfer complete interrupt
    DMA1_Stream6->CR |= DMA_SxCR_TCIE;

    // Enable DMA stream
    DMA1_Stream6->CR |= DMA_SxCR_EN;
}

void UART2_DMA_RX_Init(void)
{
    /* 1. Enable DMA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    /* 2. Disable stream */
    DMA1_Stream5->CR &= ~DMA_SxCR_EN;
    while (DMA1_Stream5->CR & DMA_SxCR_EN);

    /* 3. Clear flags */
    DMA1->HIFCR = DMA_HIFCR_CTCIF5 |
                  DMA_HIFCR_CHTIF5 |
                  DMA_HIFCR_CTEIF5 |
                  DMA_HIFCR_CDMEIF5 |
                  DMA_HIFCR_CFEIF5;

    /* 4. Configure addresses */
    DMA1_Stream5->PAR  = (uint32_t)&USART2->DR;
    DMA1_Stream5->M0AR = (uint32_t)uart_rx_buf;
    DMA1_Stream5->NDTR = RX_BUF_SIZE;

    /* 5. Channel 4 */
    DMA1_Stream5->CR &= ~DMA_SxCR_CHSEL;
    DMA1_Stream5->CR |= (4U << DMA_SxCR_CHSEL_Pos);

    /* 6. Peripheral-to-memory */
    DMA1_Stream5->CR &= ~DMA_SxCR_DIR;

    /* 7. Increment memory */
    DMA1_Stream5->CR |= DMA_SxCR_MINC;

    /* 8. 8-bit data */
    DMA1_Stream5->CR &= ~(DMA_SxCR_MSIZE | DMA_SxCR_PSIZE);

    /* 9. Normal mode */
    DMA1_Stream5->CR &= ~DMA_SxCR_CIRC;

    /* 🔥 10. FORCE DIRECT MODE */
    DMA1_Stream5->FCR &= ~DMA_SxFCR_DMDIS;

    /* 11. Enable TC interrupt */
    DMA1_Stream5->CR |= DMA_SxCR_TCIE;
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    /* 🔥 12. ENABLE UART DMA REQUEST FIRST */
    USART2->CR3 |= USART_CR3_DMAR;

    /* 🔥 13. Enable stream LAST */
    DMA1_Stream5->CR |= DMA_SxCR_EN;
}




