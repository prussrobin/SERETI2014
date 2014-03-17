//---------------------------------------------------------------------------
//#define MY_DEBUG_PRINT(f, s) fprintf(f, "\n%s", s)
#define MY_DEBUG_PRINT(f, s) 0

#include <pthread.h>

#ifndef WINSOCK2_H
#include <winsock2.h>
#define WINSOCK2_H
#endif

#include "ntk.h"

#ifndef _INC_WINDOWS
#include <windows.h>
#define _INC_WINDOWS
#endif

#ifndef PROCESS_H
#include <process.h>
#define PROCESS_H
#endif         

#ifndef STDLIB_H
#include <stdlib.h>   //for rand();
#define STDLIB_H
#endif

#ifndef TIME_H
#include <time.h>			//for srand(unsigned);
#define TIME_H
#endif

#ifndef STDIO_H
#include <stdio.h>
#define STDIO_H
#endif

/*task object administration for self-terminating tasks*/
#define TEN_MINUTES 600000

struct _task{
    HANDLE taskHandle;
    unsigned taskId;		//voor NT's unieke threadnummer
    void* argPtr;		//maakt argument persistent
    int terminated;		//is de taak afgelopen?
    struct _task* next;
};

static struct _task* tStack=NULL;
static task* tStackTask=NULL;
static criticalSection tStackCS;

static unsigned __stdcall tStackTaskCode(void* arg){
    task* t=(task*)arg;
    while(!isTerminated_task(t)){
        struct _task* helpStack=NULL;
	Sleep(TEN_MINUTES);
	enter_criticalSection(&tStackCS);
	while(tStack!=NULL){
            struct _task* tPtr=tStack;
            tStack=tStack->next;
            if(WaitForSingleObject((HANDLE)(tPtr->taskHandle),0)==WAIT_TIMEOUT){
                /*task did not yet return, but has called terminate_task*/
		/*so free this task object the next time*/
                tPtr->next=helpStack;
                helpStack=tPtr;
            }else free(tPtr);
        }
        tStack=helpStack;
        leave_criticalSection(&tStackCS);
    }
    return 0;
}

void startNTK(void){
    create_criticalSection(&tStackCS);
    tStackTask=(task*)malloc(sizeof(task));
    create_task(tStackTask,tStackTaskCode,NULL,0,1);
    setPriority_task(tStackTask,low);
    resume_task(tStackTask);
}
#pragma startup startNTK 64

//---------------------------------------------------------------------------
//task implementaties -------------------------------------------------------
//first parameter must be a task* because of 4-th _beginthreadex parameter
void create_task(task* t,taskType code,void* arg,size_t argSize,int suspended){
    *t=(struct _task*)malloc(sizeof(struct _task));
    if(arg!=NULL){
        (*t)->argPtr=(char*)malloc(argSize);
        memcpy((*t)->argPtr,arg,argSize);
    }else (*t)->argPtr=NULL;
    (*t)->terminated=0;
    
    //if(suspended)(*t)->taskHandle=(HANDLE)_beginthreadex(NULL,0,code,t,CREATE_SUSPENDED,&((*t)->taskId));
    if(suspended)(*t)->taskHandle=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)code,t,CREATE_SUSPENDED,(LPDWORD)&((*t)->taskId));
else (*t)->taskHandle=(HANDLE)CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)code,t,0,(LPDWORD)&((*t)->taskId));
}

void delete_task(task* t){
    (*t)->terminated=1;
    WaitForSingleObject((HANDLE)((*t)->taskHandle),INFINITE);
    if((*t)->argPtr)free((*t)->argPtr);
    free(*t);
}

void terminate_task(task* t){
    struct _task* tS;
    if((*t)->argPtr)free((*t)->argPtr);
    //pop on the stack of selfterminated tasks
    enter_criticalSection(&tStackCS);
    tS=tStack;
    tStack=(*t);
    tStack->next=tS;
    leave_criticalSection(&tStackCS);
    free(t);
}

unsigned long suspend_task(task* t){
    return SuspendThread((*t)->taskHandle);
}

unsigned long resume_task(task* t){
    return ResumeThread((*t)->taskHandle);
}

int setPriority_task(task* t,priority p){
    return SetThreadPriority((*t)->taskHandle,(int)p);
}

priority getPriority_task(task* t){
    return (priority)GetThreadPriority((*t)->taskHandle);
}

void* getArgument_task(task* t){
    return (*t)->argPtr;
}

