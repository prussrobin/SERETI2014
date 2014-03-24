/* 
 * File:   Rijbaan.h
 * Author: Robin
 *
 * Created on 17 maart 2014, 14:39
 */

#ifndef RIJBAAN_H
#define	RIJBAAN_H

#include "Stoplicht.h"

typedef struct{
    int ID;
    Stoplicht *ptrSLrr;
    Stoplicht *ptrSLl;
}Rijbaan;

void Rijbaan_Construct(Rijbaan* ptrRijbaan, int SLrr_ID, int SLl_ID, int Sensor_ID1, int Sensor_ID2);
void Rijbaan_Print();

#endif	/* RIJBAAN_H */

