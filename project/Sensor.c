#include "ntk.h"
#include "Kruispunt.h"
#include <stdio.h>
#include <stdlib.h>

void event_AutoOp_Sensor(Kruispunt* v,KruispuntEvent e){
	put_mailBox(&(v->mailForKruispunt),&e);
}

void event_AutoAf_Sensor(Kruispunt* v,KruispuntEvent e){
	put_mailBox(&(v->mailForKruispunt),&e);
}

static unsigned __stdcall SensorTask(void* arg){
	task* t=(task*)arg;
	Kruispunt* v=*((Kruispunt**)(getArgument_task(t)));
	KruispuntEvent e;
	actionType a;

	//Zolang het vat nog bestaat, zal deze taak actief blijven
	while(!isTerminated_task(t)){
		//Wacht tot er een gebeurtenis (event) plaats heeft gevonden, mbv een mailbox
		get_mailBox(&(v->mailForKruispunt),&e);

		//Zoek de status op en voer de juiste actie uit.

		//<NIEUWE TOESTAND> = lookUp_STD(<VERWIJZING NAAR STD>, <HUIDIGE TOESTAND>, <GEBEURTENIS>, <WELKE ACTIE?>)
		//v->status = (vatStatus)lookUp_STD(vatSTD,v->status,e,&a);
		
		//Voer actie uit
		a(v);
	}

	return 0;
}

void Sensor_Construct(Sensor** ptrSensor, int Stoplicht_ID, int Sensor_ID, int InUit){
    (*ptrSensor)=(Sensor*)malloc(sizeof(Sensor));
    (*ptrSensor)->ID = Sensor_ID;
    (*ptrSensor)->InOfUitSensor = InUit;
    (*ptrSensor)->StoplichtID = Stoplicht_ID;
}
