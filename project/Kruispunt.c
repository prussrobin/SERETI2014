#include <stdio.h>
#include <stdlib.h>
#include "ntk.h"
#include "Kruispunt.h"

static STD *kruispuntSTD=NULL;

static void initialAction(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
}

static void actionSet1Groen(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
}

static void actionSet2Groen(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
}

static void actionSet3Groen(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
}

static void actionSet4Groen(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
}

static void actionTijdOp(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
}

static void actionSchakelNoodStopIn(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
}

static void actionVervolgRegulier(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
}

static void defaultAction(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
}

static void construct_kruispuntSTD() {
    if (kruispuntSTD==NULL) {
        kruispuntSTD=(STD*)malloc(sizeof(STD));
        
        
        create_STD(
                kruispuntSTD,
                KRUISPUNT_AANTAL_TOESTANDEN,
                defaultAction);
        
        
        /*
         * AllesRood -> Set...Groen
         */
        
        addTransition_STD(
                kruispuntSTD, 
                StatusAllesRood, 
                EventSet1Groen, 
                StatusSet1Groen, 
                actionSet1Groen);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusAllesRood, 
                EventSet2Groen, 
                StatusSet2Groen, 
                actionSet2Groen);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusAllesRood, 
                EventSet3Groen, 
                StatusSet3Groen, 
                actionSet3Groen);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusAllesRood, 
                EventSet4Groen, 
                StatusSet4Groen, 
                actionSet4Groen);
        
        /*
         * Set...Groen -> AllesRood
         */
        
        
        addTransition_STD(
                kruispuntSTD, 
                StatusSet1Groen, 
                EventTijdOp, 
                StatusAllesRood, 
                actionTijdOp);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusSet2Groen, 
                EventTijdOp, 
                StatusAllesRood, 
                actionTijdOp);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusSet3Groen, 
                EventTijdOp, 
                StatusAllesRood, 
                actionTijdOp);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusSet4Groen, 
                EventTijdOp, 
                StatusAllesRood, 
                actionTijdOp);
        
        /*
         * AllesRood <-> NoodStop 
         */
        
        addTransition_STD(
                kruispuntSTD, 
                StatusAllesRood, 
                EventNoodStopAan, 
                StatusNoodStop, 
                actionSchakelNoodStopIn);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusNoodStop, 
                EventNoodStopUit, 
                StatusAllesRood, 
                actionVervolgRegulier);
        
        /*
         * Set...Groen -> NoodStop
         */
        
        addTransition_STD(
                kruispuntSTD, 
                StatusSet1Groen, 
                EventNoodStopAan, 
                StatusNoodStop, 
                actionSchakelNoodStopIn);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusSet2Groen, 
                EventNoodStopAan, 
                StatusNoodStop, 
                actionSchakelNoodStopIn);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusSet3Groen, 
                EventNoodStopAan, 
                StatusNoodStop, 
                actionSchakelNoodStopIn);
        
        addTransition_STD(
                kruispuntSTD, 
                StatusSet4Groen, 
                EventNoodStopAan, 
                StatusNoodStop, 
                actionSchakelNoodStopIn);
    }
}


void create_Kruispunt(Kruispunt **dptrKruispunt) {
    construct_kruispuntSTD();
    
    (*dptrKruispunt)=(Kruispunt*)malloc(sizeof(Kruispunt));
    
    (*dptrKruispunt)->status=StatusAllesRood;
    
    Rijbaan_Construct(&(*dptrKruispunt)->ptrR1, 1, 2, 0, 0);
    Rijbaan_Construct(&(*dptrKruispunt)->ptrR2, 3, 4, 0, 0);
    Rijbaan_Construct(&(*dptrKruispunt)->ptrR3, 5, 6, 0, 0);
    Rijbaan_Construct(&(*dptrKruispunt)->ptrR4, 7, 8, 0, 0);
    
    initialAction(*dptrKruispunt);
}

