/* 
 * File:   Kruispunt.h
 * Author: thijs
 *
 * Created on March 18, 2014, 3:54 PM
 */

#ifndef KRUISPUNT_H
#define	KRUISPUNT_H

#include "ntk.h"

#include "Counter.h"
#include "Rijbaan.h"
#include "Sensor.h"
#include "Stoplicht.h"


#define KRUISPUNT_AANTAL_TOESTANDEN 6


typedef enum {
    StatusAllesRood,
    StatusSet1Groen,
    StatusSet2Groen,
    StatusSet3Groen,
    StatusSet4Groen,
    StatusNoodStop
} KruispuntStatus;



typedef enum {
    EventNoodStopAan,
    EventNoodStopUit,
    EventTijdOp,
    EventSet1Groen,
    EventSet2Groen,
    EventSet3Groen,
    EventSet4Groen
} KruispuntEvent;



typedef struct {
    Rijbaan *ptrR1;
    Rijbaan *ptrR2;
    Rijbaan *ptrR3;
    Rijbaan *ptrR4;
    KruispuntStatus status;
    task *ptrKruispuntController;
    mailBox mailForKruispunt;
} Kruispunt;


void create_Kruispunt(Kruispunt **dptrKruispunt);

#endif	/* KRUISPUNT_H */

