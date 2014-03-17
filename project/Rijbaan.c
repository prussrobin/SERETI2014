
#include <stdlib.h>
#include "Stoplicht.h"
#include "Rijbaan.h"

void Rijbaan_Construct(Rijbaan** ptrRijbaan, int SLrr_ID, int SLl_ID, int Sensor_ID1, int Sensor_ID2){
    (*ptrRijbaan)=(Rijbaan*)malloc(sizeof(Rijbaan));
    
    (*ptrRijbaan)->ptrSLrr->ID = SLrr_ID;
    (*ptrRijbaan)->ptrSLrr->HuidigeStatus = STOPLICHT_ROOD;
    (*ptrRijbaan)->ptrSLl->ID = SLl_ID;
    (*ptrRijbaan)->ptrSLl->HuidigeStatus = STOPLICHT_ROOD;       
}