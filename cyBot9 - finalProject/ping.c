/**
 * @file ping.c
 * @brief this controls the sonar sensor on the CYBot, it is configured for CyBot9
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */

#include <timer.h>
#include <lcd.h>
#include <stdio.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

unsigned long time_diff;
volatile unsigned current_time = 0, time_high = 0, last_time = 0;
volatile float distance = 0;
int flag = 0, overflow = 0;

void TIMER3B_Handler(); // Initializes the timer handler.
/**
 * Initialzes PortB and timer3 and connnects it to the sonar
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
void ping_init()
{

    SYSCTL_RCGCGPIO_R |= 0b00000010; // init portb clock
    SYSCTL_RCGCTIMER_R |= 0b00001000; //init timer 3 clock

    GPIO_PORTB_AFSEL_R |= 0b00001000; //pin3b uses alternate function
    GPIO_PORTB_DEN_R |= 0b00001000; //pin3b to output
    GPIO_PORTB_PCTL_R |= 0x7000; //gives nibble index 3 t3ccp

    TIMER3_CTL_R &= 0xFFFFFEFF; //disable bit
    TIMER3_CTL_R |= 0xC00; //detect rising and falling edge of clock
    TIMER3_CFG_R |= 0X4; //edge timer mode
    TIMER3_TBMR_R |= 0x17; //capture mode
    TIMER3_TBILR_R |= 0xFFFF; //read value and load counter
    TIMER3_ICR_R |= 0x400; //clear ris and mis bits
    TIMER3_IMR_R |= 0x400; //enable interrupts for tiemr 3

    NVIC_EN1_R = 0x10;

    TIMER3_CTL_R |= 0x100; // enables timer3B

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();
}

/**
 * Sends a signal and receives  a sonar pulse.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
void pulse(void)
{
//    TIMER3_CTL_R &= ~(0b100000000);
    GPIO_PORTB_AFSEL_R &= 0xFFFFFFF7; //disconnect pin 3 from alternate function

    GPIO_PORTB_DIR_R |= 0b00001000; //set pin3b to output
    GPIO_PORTB_DATA_R |= 0b00001000; //send high volt f rom pin3b

    timer_waitMicros(10); //wait 10 microseconds

    GPIO_PORTB_DATA_R &= 0b11110111; //clear output of pin3b
    GPIO_PORTB_DIR_R &= 0b11110111; //set it back to input

    GPIO_PORTB_AFSEL_R |= 0b00001000; //bring back alternate function
    GPIO_PORTB_PCTL_R |= 0x7000; //bring back t3cc
//    TIMER3_CTL_R |= (0b100000000);
    flag = 0; //james code
}


/**
 * Timer3 Handler for the sonar data.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
void TIMER3B_Handler(void)
{

    //JAMES CODE
    int trigger_time = TIMER3_TBR_R; //this is time last edge occured since we are in edge-time mode.

    //in this case, flag indicates a time_high start
    TIMER3_ICR_R |= 0x400;

    if (flag == 0)
    { //signal has not been received while timer is ticking

        last_time = trigger_time;
        flag = 1; //set flag to 1 indicating that signal has been sent out, starting a time_high
    }
    else if  (flag == 1)
    {
        flag = 2;
        current_time = trigger_time;
        if (current_time < last_time) //incase future time is less than past time which
        //is impossible unless fault in the system. will get negative number for time_high
        //and that is overflow, code taken from slides
        {
            overflow = 1;
            time_high = ((unsigned long) overflow << 24)
                    + (current_time - last_time);
        }
        time_high = current_time - last_time; //set time_high as future time - laast time
    }

    distance = (time_high * 0.0010625);
}


/**
 * Getter function to retrieve the distance in the main class.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
float get_distance()
{

    while (flag != 2); //james code
return distance;
}
