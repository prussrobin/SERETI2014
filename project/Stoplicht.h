/* 
 * File:   Stoplicht.h
 * Author: Robin
 *
 * Created on 17 maart 2014, 14:28
 */

#ifndef STOPLICHT_H
#define	STOPLICHT_H

typedef struct{
    int ID;
    int HuidigeStatus;
    Sensor InSensor;
    Sensor UitSensor;
}Stoplicht;

#define STOPLICHT_GROEN 1
#define STOPLICHT_ORANJE 2
#define STOPLICHT_ROOD 3

void Stoplicht_Construct(Stoplicht** ptrStoplicht, int Stoplicht_ID, int Stoplicht_State);
void Stoplicht_Set_State(Stoplicht *obj, int state);
int Stoplicht_Get_State(Stoplicht *obj);


#endif	/* STOPLICHT_H */

