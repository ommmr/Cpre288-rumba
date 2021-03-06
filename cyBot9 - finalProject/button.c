/**
 * @file button.c
 * @brief This initializes buttons for the CYBOT and has various methods
 * in which the buttons can function
 *
 * @author Omar Abbas
 * @author Jack Temple
 *
 * @date 7/3/2018
 */

//The buttons are on PORTE 0:5
#include "button.h"



#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

#define BUTTON_PORT		GPIO_PORTE_DATA_R

uint8_t _prevButton; //must be set yourself in button_getButton()

/**
 * Initialize PORTE and configure bits 5-0 to be used as inputs.
 * @author Omar Abbas
 * @author Jack Temple
 *
 * @param void
 * @data 07/3/2018
 */
void button_init()
{
    static uint8_t initialized = 0;

    //Check if already initialized
    if (initialized)
        return;

    //Turn on PORTE system clock
    SYSCTL_RCGCGPIO_R |= BIT4;

    //Set the buttons to inputs and enable
    GPIO_PORTE_DIR_R &= ~(BIT6 - 1); //Clear bits 5:0
    GPIO_PORTE_DEN_R |= (BIT6 - 1);

    initialized = 1;
}

/**
 * returns a 6-bit field, representing the push buttons. A 1 means the corresponding button is pressed.
 * @author Omar Abbas
 * @author Jack Temple
 *
 * @param void
 * @return 6 bit fields representing which button has been pushed
 * @data 07/03/2018
 */


uint8_t button_checkButtons()
{
    return (~BUTTON_PORT) & (BIT6 - 1); //Return the button status
}


/**
 * Returns the position of the leftmost button being pushed.
 *
 * @author Omar Abbas
 * @author Jack Temple
 *
 * @param void
 * @return the position of the leftmost button being pushed. 6 is the leftmost button, 1 is the rightmost button.  0 indicates no button being pressed
 *
 * @data 07/3/2018
 */

uint8_t button_getButton()
{

#warning "Unimplemented function: uint8_t button_getButton(void)"	// delete warning after implementing


    if ((GPIO_PORTE_DATA_R & 0b00000001) == 0b00000000)
    {

        return 1;
    }

    if ((GPIO_PORTE_DATA_R & 0b00000010) == 0b00000000)
    {

        return 2;
    }

    if ((GPIO_PORTE_DATA_R & 0b00000100) == 0b00000000)
    {

        return 3;
    }

    if ((GPIO_PORTE_DATA_R & 0b00001000) == 0b00000000)
    {

        return 4;
    }

    if ((GPIO_PORTE_DATA_R & 0b00010000) == 0b00000000)
    {

        return 5;
    }

    if ((GPIO_PORTE_DATA_R & 0b00100000) == 0b00000000)
    {

        return 6;
    }

    return 0; // REPLACE ME
}

uint8_t button_getButtonBlocking()
{
    uint8_t button;

    //Wait for button to be pressed
    while ((button = button_getButton()) == 0);

    return button;
}

int8_t button_getButtonChange()
{
    uint8_t lastButton = _prevButton; //_prevButton can be set in button_getButton if you want to use this function.
    int8_t curButton = button_getButton();

    if (curButton != lastButton)
    {
        return curButton;
    }
    else
    {
        return -1;
    }
}

int8_t button_getButtonChangeBlocking()
{
    int8_t button;

    while ((button = button_getButtonChange()) == -1)
        ;

    return button;
}
