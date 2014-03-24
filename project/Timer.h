/* 
 * File:   Timer.h
 * Author: thijs
 *
 * Created on March 24, 2014, 4:14 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include "ntk.h"

typedef struct {
    task *ptrTask;
    mailBox mailbox;
} Timer;

void Timer_Construct(Timer *ptrTimer);
void Timer_Start(Timer *ptrTimer, void (*action)(void*), void *arg, unsigned long milliseconds);
void Timer_Stop(Timer *ptrTimer);

#endif	/* TIMER_H */