int isTerminated_task(task* t){
    return (*t)->terminated;
}
//---------------------------------------------------------------------------
//semphore implementaties ---------------------------------------------------
void create_semaphore(semaphore* s,long iVal,long mVal){
    *s=CreateSemaphore(NULL,iVal,mVal,NULL);
}

void delete_semaphore(semaphore* s){
    CloseHandle(*s);
}

void wait_semaphore(semaphore* s){
    WaitForSingleObject(*s,INFINITE);
}

unsigned long waitTimed_semaphore(semaphore* s,unsigned long timeOut){
    return WaitForSingleObject(*s,timeOut);
}

void signal_semaphore(semaphore* s){
    ReleaseSemaphore(*s,1,NULL);
}

//signaleer n keer
void signalN_semaphore(semaphore* s,long n){
    ReleaseSemaphore(*s,n,NULL);
}

//---------------------------------------------------------------------------
//criticalSection implementaties --------------------------------------------
void create_criticalSection(criticalSection* c){
    InitializeCriticalSection(c);
}

void delete_criticalSection(criticalSection* c){
    DeleteCriticalSection(c);
}

void leave_criticalSection(criticalSection* c){
    LeaveCriticalSection(c);
}

void enter_criticalSection(criticalSection* c){
    EnterCriticalSection(c);
}

//---------------------------------------------------------------------------
//mutual exclusion implementaties -------------------------------------------
void create_mutex(mutex* m){
    *m=CreateMutex(NULL,FALSE,NULL);
}

void delete_mutex(mutex* m){
    CloseHandle(*m);
}

unsigned long requestTimed_mutex(mutex* m,unsigned long timeOut){
    return WaitForSingleObject(*m,timeOut);
}

void request_mutex(mutex* m){
    WaitForSingleObject(*m,INFINITE);
}

void release_mutex(mutex* m){
    ReleaseMutex(*m);
}

//---------------------------------------------------------------------------
//event implementaties ------------------------------------------------------
struct _event{
    HANDLE theEvent;
    int persistent;
};

void create_event(event* e,int persist){
    *e=(struct _event*)malloc(sizeof(struct _event));
    (*e)->theEvent=CreateEvent(NULL,persist,FALSE,NULL);
    (*e)->persistent=persist;
}

void delete_event(event* e){
    CloseHandle((*e)->theEvent);
    free(*e);
}

void set_event(event* e){  		//signaleer gebeurtenis
    SetEvent((*e)->theEvent);
}

void reset_event(event* e){		//de-signaleer gebeurtenis
    ResetEvent((*e)->theEvent);
}

void pulse_event(event* e){    //set en reset, alle wachtende taken aktief
    PulseEvent((*e)->theEvent);
}

void wait_event(event* e){
    WaitForSingleObject((*e)->theEvent,INFINITE);
}

unsigned long	waitTimed_event(event* e,unsigned long timeOut){
    return WaitForSingleObject((*e)->theEvent,timeOut);
}

//---------------------------------------------------------------------------
//mailBox implementaties ---------------------------------------------------- 
struct _mailBox{
    unsigned        in,         //buffer pointers
                    out;
    unsigned        nElements;
    void*           theBuffer;  //dynamische buffer
    size_t          bufSize,    //bufferlengte
                    elSize;     //lengte van te plaatsen bericht
    semaphore       filled,     //synchronisatie volle en lege buffer
                    available;
    criticalSection csPut,	    //voor operaties op in en out
		    csGet;
};

static unsigned next(size_t s,unsigned i){ //next buffer index
    if(i<(s-1))return i+1;
    else return 0;
}

void create_mailBox(mailBox* b,size_t sizeOfBuffer,size_t sizeOfElement){
    *b=(struct _mailBox*)malloc(sizeof(struct _mailBox));
    (*b)->in=(*b)->out=0;
    (*b)->nElements=0;
    (*b)->bufSize=sizeOfBuffer;
    (*b)->elSize=sizeOfElement;
    (*b)->theBuffer=(char*)calloc(sizeOfBuffer,sizeOfElement);
    create_semaphore(&((*b)->filled),0,sizeOfBuffer);
    create_semaphore(&((*b)->available),sizeOfBuffer,sizeOfBuffer);
    create_criticalSection(&((*b)->csPut));
    create_criticalSection(&((*b)->csGet));
}

void delete_mailBox(mailBox* b){
    free((*b)->theBuffer);
    delete_semaphore(&((*b)->filled));
    delete_semaphore(&((*b)->available));
    delete_criticalSection(&((*b)->csPut));
    delete_criticalSection(&((*b)->csGet));
    free(*b);
}

