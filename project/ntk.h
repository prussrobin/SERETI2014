//---------------------------------------------------------------------------
#ifndef NTK_H
#define NTK_H

#ifndef _INC_WINDOWS
#include <windows.h>		/*for HANDLE*/
#define _INC_WINDOWS
#endif

#include <stdbool.h>

//---------------------------------------------------------------------------
//task prototypes -----------------------------------------------------------
typedef struct _task* task;

typedef unsigned (__stdcall * taskType)(void*);

typedef enum {low=-2,below,normal,above,high} priority;
/*
  In het gebruik van het object task onderkennen we twee mogelijkheden.
  Mogelijkheid 1:
  #include "ntk.h"
  ...
  task* t1,*t2;
  unsigned __stdcall taskCode(void* arg);

  we willen b.v. elke kopie van een taak, beschreven door taskCode,
  een uniek nummer als parameter doorgeven.
  De onderstaande taak kenmerkt zich door een 'oneindige lus' en zal weer
  'van buiten af' moeten worden verwijderd.
  unsigned __stdcall taskCode(void* arg);
  int taskId;
  ...
  taskId=1;
	t1=(task*)malloc(sizeof(task));
  create_task(t1,taskCode,&taskId,sizeOf(int),0);
  taskId=2;
	t2=(task*)malloc(sizeof(task));
  create_task(t2,taskCode,&taskId,sizeOf(int),0);
  ...
  delete_task(t1);
  free(t1);
  ...
  delete_task(t2);
  free(t2);
	...
  unsigned __stdcall taskCode(void* arg)
  {
  	task* t=(task*)(arg);
    int* i=(int*)getArgument_task(t);
    while(!isTerminated_task(t)){
  		...
    	*i geeft binnen taskCode voor elke kopie het unieke nummer
    	...
    }
    return 0;
  }


  Mogelijkheid 2:
  #include "ntk.h"
  ...
  task* t1,*t2;
  unsigned __stdcall taskCode(void* arg);

  we willen b.v. elke kopie van een taak, beschreven door taskCode,
  een uniek nummer als parameter doorgeven.
  De onderstaande taak kenmerkt zich door een 'eindige lus' en zal zich zelf
  moeten verwijderen.

  unsigned __stdcall taskCode(void* arg);
  int taskId;
  ...
  taskId=1;
	t1=(task*)malloc(sizeof(task));
  create_task(t1,taskCode,&taskId,sizeOf(int),false);
  taskId=2;
	t2=(task*)malloc(sizeof(task));
  create_task(t2,taskCode,&taskId,sizeOf(int),false);
  ...
  unsigned __stdcall taskCode(void* arg)
  {
  	task* t=(task*)(arg);
    int* i=(int*)getArgument_task(t);
 		...
   	*i geeft binnen taskCode voor elke kopie het unieke nummer
   	...
    terminate_task(t);
    return 0;
  }
*/

//creeert taak t, met als funktievoorschrift code.
//code wordt voorzien van parameter arg met lengt argSize
//wordt wel of niet suspended gecreeerd
void create_task(task* t,taskType code,void* arg,size_t argSize,int suspended);

//verwijdert taak t, laat de bijbehorende thread eindigen
void delete_task(task* t);

//hiermee verwijdert taak t zichzelf
void terminate_task(task* t);

//taken kunnen meerdere keren suspended worden. Dit resulteert in een suspend count.
//Wil een taak weer aktief worden, dan moet de suspend count 0 worden

//suspends t en retourneert previous suspend count(indien suspend lukt)
//retourneert 0xFFFFFFFF indien suspend niet lukt
unsigned long suspend_task(task* t);

//resumes t en retourneert previous suspend count (indien resume lukt),
//t wordt aktief, indien de suspend count 0 wordt
//retourneert 0xFFFFFFFF indien resume niet lukt
unsigned long resume_task(task* t);

//retourneert nonzero indien gelukt
int setPriority_task(task*,priority);

//retourneert de prioriteit
priority getPriority_task(task*);

//retourneer het argument
void* getArgument_task(task*);

//Is de taak afgelopen
int isTerminated_task(task*);

//---------------------------------------------------------------------------
//semaphore prototypes ------------------------------------------------------
typedef HANDLE semaphore;

//instantieer semaphore s met initiele waarde iVal en maximale waarde mVal
void create_semaphore(semaphore* s,long iVal,long mVal);

void delete_semaphore(semaphore* s);

void wait_semaphore(semaphore* s);

//return value:
//WAIT_OBJECT_0, signal geweest en timeOut niet verstreken
//WAIT_TIMEOUT, timeOut verstreken, signal niet geweest
unsigned long waitTimed_semaphore(semaphore* s,unsigned long timeOut);

void signal_semaphore(semaphore* s);

//signaleer n keer
void signalN_semaphore(semaphore* s,long n);

