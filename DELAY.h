/*
 * delay.h
 *
 *  Created on: Oct 26, 2018
 *      Author: Itachi
 */

#ifndef DELAY_H_
#define DELAY_H_

#define PF1       (*((volatile unsigned long *)0x40025008))
static unsigned int Tick=0;
static unsigned long Tick_monitor=0;
void Systick_Init();
void Monitor(void);
void delay_us(unsigned int microseconds);
#endif /* DELAY_H_ */
