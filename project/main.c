
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

event *eventQuit;



int main_thijs(int argc, char** argv) {
    //Rijbaan *R1;
    //Rijbaan_Construct(&R1, 1, 2, 1, 2);
    //Rijbaan_Print(R1);
    
    Kruispunt *ptrKruispunt=(Kruispunt*)malloc(sizeof(Kruispunt));
    Kruispunt_Construct(ptrKruispunt);
    
    sleep(1);
    
    Kruispunt_PrintAll(ptrKruispunt);
    
    Controller *ptrController=(Controller*)malloc(sizeof(Controller));
    Controller_Construct(ptrController, ptrKruispunt);
    
    sleep(3);
    
    return (EXIT_SUCCESS);
}
      
int main_robin(int argc, char** argv) {
    
    //Rijbaan *R1;
    //Rijbaan_Construct(&R1, 1, 2, 1, 2);
    //Rijbaan_Print(R1);
    
    Kruispunt *ptrKruispunt=NULL;
    Kruispunt_Construct(ptrKruispunt);
    
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

void testTimerAction(void *obj) {
    printf("derp\n");
}

int main_timertest(int argc, char** argv) {
    
    Timer *t=(Timer*)malloc(sizeof(Timer));
    Timer_Construct(t);
    
    Timer_Start(t,*testTimerAction,NULL,1000);
    Timer_Start(t,*testTimerAction,NULL,1000);
    Timer_Stop(t);
    Timer_Start(t,*testTimerAction,NULL,2000);
    
    //wait_event(ptrEventDone);
    sleep(4);
    
    return (EXIT_SUCCESS);
}


/*
 * 
 */
int main(int argc, char** argv) {
    //eventQuit=(event*)malloc(sizeof(event));
    //create_event(eventQuit,1);
    
    Kruispunt *k=malloc(sizeof(Kruispunt));
    Kruispunt_Construct(k);
    
    /*
     * "block"  de main thread, terwijl Kruispunt_Task z'n ding doet.
     * Heb while(1); geprobeerd, en wait_event(eventQuit); maar netbeans
     * vindt dat niet zo leuk. Dus dan maar sleep(groooootgetalll); 
     */
    sleep(60*5);
    
    //return main_timertest(argc, argv);
    return (EXIT_SUCCESS);
}