//---------------------------------------------------------------------------
//criticalSection prototypes ------------------------------------------------
typedef CRITICAL_SECTION criticalSection;

void create_criticalSection(criticalSection* c);

void delete_criticalSection(criticalSection* c);

void leave_criticalSection(criticalSection* c);

void enter_criticalSection(criticalSection* c);

//---------------------------------------------------------------------------
//mutal exclusion prototypes ------------------------------------------------
typedef HANDLE mutex;

void create_mutex(mutex* m);

void delete_mutex(mutex* m);

//return value:
//WAIT_OBJECT_0, mutex object verkregen en timeOut niet verstreken
//WAIT_TIMEOUT, timeOut verstreken, mutex object niet verkregen
unsigned long requestTimed_mutex(mutex*m,unsigned long timeOut);

void request_mutex(mutex* m);

void release_mutex(mutex* m);

//---------------------------------------------------------------------------
//event prototypes ----------------------------------------------------------
//een persistent event blijft gebeurd vanaf een set_event tot een expliciete
//reset_event.
//een non-persistent event zal als ongebeurd worden gekenmerkt na een
//succesvolle wait_event
typedef struct _event* event;

void create_event(event* e,int persist);

void delete_event(event* e);

//signaleer gebeurtenis
void set_event(event* e);

//de-signaleer gebeurtenis
void reset_event(event* e);

//set en reset, alle wachtende taken aktief
void pulse_event(event* e);

void wait_event(event* e);

//return value:
//WAIT_OBJECT_0, event gebeurd en timeOut niet verstreken
//WAIT_TIMEOUT, timeOut verstreken, event niet gebeurd
unsigned long	waitTimed_event(event* e,unsigned long timeOut);

//---------------------------------------------------------------------------
//mailbox prototypes --------------------------------------------------------
//Asynchrone communicatie tussen taken
//messages worden in FIFO order verwerkt
typedef struct _mailBox* mailBox;

void create_mailBox(mailBox* b,size_t sizeOfBuffer,size_t sizeOfElement);

void delete_mailBox(mailBox* b);

//lees bericht
void get_mailBox(mailBox* b,void* mess);

//return value:
//WAIT_OBJECT_0, message verkregen en timeOut niet verstreken
//WAIT_TIMEOUT, timeOut verstreken, geen message verkregen
unsigned long getTimed_mailBox(mailBox* b,void* mess,unsigned long timeOut);

//plaats bericht
void put_mailBox(mailBox* b,void* mess);

//return value:
//WAIT_OBJECT_0, bericht geplaatst en timeOut niet verstreken
//WAIT_TIMEOUT, timeOut verstreken, bericht niet geplaatst
unsigned long putTimed_mailBox(mailBox* b,void* mess,unsigned long timeOut);

//return value:
//false, indien mailBox niet leeg
//true, indien mailBox is leeg
int isEmpty_mailBox(mailBox* b);

//---------------------------------------------------------------------------
//rendezVous prototypes -----------------------------------------------------
//Synchrone communicatie tussen taken
typedef struct _rendezVous* rendezVous;

void create_rendezVous(rendezVous* r);

void delete_rendezVous(rendezVous* r);

void send_rendezVous(rendezVous* r,void* mess,size_t mLen);

//return value:
//WAIT_OBJECT_0, bericht uitgewisseld en timeOut niet verstreken
//WAIT_TIMEOUT, timeOut verstreken, bericht niet uitgewisseld
unsigned long sendTimed_rendezVous(rendezVous* r,void* mess,size_t mLen,unsigned long timeOut);

void receive_rendezVous(rendezVous* r,void* mess);

//return value:
//WAIT_OBJECT_0, bericht uitgewisseld verkregen en timeOut niet verstreken
//WAIT_TIMEOUT, timeOut verstreken, bericht niet uitgewisseld
unsigned long receiveTimed_rendezVous(rendezVous* r,void* mess,unsigned long timeOut);

//---------------------------------------------------------------------------
//rwLock prototypes --------------------------------------------------------
//
//ADT om het Readers/Rriters probleem op te lossen.
//Writers hebben exclusieve toegang
//Readers hebben parallelle toegang
//Writers hebben prioriteit t.o.v. Readers
//Gebruik:
//rwLock L;
//create_rwLock(&L);
//......
//Readers protocol:									Writers-protocol:
//startReading_rwLock(&L);          startWriting_rwLock(&L);
//<lees>														<schrijf>
//stopReading_rwLock(&L);		 				stopWriting_rwLock(&L);

typedef struct _rwLock* rwLock;

void create_rwLock(rwLock* l);

void delete_rwLock(rwLock* l);

//readers protocol
void startReading_rwLock(rwLock* l);

void stopReading_rwLock(rwLock* l);

//writers protocol
void startWriting_rwLock(rwLock* l);

void stopWriting_rwLock(rwLock* l);

