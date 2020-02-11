/**
 * @file ADC.c
 * @brief This initializes the ADC and connects it to the
 * infrared sensor on the CyBot, all the calibrations have been done
 * for CyBot9
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chang
 *
 * @date 7/3/2018
 */

#include <stdint.h>
#include <math.h>

#include <inc/tm4c123gh6pm.h>

/**
 * This method initializes the ADC, follows steps from ADC datasheet section.
 *
 * @author Omar Abbas
 * @author Jack Temple
 *
 * @param void
 *
 * @date 7/3/2018
 */
void ADC_init()
{
    //enable ADC 0 module port D
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;

    //ENABLE CLOCK FOR ADC0
    SYSCTL_RCGCADC_R |= 0x1;

    //ENABLE PORT D PIN 0 TO WORK AS ALTERNATE FUNCTIONS
    GPIO_PORTD_AFSEL_R |= 0x01;

    //set pin4 to input, rest to out
    GPIO_PORTD_DEN_R &= 0b11101111;;

    //DISABLE ANALOG ISOLATION FOR THE PIN
    GPIO_PORTD_AMSEL_R |= 0b00010000;

    //INITIALIZE THE PORT TRIGGER SOURCE AS PROCESSOR
    GPIO_PORTD_ADCCTL_R = 0x00;

    //disable SS0 sample sequencer to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;

    //initialize the ADC trigger source as processor (default)
    ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;

    //set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX0_R |= 0x000A;

    //enable raw interrupt status
    ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);

    //enable oversampling to average
    ADC0_SAC_R |= ADC_SAC_AVG_64X;

    //re-enable ADC0 SS0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
}


/**
 * This method initializes the sample sequencers in the adc and gets a reading from it.
 *
 * @author Omar Abbas
 * @author Jack Temple
 *
 * @param channel : The channel that the input is going through
 *
 * @date 7/3/2018
 */

unsigned int ADC_read(char channel)
{
    //disable SS0 to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;

    //set first sample to use channel ADC pin
    ADC0_SSMUX0_R |= channel;

    //re-enable ADC0 SS0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;

    //initiate SS0 conversion
    ADC0_PSSI_R = ADC_PSSI_SS0;

    //wait for conversion to be complete
    while((ADC0_RIS_R & ADC_RIS_INR0) == 0){

    }

    //clear interrupt
    ADC0_ISC_R = ADC_ISC_IN0;

    return ADC0_SSFIFO0_R;

}
/**
 * This method takes a paramater which is supposed to be a reading from the adc
 * and converts it according to a graph with a linear equation y = mx + c.
 *
 * @author Omar Abbas
 * @author Jack Temple
 *
 * @param adc input which will be converted into centimeters according to graph
 *
 * @date 7/3/2018
 */

float convert_toCM(int adc_read){

    float adc_cm = 122594 * (pow(adc_read, -1.181)); //CyBot 9
//    float adc_cm = 253399 * pow(adc_read, -1.285);//Cybot 6

    return adc_cm;
}
