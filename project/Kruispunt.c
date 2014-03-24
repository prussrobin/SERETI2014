#include <stdio.h>
#include <stdlib.h>
#include "ntk.h"
#include "Kruispunt.h"

#define KRUISPUNT_DEBUG 1


static STD *kruispuntSTD=NULL;


static void initialAction(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:initalAction: \n");
    }
}



static void actionSet1Groen(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionSet1Groen: \n");
    }
    ptrKruispunt->actieveSet=1;
}

static void actionSet2Groen(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionSet2Groen: \n");
    }
    ptrKruispunt->actieveSet=2;
}

static void actionSet3Groen(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionSet3Groen: \n");
    }
    ptrKruispunt->actieveSet=3;
}

static void actionSet4Groen(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionSet4Groen: \n");
    }
    ptrKruispunt->actieveSet=4;
}



static void actionGroenNaarOranje(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionGroenNaarOranje: \n");
    }
}



static void actionOranjeNaarRood(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionOranjeNaarRood: \n");
    }
    ptrKruispunt->actieveSet=GEEN_SET;
}



static void actionGroenNaarNoodStopOranje(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionGroenNaarNoodStopOranje: \n");
    }
}

static void actionOranjeNaarNoodStopOranje(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionOranjeNaarNoodStopOranje: \n");
    }
}




static void actionNoodStopOranjeNaarNoodStop(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionNoodStopOranjeNaarNoodStop: \n");
    }
    ptrKruispunt->actieveSet=GEEN_SET;
}



static void actionRoodNaarNoodStop(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionRoodNaarNoodStop: \n");
    }
}

static void actionVervolgRegulier(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:actionVervolgRegulier: \n");
    }
}



static void defaultAction(void *obj) {
    Kruispunt *ptrKruispunt=(Kruispunt*)obj;
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt.c:defaultAction: ");
        Kruispunt_PrintState(ptrKruispunt);
    }
    ptrKruispunt->illegalEvents++;
}




static void construct_kruispuntSTD() {
    if (kruispuntSTD==NULL) {
        kruispuntSTD=(STD*)malloc(sizeof(STD));
        
        
        create_STD(
                kruispuntSTD,
                KRUISPUNT_AANTAL_TOESTANDEN,
                defaultAction);
        
        
        //AllesRood -> ActieveSetGroen
        addTransition_STD(kruispuntSTD, StatusAllesRood, EventSet1Groen, StatusActieveSetGroen, actionSet1Groen);
        addTransition_STD(kruispuntSTD, StatusAllesRood, EventSet2Groen, StatusActieveSetGroen, actionSet2Groen);
        addTransition_STD(kruispuntSTD, StatusAllesRood, EventSet3Groen, StatusActieveSetGroen, actionSet3Groen);
        addTransition_STD(kruispuntSTD, StatusAllesRood, EventSet4Groen, StatusActieveSetGroen, actionSet4Groen);
        //AllesRood -> NoodStop
        addTransition_STD(kruispuntSTD, StatusAllesRood, EventNoodStopAan, StatusNoodStop, actionRoodNaarNoodStop);
        //NoodStop -> AllesRood
        addTransition_STD(kruispuntSTD, StatusNoodStop, EventNoodStopUit, StatusAllesRood, actionVervolgRegulier);
        //ActieveSetGroen -> ActieveSetOranje
        addTransition_STD(kruispuntSTD, StatusActieveSetGroen, EventOranje, StatusActieveSetOranje, actionGroenNaarOranje);
        //ActieveSetGroen -> NoodStopOranje
        addTransition_STD(kruispuntSTD, StatusActieveSetGroen, EventNoodStopAan, StatusNoodStopOranje, actionGroenNaarNoodStopOranje);
        //ActieveSetOranje -> AllesRood
        addTransition_STD(kruispuntSTD, StatusActieveSetOranje, EventRood, StatusAllesRood, actionOranjeNaarRood);
        //ActieveSetOranje -> NoodStopOranje
        addTransition_STD(kruispuntSTD, StatusActieveSetOranje, EventNoodStopAan, StatusNoodStopOranje, actionOranjeNaarNoodStopOranje);
        //NoodStopOranje -> NoodStop
        addTransition_STD(kruispuntSTD, StatusNoodStopOranje, EventRood, StatusNoodStop, actionNoodStopOranjeNaarNoodStop);
        
    }
}



//Taak voor het Kruispunt object
static unsigned __stdcall Kruispunt_Task(void* arg){
	task* t=(task*)arg;
	Kruispunt *ptrKruispunt=(Kruispunt*)(getArgument_task(t));
	KruispuntEvent e;
	actionType a;
        
	//Zolang het kruispunt nog bestaat, zal deze taak actief blijven
	while(!isTerminated_task(t)){
            //Wacht tot er een gebeurtenis (event) plaats heeft gevonden, mbv een mailbox
            get_mailBox(&(ptrKruispunt->mailbox),&e);
            
            if (KRUISPUNT_DEBUG) {
                printf("Kruispunt_Task: get from mailbox (");
                Kruispunt_PrintEventLabelInline(e);
                printf(")\n");
            }

            //Zoek de status op en voer de juiste actie uit.

            //<NIEUWE TOESTAND> = lookUp_STD(<VERWIJZING NAAR STD>, <HUIDIGE TOESTAND>, <GEBEURTENIS>, <WELKE ACTIE?>)
            ptrKruispunt->kruispuntStatus = (KruispuntStatus)lookUp_STD(kruispuntSTD,ptrKruispunt->kruispuntStatus,e,&a);
            
            //Voer actie uit
            a(ptrKruispunt);
            
            if (KRUISPUNT_DEBUG) {
                printf("Kruispunt_Task: ");
                Kruispunt_PrintState(ptrKruispunt);
                printf("\n");
            }
	}

	return 0;
}




