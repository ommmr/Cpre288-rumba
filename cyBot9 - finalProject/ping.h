/*
 * ping.h
 *
 *  Created on: Jun 20, 2018
 *      Author: jetemple
 */

#ifndef PING_H_
#define PING_H_

void ping_init();
void pulse(void);
void TIMER3B_Handler(void);
float get_distance(void);



#endif /* PING_H_ */
