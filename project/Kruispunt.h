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


#define KRUISPUNT_AANTAL_TOESTANDEN 5
#define GEEN_SET -1


typedef enum {
    StatusAllesRood,
    StatusActieveSetGroen,
    StatusActieveSetOranje,
    StatusNoodStopOranje,
    StatusNoodStop
} KruispuntStatus;



typedef enum {
    EventNoodStopAan,
    EventNoodStopUit,
    EventSet1Groen,
    EventSet2Groen,
    EventSet3Groen,
    EventSet4Groen,
    EventOranje,
    EventRood
} KruispuntEvent;



typedef struct {
    Rijbaan *ptrR1;
    Rijbaan *ptrR2;
    Rijbaan *ptrR3;
    Rijbaan *ptrR4;
    KruispuntStatus status;
    int actieveSet;
    
    int illegalEvents;
    task *ptrTask;
    mailBox mailbox;
} Kruispunt;


void Kruispunt_Construct(Kruispunt **dptrKruispunt);
void Kruispunt_SendEvent(Kruispunt* ptrKruispunt,KruispuntEvent e);

/*
 * Print functies. Voor debug purposes.
 */
void Kruispunt_PrintAll(Kruispunt *ptrKruispunt);
void Kruispunt_PrintState(Kruispunt *ptrKruispunt);
void Kruispunt_PrintEventLabelInline(KruispuntEvent e);
void Kruispunt_PrintStatusLabelInline(KruispuntStatus e);

#endif	/* KRUISPUNT_H */