void Kruispunt_Construct(Kruispunt *ptrKruispunt) {
    // STD bouwen.
    construct_kruispuntSTD();
    
    // Initieele status
    ptrKruispunt->kruispuntStatus=StatusAllesRood;
    
    // Rijbanen constructen
    ptrKruispunt->ptrR1=(Rijbaan*)malloc(sizeof(Rijbaan));
    ptrKruispunt->ptrR2=(Rijbaan*)malloc(sizeof(Rijbaan));
    ptrKruispunt->ptrR3=(Rijbaan*)malloc(sizeof(Rijbaan));
    ptrKruispunt->ptrR4=(Rijbaan*)malloc(sizeof(Rijbaan));
    // Rijbaan_Construct(&ptrRijbaan, licht1 ID, licht2 ID, sensor1 ID, sensor2 ID)
    Rijbaan_Construct(ptrKruispunt->ptrR1, 10, 11, 10, 11); //, 12, 13);
    Rijbaan_Construct(ptrKruispunt->ptrR2, 20, 21, 20, 21); //, 22, 23);
    Rijbaan_Construct(ptrKruispunt->ptrR3, 30, 31, 30, 31); //, 32, 33);
    Rijbaan_Construct(ptrKruispunt->ptrR4, 40, 41, 40, 41); //, 42, 43);
    
    // Alles staat op rood, dus geen set actief.
    ptrKruispunt->actieveSet=GEEN_SET;
    
    //
    ptrKruispunt->illegalEvents=0;
    
    ptrKruispunt->intTest = 444;
    
    // Voer actie uit die van Entry Point naar AllesRood zou gaan.
    initialAction(ptrKruispunt);
    
    
    
    //Mailbox aanmaken, waar 10 berichten in passen
    create_mailBox(&(ptrKruispunt->mailbox),10,sizeof(KruispuntEvent));

    //Taak creeren en starten voor het kruispunt
    ptrKruispunt->ptrTask=(task*)malloc(sizeof(task));
    create_task(ptrKruispunt->ptrTask,Kruispunt_Task,ptrKruispunt,sizeof(Kruispunt),0);
}

void Kruispunt_SendEvent(Kruispunt *ptrKruispunt, KruispuntEvent e) {
    if (KRUISPUNT_DEBUG) {
        printf("Kruispunt_SendEvent: put in mailbox (");
        Kruispunt_PrintEventLabelInline(e);
        printf(")\n");
    }
    
    put_mailBox(&(ptrKruispunt->mailbox),&e);
}


void Kruispunt_PrintAll(Kruispunt *ptrKruispunt) {
    printf("Kruispunt[\n");
    printf("\tptrR1: ");
    Rijbaan_Print(ptrKruispunt->ptrR1);
    printf("\tptrR2: ");
    Rijbaan_Print(ptrKruispunt->ptrR2);
    printf("\tptrR3: ");
    Rijbaan_Print(ptrKruispunt->ptrR3);
    printf("\tptrR4: ");
    Rijbaan_Print(ptrKruispunt->ptrR4);
    printf("\tkruispuntStatus: ");
    Kruispunt_PrintStatusLabelInline(ptrKruispunt->kruispuntStatus);
    printf("\n");
    printf("\tactieveSet: %d\n", ptrKruispunt->actieveSet);
    printf("\tillegalEvents: %d\n", ptrKruispunt->illegalEvents);
    printf("]\n");
}

void Kruispunt_PrintState(Kruispunt *ptrKruispunt) {
    printf("Kruispunt[ kruispuntStatus=");
    Kruispunt_PrintStatusLabelInline(ptrKruispunt->kruispuntStatus);
    printf(", illegalEvents=%d, actieveSet=%d ]\n", 
            ptrKruispunt->illegalEvents,
            ptrKruispunt->actieveSet);
}

void Kruispunt_PrintEventLabelInline(KruispuntEvent e) {
    switch (e) {
        case EventNoodStopAan:
            printf("EventNoodStopAan");
            break;
        case EventNoodStopUit:
            printf("EventNoodStopUit");
            break;
        case EventSet1Groen:
            printf("EventSet1Groen");
            break;
        case EventSet2Groen:
            printf("EventSet2Groen");
            break;
        case EventSet3Groen:
            printf("EventSet3Groen");
            break;
        case EventSet4Groen:
            printf("EventSet4Groen");
            break;
        case EventOranje:
            printf("EventOranje");
            break;
        case EventRood:
            printf("EventRood");
            break;
        default:
            printf("unknown");
            break;
    }
}

void Kruispunt_PrintStatusLabelInline(KruispuntStatus s) {
    switch(s) {
        case StatusActieveSetGroen:
            printf("ActieveSetGroen");
            break;
        case StatusActieveSetOranje:
            printf("ActieveSetOranje");
            break;
        case StatusAllesRood:
            printf("AllesRood");
            break;
        case StatusNoodStop:
            printf("NoodStop");
            break;
        case StatusNoodStopOranje:
            printf("NoodStopOranje");
            break;
        default:
            printf("unknown");
            break;
    }
}