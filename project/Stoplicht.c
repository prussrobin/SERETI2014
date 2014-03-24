#include <stdio.h>
#include <stdlib.h>
#include "Stoplicht.h"

void Stoplicht_Construct(Stoplicht* ptrStoplicht, int Stoplicht_ID, int Stoplicht_State){
    ptrStoplicht->ID = Stoplicht_ID;
    Stoplicht_Set_State(ptrStoplicht, Stoplicht_State);
}

void Stoplicht_Set_State(Stoplicht *obj, int state){
    switch(state){
        case STOPLICHT_GROEN:
        case STOPLICHT_ORANJE:
        case STOPLICHT_ROOD:
            obj->HuidigeStatus = state;
            break;
        default:
            //TODO handle invalid arg
            break;
    }
}

int Stoplicht_Get_State(Stoplicht *obj){
    return obj->HuidigeStatus;
}