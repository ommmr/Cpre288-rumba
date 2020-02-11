
#include "lcd.h"
#include "uart.h"
#include <stdint.h>
#include <string.h>
#include "servo.h"
#include "ping.h"
#include "ADC.h"
#include <math.h>
#include "Scan.h"
#include <open_interface.h>
#include <movement.h>


/**
 * Main function, includes initializations and memory allocation for open interface
 * mode. Only function is moveservo 0 used to 0 poistion the servo, and command, which has everything
 * else
 */

/**
 * Main function, includes initializations and memory allocation for open interface mode.
 * Only function is moveservo_0 used to 0 position the servo. Command is the function
 * where all other functions are, such as scan, move..etc.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
int main(void){

    lcd_init();
    uart_init();
    ping_init();
    ADC_init();
    timer1_init();

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    int dist = 100;
    int angle = 30;

    move_servo0();
    while(1){

        command(dist, angle, sensor_data);
    }
}
