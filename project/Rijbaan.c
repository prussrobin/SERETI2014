
#include <stdlib.h>
#include <stdio.h>
#include "Stoplicht.h"
#include "Rijbaan.h"

void Rijbaan_Construct(Rijbaan** dptrRijbaan, int SLrr_ID, int SLl_ID, int Sensor_ID1, int Sensor_ID2){
    (*dptrRijbaan)=(Rijbaan*)malloc(sizeof(Rijbaan));
    
    Stoplicht_Construct(&((*dptrRijbaan)->ptrSLl), SLl_ID, STOPLICHT_ROOD);
    Stoplicht_Construct(&((*dptrRijbaan)->ptrSLrr), SLrr_ID, STOPLICHT_ROOD);
}

void Rijbaan_Print(Rijbaan* ptrRijbaan){
    printf("Rijbaan %i aangemaakt en stoplicht %i (%i) en %i (%i)\n", 1, ptrRijbaan->ptrSLrr->ID, ptrRijbaan->ptrSLrr->HuidigeStatus, ptrRijbaan->ptrSLl->ID, ptrRijbaan->ptrSLl->HuidigeStatus );
    
}