
#include <stdlib.h>
#include <stdio.h>
#include "Stoplicht.h"
#include "Rijbaan.h"
#include "Sensor.h"

void Rijbaan_Construct(Rijbaan** dptrRijbaan, int SLrr_ID, int SLl_ID, int SensorRR1_ID, int SensorL1_ID, int SensorRR2_ID, int SensorL2_ID) {
    (*dptrRijbaan) = (Rijbaan*) malloc(sizeof (Rijbaan));

    Stoplicht_Construct(&((*dptrRijbaan)->ptrSLl), SLl_ID, STOPLICHT_ROOD);
    Sensor_Construct(&((*dptrRijbaan)->ptrSLl->InSensor), SLl_ID, SensorL1_ID, 0);
    Sensor_Construct(&((*dptrRijbaan)->ptrSLl->InSensor), SLl_ID, SensorL2_ID, 1);
    Stoplicht_Construct(&((*dptrRijbaan)->ptrSLrr), SLrr_ID, STOPLICHT_ROOD);
    Sensor_Construct(&((*dptrRijbaan)->ptrSLl->InSensor), SLrr_ID, SensorRR1_ID, 0);
    Sensor_Construct(&((*dptrRijbaan)->ptrSLl->InSensor), SLrr_ID, SensorRR2_ID, 1);
}

void Rijbaan_Print(Rijbaan* ptrRijbaan) {   
    printf("Rijbaan %i aangemaakt en stoplicht %i (%i) en %i (%i)\n", 1, ptrRijbaan->ptrSLrr->ID, ptrRijbaan->ptrSLrr->HuidigeStatus, ptrRijbaan->ptrSLl->ID, ptrRijbaan->ptrSLl->HuidigeStatus);

}