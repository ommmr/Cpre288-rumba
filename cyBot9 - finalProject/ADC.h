/*
 * ADC.h
 *
 *  Created on: Jun 11, 2018
 *      Author: omabbas
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#include <inc/tm4c123gh6pm.h>

void ADC_init();
unsigned int ADC_read(char channel);
float convert_toCM(int adc_read);

#endif /* ADC_H_ */
