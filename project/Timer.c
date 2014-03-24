#include <stdlib.h>
#include <stdio.h>
#include "ntk.h"
#include "Timer.h"

#define TIMER_DEBUG 1

struct TimerMessage {
    int timer_state;
    void (*action)();
    unsigned long milliseconds;
};

void Timer_PrintMsg(struct TimerMessage *msg) {
    if (msg==NULL) {
        printf("msgNULL\n");
    } else {
        printf("msg@%d[timer_state=", msg);
        switch(msg->timer_state){
            case TIMER_START:
                printf("TIMER_START");
                break;
            case TIMER_STOP:
                printf("TIMER_STOP");
                break;
            default:
                printf("unknown");
                break;
        }
        printf(", milliseconds=%lu]\n",msg->milliseconds);
    }
}

static unsigned __stdcall Timer_Task(void* arg){
	task* t=(task*)arg;
	Timer *ptrTimer=(Timer*)(getArgument_task(t));
        
        struct TimerMessage *msg=NULL,*nextMsg;
        
	//Zolang het kruispunt nog bestaat, zal deze taak actief blijven
	while(!isTerminated_task(t)) {
            nextMsg=(struct TimerMessage*)malloc(sizeof(struct TimerMessage));
            
            if (TIMER_DEBUG) {
                printf("Timer_Task(): use ");
                Timer_PrintMsg(msg);
            }
            if (msg==NULL||msg->timer_state==TIMER_STOP) {
                if (TIMER_DEBUG){
                        printf("Timer_Task(): get_mailBox()...\n");
                }
                get_mailBox(&(ptrTimer->mailbox),nextMsg);
                printf("Timer_Task(): received ");
                Timer_PrintMsg(nextMsg);
                free(msg);
                msg=nextMsg;
            } else {
                if (TIMER_DEBUG){
                        printf("Timer_Task(): getTimed_mailBox()...\n");
                }
                if (getTimed_mailBox(&(ptrTimer->mailbox),nextMsg,msg->milliseconds)==WAIT_TIMEOUT){
                    
                    // perform timed action
                    msg->action();
                    
                    if (TIMER_DEBUG) {
                        printf("Timer_Task(): timer done after %u ms!\n", msg->milliseconds);
                    }
                    free(msg);
                    nextMsg=(struct TimerMessage*)malloc(sizeof(struct TimerMessage));
                    nextMsg->action=NULL;
                    nextMsg->timer_state=TIMER_STOP;
                    nextMsg->milliseconds=0;
                    msg=nextMsg;
                } else {
                    if (TIMER_DEBUG){
                        printf("Timer_Task(): timer stopped, received ");
                        Timer_PrintMsg(nextMsg);
                    }
                    free(msg);
                    msg=nextMsg;
                }
            }
            
            printf("\n");
	}

	return 0;
}


void Timer_Construct(Timer *ptrTimer) {
    //Mailbox aanmaken, waar 10 berichten in passen
    create_mailBox(&(ptrTimer->mailbox),10,sizeof(struct TimerMessage));

    //Taak creeren en starten voor de timer
    ptrTimer->ptrTask=(task*)malloc(sizeof(task));
    create_task(ptrTimer->ptrTask,Timer_Task,ptrTimer,sizeof(Timer),0);
}

void Timer_Start(Timer *ptrTimer, void (*action)(), unsigned long milliseconds) {
    struct TimerMessage *ptrMsg=(struct TimerMessage*)malloc(sizeof(struct TimerMessage));
    ptrMsg->action=action;
    ptrMsg->timer_state=TIMER_START;
    ptrMsg->milliseconds=milliseconds;
    put_mailBox(&(ptrTimer->mailbox),ptrMsg);
}

void Timer_Stop(Timer *ptrTimer) {
    struct TimerMessage *ptrMsg=(struct TimerMessage*)malloc(sizeof(struct TimerMessage));
    ptrMsg->action=NULL;
    ptrMsg->timer_state=TIMER_STOP;
    ptrMsg->milliseconds=0;
    put_mailBox(&(ptrTimer->mailbox),ptrMsg);
}
