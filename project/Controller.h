/* 
 * File:   Controller.h
 * Author: thijs
 *
 * Created on March 19, 2014, 6:31 PM
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include "ntk.h"
#include "Kruispunt.h"

typedef struct {
    Kruispunt *ptrKruispunt;
    task *ptrTask;
    int intTest;
} Controller;


void Controller_Construct(Controller *ptrController, Kruispunt *ptrKruispunt);


#endif	/* CONTROLLER_H */

