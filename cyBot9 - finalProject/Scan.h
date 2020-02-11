/*
 * Scan.h
 *
 *  Created on: Jun 28, 2018
 *      Author: omabbas
 */


#include "lcd.h"
#include "uart.h"
#include "button.h"
#include <stdint.h>
#include <string.h>
#include "servo.h"
#include "ping.h"
#include "ADC.h"
#include <math.h>
    struct object
{

    float distance; //distance to servo
    int radial_size;
    int degrees;
    int linear_width; //added
};

typedef struct object object_t;

    float linear_width(object_t obj);
    void scan(void);
    int get_numObjects(void);