//lees bericht
void get_mailBox(mailBox* b,void* mess){
    wait_semaphore(&((*b)->filled));
    enter_criticalSection(&((*b)->csGet));
    memcpy(mess,(void*)(((char*)((*b)->theBuffer))+((*b)->out*(*b)->elSize)),(*b)->elSize);
    (*b)->out=next((*b)->bufSize,(*b)->out);
    (*b)->nElements-=1;
    leave_criticalSection(&((*b)->csGet));
    signal_semaphore(&((*b)->available));
}

unsigned long getTimed_mailBox(mailBox* b,void* mess,unsigned long timeOut){
    if(waitTimed_semaphore(&((*b)->filled),timeOut)==WAIT_TIMEOUT) return WAIT_TIMEOUT;
    else{
        enter_criticalSection(&((*b)->csGet));
	memcpy(mess,(void*)(((char*)((*b)->theBuffer))+((*b)->out*(*b)->elSize)),(*b)->elSize);
	(*b)->out=next((*b)->bufSize,(*b)->out);
        (*b)->nElements-=1;
	leave_criticalSection(&((*b)->csGet));
	signal_semaphore(&((*b)->available));
	return WAIT_OBJECT_0;
    }
}

//plaats bericht
void put_mailBox(mailBox* b,void* mess){
    wait_semaphore(&((*b)->available));
    enter_criticalSection(&((*b)->csPut));
    memcpy((void*)(((char*)((*b)->theBuffer))+((*b)->in*(*b)->elSize)),mess,(*b)->elSize);
    (*b)->in=next((*b)->bufSize,(*b)->in);
    (*b)->nElements+=1;
    leave_criticalSection(&((*b)->csPut));
    signal_semaphore(&((*b)->filled));
}

unsigned long putTimed_mailBox(mailBox* b,void* mess,unsigned long timeOut){
    if(waitTimed_semaphore(&((*b)->available),timeOut)==WAIT_TIMEOUT) return WAIT_TIMEOUT;
    else{
        enter_criticalSection(&((*b)->csPut));
	memcpy((void*)(((char*)((*b)->theBuffer))+((*b)->in*(*b)->elSize)),mess,(*b)->elSize);
	(*b)->in=next((*b)->bufSize,(*b)->in);
        (*b)->nElements+=1;
	leave_criticalSection(&((*b)->csPut));
	signal_semaphore(&((*b)->filled));
	return WAIT_OBJECT_0;
    }
}

int isEmpty_mailBox(mailBox* b){
    return (*b)->nElements==0;
}

//---------------------------------------------------------------------------
//rendezVous implementaties -------------------------------------------------
struct _rendezVous{
    event  forSender,	//Sender op rendezVous aanwezig
	   forReceiver;	//Ontvanger op rendezVous aanwezig
    void*  message;	//uit te wisselen bericht
    size_t messLen;	//lengte van het bericht
};

void create_rendezVous(rendezVous* r){
    *r=(struct _rendezVous*)malloc(sizeof(struct _rendezVous));
    create_event(&((*r)->forSender),0);
    create_event(&((*r)->forReceiver),0);
}

void delete_rendezVous(rendezVous* r){
    delete_event(&((*r)->forSender));
    delete_event(&((*r)->forReceiver));
    free(*r);
}

void send_rendezVous(rendezVous* r,void* mess,size_t mLen){
    //plaats het bericht
    (*r)->message=mess;
    (*r)->messLen=mLen;
    set_event(&((*r)->forReceiver));		//ontvanger inlichten
    wait_event(&((*r)->forSender));	 	//wacht om te kunnen zenden
}

unsigned long sendTimed_rendezVous(rendezVous* r,void* mess,size_t mLen,unsigned long timeOut){
    //plaats het bericht
    (*r)->message=mess;
    (*r)->messLen=mLen;
    set_event(&((*r)->forReceiver));									//ontvanger inlichten
    //wacht om te kunnen zenden
    if(waitTimed_event(&((*r)->forSender),timeOut)==WAIT_TIMEOUT){
        /*hopelijk geen context switch hier!!*/
        reset_event(&((*r)->forReceiver));
        return WAIT_TIMEOUT;
    }else return WAIT_OBJECT_0;
}

void receive_rendezVous(rendezVous* r,void* mess){
    wait_event(&((*r)->forReceiver));	//wacht om te kunnen ontvangen
    memcpy(mess,(*r)->message,(*r)->messLen);					//ontvang het bericht
    set_event(&((*r)->forSender));	//zender van ontvangst inlichten
}

