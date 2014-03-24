
/* 
 * File:   main.c
 * Author: Robin
 *
 * Created on 17 maart 2014, 13:39
 */

#include <stdio.h>
#include <stdlib.h>
#include "Rijbaan.h"
#include "Stoplicht.h"
#include "ntk.h"
#include "Kruispunt.h"
#include "Controller.h"
#include "Timer.h"

int main_thijs(int argc, char** argv) {
    //Rijbaan *R1;
    //Rijbaan_Construct(&R1, 1, 2, 1, 2);
    //Rijbaan_Print(R1);
    
    Kruispunt *ptrKruispunt;
    Kruispunt_Construct(&ptrKruispunt);
    
    sleep(1);
    
    Kruispunt_PrintAll(ptrKruispunt);
    
    Controller *ptrController;
    Controller_Construct(&ptrController, ptrKruispunt);
    
    sleep(3);
    
    return (EXIT_SUCCESS);
}
        
        


int main_robin(int argc, char** argv) {
    
    //Rijbaan *R1;
    //Rijbaan_Construct(&R1, 1, 2, 1, 2);
    //Rijbaan_Print(R1);
    
    Kruispunt *ptrKruispunt=NULL;
    Kruispunt_Construct(&ptrKruispunt);
    
    Kruispunt_SendEvent(ptrKruispunt,EventNoodStopUit);
    
    sleep(1);
    
    Kruispunt_SendEvent(ptrKruispunt,EventSet1Groen);
    
    sleep(1);
    
    Kruispunt_SendEvent(ptrKruispunt,EventNoodStopAan);
    
    sleep(1);
    
    Kruispunt_SendEvent(ptrKruispunt,EventOranje);
    
    sleep(1);
    
    Kruispunt_SendEvent(ptrKruispunt,EventRood);
    
    sleep(1);
    
    Kruispunt_SendEvent(ptrKruispunt,EventNoodStopAan);
    
    sleep(1);
    
    
    return (EXIT_SUCCESS);
}

void derp() {
    printf("derp\n");
}

int main_timertest(int argc, char** argv) {
    
    Timer *t=(Timer*)malloc(sizeof(Timer));
    Timer_Construct(t);
    
    Timer_Start(t,*derp,1000);
    Timer_Start(t,*derp,1000);
    Timer_Stop(t);
    Timer_Start(t,*derp,2000);
    
    //wait_event(ptrEventDone);
    sleep(4);
    
    return (EXIT_SUCCESS);
}


/*
 * 
 */
int main(int argc, char** argv) {
    return main_timertest(argc, argv);
}
