/**
 * @file Servo.c
 * @brief inits, the servo and configures it to Cybot9, 6 or 14.
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
#include <stdint.h>

#include <inc/tm4c123gh6pm.h>
#include "servo.h"
#include "timer.h"
volatile int total = 0;
volatile int direction = 0;
/*
 * Calculate these values by multiplying the voltage.
 * 1 degree for CyBot9 == .01 Voltage
 */
/*
 * CyBot9
 * 0: .4 (*16000)
 * 90: 1.3
 * 180: 2.2
 * 1 degree = .01
 */

/*
 * Cybot6
 * 0: .45
 * 90: 1.35
 * 180: 2.25
 */
/*
 * CyBot14
 * 0:
 * 90:
 * 180:
 */
unsigned pulse_period = 0x4E200; // pulse period in cycles

void move_servo180(void);


/**
 * Converts the degrees into a PWM that will be able to read by the servo.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @Josh Hall
 * @Yue Chen
 *
 * @param degrees: number of degrees the servo has moved.
 *
 * @date 7/3/2018
 */
unsigned convert(int degrees)
{
    int number = (16000 * ((degrees * .01) + .4)); //cybot9
    return number;
}


/**
 * The timer1 init which sets PortB and Timer1
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @Josh Hall
 * @Yue Chen
 *
 * @param degrees: number of degrees the servo has moved.
 *
 * @date 7/3/2018
 */
void timer1_init()
{
//***set GPIO PB5, turn on clk, alt. function, output, enable***
    SYSCTL_RCGCGPIO_R |= 0b000010;

    GPIO_PORTB_DIR_R |= 0b00100000;
    GPIO_PORTB_DEN_R |= 0b00100000;
    GPIO_PORTB_AFSEL_R |= 0b00100000;
    GPIO_PORTB_PCTL_R |= 0x00700000;

    //turn on clk for timer1
    SYSCTL_RCGCTIMER_R |= 0b00000010;
    //disable timer to config
    TIMER1_CTL_R &= ~0x00000100;

    TIMER1_TBMR_R = TIMER_TBMR_TBMR_PERIOD | TIMER_TBMR_TBAMS;
    //periodic and PWM enable
    TIMER1_CFG_R |= 0b00000100;
    //set size of timer to 16
    TIMER1_TBILR_R = pulse_period & 0xFFFF; //lower 16 bits of the interval
    TIMER1_TBPR_R = pulse_period >> 16; //set the upper 8 bits of the interval
    move_servo180();
//    GPTMTAMATCHR0 = 0X4E200 - mid_width; // if you want to move servo to the
//    middle TIMER0_CTL_R = _____
            //enable timer
    TIMER1_CTL_R |= 0x00000100;

}

/**
 * Moves the servo to the 180 degrees position.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @Josh Hall
 * @Yue Chen
 *
 * @date 7/3/2018
 */
void move_servo180(void)
{
//    unsigned pulse_width = 36000; //CyBot6
    unsigned pulse_width = 35200;            //Cybot 9

    // calculate pulse width in cycles
    TIMER1_TBMATCHR_R = (pulse_period - pulse_width) & 0xFFFFFF; // set pulse width
    TIMER1_TBPMR_R = (pulse_period - pulse_width) >> 16;
    // you need to call timer_waitMillis( ) here to enforce a delay for the servo to
    timer_waitMillis(100);
    // move to the position
}

/**
 * Moves the servo to the 0 degrees position.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @Josh Hall
 * @Yue Chen
 *
 * @date 7/3/2018
 */
void move_servo0(void)
{
//    unsigned pulse_width = 7200; //Cybot6
    unsigned pulse_width = 6400; //Cybot 9

    // calculate pulse width in cycles
    TIMER1_TBMATCHR_R = (pulse_period - pulse_width) & 0xFFFFFF; // set pulse width
    TIMER1_TBPMR_R = (pulse_period - pulse_width) >> 16;
    // you need to call timer_waitMillis( ) here to enforce a delay for the servo to
    timer_waitMillis(100);
    // move to the position
}


/**
 * Moves the servo by the X degrees.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @Josh Hall
 * @Yue Chen
 *
 *
 * @param degree: how much you want the servo to move by.
 * @date 7/3/2018
 */
int move_servo_by(int degree)
{
    if (total >= 180)
        {
            total = 0;
        }
        else if (total <= 0)
        {
            total = 0;
        }

    total += degree;

    unsigned pulse_width = convert(total);

    // calculate pulse width in cycles
    TIMER1_TBMATCHR_R = (pulse_period - pulse_width) & 0xFFFFFF; // set pulse width
    TIMER1_TBPMR_R = (pulse_period - pulse_width) >> 16;
    // you need to call timer_waitMillis( ) here to enforce a delay for the servo to
    timer_waitMillis(100);
    // move to the position
    return degree;
}


/**
 * Moves the servo to the X degrees.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @Josh Hall
 * @Yue Chen
 *
 *
 * @param degree: how much you want the servo to move by.
 * @date 7/3/2018
 */
int move_final(int degree)
{
    unsigned pulse_width = convert(degree);

    // calculate pulse width in cycles
    TIMER1_TBMATCHR_R = (pulse_period - pulse_width) & 0xFFFFFF; // set pulse width
    TIMER1_TBPMR_R = (pulse_period - pulse_width) >> 16;
    // you need to call timer_waitMillis( ) here to enforce a delay for the servo to
    timer_waitMillis(100);
    // move to the position
    return degree;
}

int button_press(uint8_t button)
{
    if (button == 4)
    {
        if (direction == 0)
        {
            direction = 1;
        }
        else
        {
            direction = 0;
        }
    }
    if (direction == 0)
    {
        if (button == 1)
        {
            move_servo_by(1);
        }
        else if (button == 2)
        {
            move_servo_by(2.5);
        }
        else if (button == 3)
        {
            move_servo_by(5);
        }
    }
    else if (direction == 1)
    {
        if (button == 1)
        {
            move_servo_by(-1);
        }
        else if (button == 2)
        {
            move_servo_by(-2.5);
        }
        else if (button == 3)
        {
            move_servo_by(-5);
        }
    }
    return total;
}