//---------------------------------------------------------------------------
//STD prototypes ------------------------------------------------------------
//Table driven State Transition Diagram

typedef void (*actionType)(void *);
typedef struct _STD* STD;

//instantieer STD s, met nStates toestanden
//defAct wordt uitgevoerd bij een illegal event
void create_STD(STD* s,int nStates,actionType defAct);

void delete_STD(STD* s);

//voeg een overgang toe aan s
//een overgang wordt gekenmerkt door
//begintoestand state,
//gebeurtenis event,
//eindtoestand nextState en
//te ondernemen aktiviteit a
//Toestanden moeten gegeven worden in de vorm van een enumeratie,
//beginnend bij 0, oplopend in stappen van 1!!!
void addTransition_STD(STD* s,int state,int event,int nextState,actionType a);

//STD s bevindt zich in begintoestand currentState, gebeurtenis currentEvent
//vindt plaats. Gezocht wordt wat de te ondernemen aktie action is,
//De funktie retourneert de eindtoestand
int lookUp_STD(STD* s,int currentState,int currentEvent,actionType* action);

//---------------------------------------------------------------------------
//eventStore prototypes -----------------------------------------------------
//client moet een void sendEvent_client(client*,int) functie hebben,
//een persistent eventStore accumuleert gebeurtenissen
//een non-persistent eventStore maakt geen onderscheid tussen 1 of
//meerdere keren gebeurd.
//onderstaande events worden naar een eventStore object gestuurd.
//Dit gebeurt vanuit een client applicatie om het eventStore object af te vragen
//of een asynchroon event heeft plaats gevonden (doWait), of om het eventStore
//object weer in staat te stellen de asynchrone events te accumuleren (doReset),
//of om het ge-accumuleerde aantal asynchrone events uit het eventStore object
//te verwijderen (doClear)
//Verder wordt een asynchroon applicatie event getransformeerd naar het event
//aSync voor het eventStore object
//
typedef enum {doWait,				//inspecteer asynchroon event, client event
							doReset,			//accumuleer het asynchrone event, client event
              doClear,			//verwijder de gebeurtenissen, client event
							aSync}				//asynchroon event
              eventForEventStore;

typedef enum {persistent, nonPersistent}	eventStoreKind;

typedef void (*sendEventToClientF)(void*,int);

typedef struct _eventStore* eventStore;

//instantieert eventStore e, voor client c
//cEvent wordt met de functie s verstuurt naar c
//k is persistent of non-persistent
void create_eventStore(eventStore* e,void* c,sendEventToClientF s,int cEvent,eventStoreKind k);

void delete_eventStore(eventStore* e);

void sendEvent_eventStore(eventStore* e,eventForEventStore ev);

bool hasEvent_eventStore(eventStore*);

//---------------------------------------------------------------------------
//cRandom prototypes --------------------------------------------------------
//Als er gebruik wordt gemaakt van de standaard C randomizer faciliteiten,
//rand en randomize uit stdlib, dan krijgt elke parallele taak die deze
//mogelijkheden gebruikt, een zelfde random reeks van getallen.
//Het ADT cRandom, concurrent random, geeft een random reeks getallen af in
//een concurrent omgeving.
typedef struct _cRandom* cRandom;

//initialiseert een random number generator voor getallen tussen 0 en range-1
//indien pseudoRandom == 1, dan is het een pseudo random generator
void create_cRandom(cRandom* r,int range,int pseudoRandom);

void delete_cRandom(cRandom* r);

//retourneert een random number tussen 0 en range-1
int getNumber_cRandom(cRandom* r);


//---------------------------------------------------------------------------
//clientSocket prototypes
typedef struct _clientSocket* clientSocket;

void construct_ClSo(clientSocket*, const char *host, int port);

/* n==max aantal bytes */
//retourneert negatieve waarde indien error.
//Anders het aantal gelezen bytes
int read_ClSo(clientSocket*,char*, int n); //blocking

/* n==max aantal bytes */
//retourneert negatieve waarde indien error.
//Anders het aantal geschreven bytes
int write_ClSo(clientSocket*, const char *,int n);

//retourneert negatieve waarde indien error.
//Anders een, niet nader gespecificeerde, niet-negatieve waarde
int getStatus_ClSo(clientSocket*);

void destruct_ClSo(clientSocket*);
/*---------------------------------------------------------------------------*/
//serverSocket prototypes.
typedef struct _serverSocket* serverSocket;

void construct_SeSo(serverSocket*, const char *host, int port);

clientSocket* acceptClient_SeSo(serverSocket*); //blocking

//retourneert negatieve waarde indien error.
//Anders een, niet nader gespecificeerde, niet-negatieve waarde
int getStatus_SeSo(serverSocket*);

void destruct_SeSo(serverSocket*);
//---------------------------------------------------------------------------

#endif