unsigned long receiveTimed_rendezVous(rendezVous* r,void* mess,unsigned long timeOut){
    if(waitTimed_event(&((*r)->forReceiver),timeOut)==WAIT_TIMEOUT){	//wacht om te kunnen ontvangen
        return WAIT_TIMEOUT;
    }
    else{
        memcpy(mess,(*r)->message,(*r)->messLen);					//ontvang het bericht
        set_event(&((*r)->forSender));	//zender van ontvangst inlichten
        return WAIT_OBJECT_0;
    }
}

//---------------------------------------------------------------------------
//rwLock implementaties -----------------------------------------------------
struct _rwLock{
    unsigned            writeCount,
                        readCount;
    criticalSection	wCount,
			rCount;
    event		w,r;
};

void create_rwLock(rwLock* l){
    *l=(struct _rwLock*)malloc(sizeof(struct _rwLock));
    (*l)->writeCount=(*l)->readCount=0;
    create_criticalSection(&((*l)->wCount));
    create_criticalSection(&((*l)->rCount));
    create_event(&((*l)->r),0);
    set_event(&((*l)->r));
    create_event(&((*l)->w),0);
    set_event(&((*l)->w));
}

void delete_rwLock(rwLock* l){
    delete_criticalSection(&((*l)->wCount));
    delete_criticalSection(&((*l)->rCount));
    delete_event(&((*l)->r));
    delete_event(&((*l)->w));
    free(*l);
}

void startReading_rwLock(rwLock* l){
    wait_event(&((*l)->r));
    enter_criticalSection(&((*l)->rCount));
    (*l)->readCount++;
    if((*l)->readCount==1)wait_event(&((*l)->w));
    leave_criticalSection(&((*l)->rCount));
    set_event(&((*l)->r));
}

void stopReading_rwLock(rwLock* l){
    enter_criticalSection(&((*l)->rCount));
    (*l)->readCount--;
    if((*l)->readCount==0)set_event(&((*l)->w));
    leave_criticalSection(&((*l)->rCount));
}

void startWriting_rwLock(rwLock* l){
    enter_criticalSection(&((*l)->wCount));
    (*l)->writeCount++;
    if((*l)->writeCount==1)wait_event(&((*l)->r));
    leave_criticalSection(&((*l)->wCount));
    wait_event(&((*l)->w));
}

void stopWriting_rwLock(rwLock* l){
    set_event(&((*l)->w));
    enter_criticalSection(&((*l)->wCount));
    (*l)->writeCount--;
    if((*l)->writeCount==0)set_event(&((*l)->r));
    leave_criticalSection(&((*l)->wCount));
}

//---------------------------------------------------------------------------
//STD implementaties --------------------------------------------------------
struct STDNode{
    int		    event;
    int		    nextState;
    actionType	    action;
    struct STDNode* nextSTDNode;
};
typedef struct STDNode	STDNode;
typedef struct STDNode*	STDNodePtr;

struct _STD{
    STDNodePtr* table;
    actionType defaultAction;
    int nrOfStates;
};

void create_STD(STD* s,int nStates,actionType defAct){
    int i;
    *s=(struct _STD*)malloc(sizeof(struct _STD));
    (*s)->table = (STDNodePtr*)calloc(nStates,sizeof(STDNodePtr));
    for(i=0;i<nStates;i++)(*s)->table[i]=NULL;
    (*s)->defaultAction=defAct;
    (*s)->nrOfStates=nStates;
}

void delete_STD(STD* s){
    int i;
    /*rij voor rij afbreken*/
    STDNodePtr m,p;
    for(i=0;i<(*s)->nrOfStates;i++){
        p=(*s)->table[i];
        while(p!=NULL){
            m=p;
            p=p->nextSTDNode;
            free(m);
        }
    }
    free((*s)->table);
    free(*s);
}

void addTransition_STD(STD* s,int state,int event,int nextState,actionType a){
    STDNodePtr m 		= (STDNodePtr)malloc(sizeof(STDNode));
    m->nextSTDNode	= (*s)->table[state];
    (*s)->table[state]	= m;
    m->event       	= event;
    m->nextState    = nextState;
    m->action       = a;
}

int lookUp_STD(STD* s,int currentState,int currentEvent,actionType* action){
    STDNodePtr p = (*s)->table[currentState];
    while((p!=NULL)&&(p->event!=currentEvent))p=p->nextSTDNode;
    if(p==NULL){
        /* do not change state, default action */
        *action=(*s)->defaultAction;
        return(currentState);
    }else{
        *action=p->action;
        return(p->nextState);
    }
}

//---------------------------------------------------------------------------
//eventStore implementaties -------------------------------------------------
#define xEQ0 10
#define xGT0 11

