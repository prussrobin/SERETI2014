#include <stdlib.h>
#include <stdio.h>
#include "Controller.h"
#include "ntk.h"

#define CONTROLLER_DEBUG 1


static unsigned __stdcall Controller_Task(void* arg){
	task* t=(task*)arg;
	Controller **ptrController1=(Controller**)(getArgument_task(t));
        Controller *ptrController=(Controller*)ptrController1;
        
        int set = 1;
        
        printf("Controller_Task(): ptrController->intTest = %d\n", ptrController->intTest);
        printf("Controller_Task(): ptrController->ptrKruispunt->intTest = %d\n", ptrController->ptrKruispunt->intTest);
        
        while (!isTerminated_task(t)) {
            
            if (CONTROLLER_DEBUG) {
                printf("Controller_Task(): ");
                Kruispunt_PrintState(ptrController->ptrKruispunt);
            }
            
            switch(ptrController->ptrKruispunt->kruispuntStatus) {
                case StatusAllesRood:
                    switch (set){
                        case 1:
                            Kruispunt_SendEvent(ptrController->ptrKruispunt, EventSet1Groen);
                            set++;
                            break;
                        case 2:
                            Kruispunt_SendEvent(ptrController->ptrKruispunt, EventSet2Groen);
                            set++;
                            break;
                        case 3:
                            Kruispunt_SendEvent(ptrController->ptrKruispunt, EventSet3Groen);
                            set++;
                            break;
                        case 4:
                            Kruispunt_SendEvent(ptrController->ptrKruispunt, EventSet4Groen);
                            set=1;
                            break;
                    }
                    break;
                case StatusActieveSetGroen:
                    Kruispunt_SendEvent(ptrController->ptrKruispunt, EventOranje);
                    break;
                case StatusActieveSetOranje:
                    Kruispunt_SendEvent(ptrController->ptrKruispunt, EventRood);
                    break;
                default:
                    if (CONTROLLER_DEBUG) {
                        printf("Controller_Task(): kruispunt->status = unknown\n");
                    }
                    break;
            }
            
            sleep(1);
        }
        
        return 0;
}

void Controller_Construct(Controller **dptrController, Kruispunt *ptrKruispunt) {
    (*dptrController)=(Controller*)malloc(sizeof(Controller));
    (*dptrController)->ptrKruispunt=ptrKruispunt;
    (*dptrController)->intTest = 333;
    
    printf("Controller_Construct(): ");
    (*dptrController)->ptrKruispunt->kruispuntStatus = StatusActieveSetOranje;
    (*dptrController)->ptrKruispunt->actieveSet = 1;
    Kruispunt_PrintAll((*dptrController)->ptrKruispunt);
    
    //Taak creeren en starten voor het kruispunt
    (*dptrController)->ptrTask=(task*)malloc(sizeof(task));
    create_task((*dptrController)->ptrTask,Controller_Task,*dptrController,sizeof(Controller),0);
}