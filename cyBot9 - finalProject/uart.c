/**
 * @file Uart.c
 * @brief handles everything that deals with the UART and sending data.
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */

#include "uart.h"
#include "button.h"
#include <string.h>


/**
 * Init's the UART, using PortB and UART1
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
void uart_init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; //enable clock to GPIO, R1 = port B
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1; //ENABLE CLOCK TO uart1, r1 = uart1.
    GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1); //enable alternate functions on port b pins 0 and 1
    GPIO_PORTB_PCTL_R |= 0x00000011; //enable Rx and Tx on port B on pins 0 and 1
    GPIO_PORTB_DEN_R |= (BIT0 | BIT1); //set pin 0 and 1 to digital
    GPIO_PORTB_DIR_R &= ~BIT0; //set pin 0 to Rx or input
    GPIO_PORTB_DIR_R |= BIT1; //set pin 1 to Tx or output

    //Calculate BaudRate for 9600
//    uint16_t iBRD = 104;
//    uint16_t fBRD = 11;

    //for 115200
    uint16_t iBRD = 8;
    uint16_t fBRD = 44;

    UART1_CTL_R &= ~(UART_CTL_UARTEN); //turns off UART1

    UART1_IBRD_R = iBRD;
    UART1_FBRD_R = fBRD;

    UART1_LCRH_R = UART_LCRH_WLEN_8; //sets the data bit, set frame
    UART1_CC_R = UART_CC_CS_SYSCLK; //Sets system clock as source.
    UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN); //re-enable enable RX, TX and UART1
}


/**
 * Init's the UART, using PortB and UART1
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @param data the data that is being sent to the UART, usually 1 char.
 * @date 7/3/2018
 */
void uart_sendChar(char data)
{

    while (UART1_FR_R & 0x20) //wait until there is room to send data
    {
    }

    UART1_DR_R = data; // send data

}

/**
 * Reads the data that is being received from the UART.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
int uart_receive(void)
{
    char data = 0;


    while (UART1_FR_R & UART_FR_RXFE)
    {

    }

    data = (char) (UART1_DR_R & 0xFF);

    return data;
}


/**
 * Sends the whole string to the UART
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @param data grabs the pointer for the data that is being sent to the UART
 * @date 7/3/2018
 */
void uart_sendStr(const char *data)
{
    int j= 0;
    while(j<strlen(data)){
        uart_sendChar(data[j]);
        j++;
    }
}