typedef enum {resetted, testing, waiting}	stateOfEventStore;
struct _eventStore{
    stateOfEventStore   state;
    int			theStore;
    int			clientEvent;
    void*		client;
    sendEventToClientF	send;
    eventStoreKind	kind;
};

static STD* eventStoreSTD=NULL;

void sendEvent_eventStore(eventStore*,eventForEventStore);

static void testX(void* e){
    eventStore* E=(eventStore*)(e);
    eventForEventStore event;
    if((*E)->theStore==0)event=xEQ0;
    else event=xGT0;
    sendEvent_eventStore(E,event);
}

static void accumulate(void* e){
    eventStore* E=(eventStore*)(e);
    (*E)->theStore++;
}

static void clearStore(void* e){
    eventStore* E=(eventStore*)(e);
    (*E)->theStore=0;
}

static void startWaiting(void* e){}

static void stopWaiting(void* e){}

static void pass(void* e){
    eventStore* E=(eventStore*)(e);
    (*E)->send((*E)->client,(*E)->clientEvent);
}

static void passAndReset(void* e){
    eventStore* E=(eventStore*)(e);
    (*E)->send((*E)->client,(*E)->clientEvent);
    if((*E)->kind==persistent)(*E)->theStore--;
    else (*E)->theStore=0;
}

static void defaultAction(void* e){}

void create_eventStore(eventStore* e,void* c,sendEventToClientF s,int cEvent,eventStoreKind k){
    *e=(struct _eventStore*)malloc(sizeof(struct _eventStore));
    if(eventStoreSTD==NULL){
        eventStoreSTD=(STD*)malloc(sizeof(STD));
  	create_STD(eventStoreSTD,3,defaultAction);
	addTransition_STD(eventStoreSTD,resetted,doWait,testing,testX);
  	addTransition_STD(eventStoreSTD,resetted,aSync,resetted,accumulate);
  	addTransition_STD(eventStoreSTD,resetted,doClear,resetted,clearStore);
  	addTransition_STD(eventStoreSTD,testing,xEQ0,waiting,startWaiting);
	addTransition_STD(eventStoreSTD,testing,xGT0,resetted,passAndReset);
  	addTransition_STD(eventStoreSTD,waiting,aSync,resetted,pass);
  	addTransition_STD(eventStoreSTD,waiting,doReset,resetted,stopWaiting);
  }
  (*e)->state=resetted;
  (*e)->theStore=0;
  (*e)->clientEvent=cEvent;
  (*e)->client=c;
  (*e)->send=s;
  (*e)->kind=k;
}

void delete_eventStore(eventStore* e){
    delete_STD(eventStoreSTD);
    free(*e);
}

void sendEvent_eventStore(eventStore* e,eventForEventStore ev){
    actionType action;
    (*e)->state=(stateOfEventStore)lookUp_STD(eventStoreSTD,(*e)->state,ev,&action);
    action(e);
}

bool hasEvent_eventStore(eventStore* e){
    return (*e)->theStore!=0;
}

//---------------------------------------------------------------------------
//cRandom implementaties ----------------------------------------------------
struct _cRandom{
    task* randomizerTask;
    mailBox randomNumbers;
    int theRange;
    int pseudoGen;
};

static unsigned __stdcall randomizerCode(void* arg){
    time_t T;
    int temp;
    task* t=(task*)(arg);
    cRandom* r=*((cRandom**)getArgument_task(t));
    if(!((*r)->pseudoGen))srand((unsigned)time(&T));
    while(!isTerminated_task(t)){
        temp=rand()%((*r)->theRange);
        put_mailBox(&((*r)->randomNumbers),&temp);
    }
    return 0;
}

void create_cRandom(cRandom* r,int range,int pseudoRandom){
    *r=(struct _cRandom*)malloc(sizeof(struct _cRandom));
    (*r)->theRange=range;
    (*r)->pseudoGen=pseudoRandom;
    create_mailBox(&((*r)->randomNumbers),10,sizeof(int));
    (*r)->randomizerTask=(task*)malloc(sizeof(task));
    create_task((*r)->randomizerTask,randomizerCode,&r,sizeof(cRandom*),0);
}

void delete_cRandom(cRandom* r){
    delete_task((*r)->randomizerTask);
    free((*r)->randomizerTask);
    delete_mailBox(&((*r)->randomNumbers));
    free(*r);
}

//retourneert een random number tussen 0 en range-1
int getNumber_cRandom(cRandom* r){
    int temp;
    get_mailBox(&((*r)->randomNumbers),&temp);
    return temp;
}
