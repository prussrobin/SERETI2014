
#include <stdlib.h>
#include <stdio.h>
#include "Stoplicht.h"
#include "Rijbaan.h"

void Rijbaan_Construct(Rijbaan* ptrRijbaan, int SLrr_ID, int SLl_ID, int Sensor_ID1, int Sensor_ID2){
    ptrRijbaan->ptrSLl=(Stoplicht*)malloc(sizeof(Stoplicht));
    ptrRijbaan->ptrSLrr=(Stoplicht*)malloc(sizeof(Stoplicht));
    
    Stoplicht_Construct(ptrRijbaan->ptrSLl, SLl_ID, STOPLICHT_ROOD);
    Stoplicht_Construct(ptrRijbaan->ptrSLrr, SLrr_ID, STOPLICHT_ROOD);
}

void Rijbaan_Print(Rijbaan* ptrRijbaan){
    printf("Rijbaan %i aangemaakt en stoplicht %i (%i) en %i (%i)\n", 1, ptrRijbaan->ptrSLrr->ID, ptrRijbaan->ptrSLrr->HuidigeStatus, ptrRijbaan->ptrSLl->ID, ptrRijbaan->ptrSLl->HuidigeStatus );
    
}