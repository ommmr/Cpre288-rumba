/*
*
*   uart.h
*
*   Used to set up the RS232 connector and WIFI module
*   uses UART1 at 115200
*
*
*   @author Dane Larson
*   @date 07/18/2016
*/


#include "Timer.h"
#include <inc/tm4c123gh6pm.h>

void timer1_init(void);

void move_servo0(void);
void move_servo180(void);
int move_servo_by(int degree);
int button_press(uint8_t button);
unsigned convert(int degrees);
int move_final(int degree);




