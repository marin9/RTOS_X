#include "cpu.h"
#include "task.h"
#include "time.h"
#include "param.h"
#include "types.h"
#include "errno.h"
#include "string.h"

static char* stack_next;
static char stack[STACK_SPACE] \
	__attribute__((aligned(8)));

static uint sched_running;
static uint active_task;
static task_t task[TASK_COUNT];
static task_queue_t queue_ready[PRIO_COUNT];
static task_queue_t sleep_queue;



void task_queue_init(task_queue_t *q){
	q->first=0;
	q->last=0;
}

void task_enqueue(task_queue_t *q, task_t *t){
	if(q->first==0){
		t->next=0;
		t->prev=0;

		q->first=t;
		q->last=t;
	}else{
		t->next=0;
		t->prev=q->last;
		q->last->next=t;
		q->last=t;
	}
}

task_t* task_dequeue(task_queue_t *q){
	task_t *tmp=q->first;

	if(!tmp){
		return 0;
	}

	q->first=tmp->next;

	if(q->first)
		q->first->prev=0;
	else
		q->last=0;

	if(q->first)

	tmp->next=0;
	tmp->prev=0;
	return tmp;
}

void task_remove(task_queue_t *q, task_t *t){
	if(q->first==t){
		task_dequeue(q);
	}else if(q->last==t){
		q->last=t->prev;
		q->last->next=0;
	}else{
		t->prev->next=t->next;
		t->next->prev=t->prev;
	}
	t->next=0;
	t->prev=0;
}



static void task_idle(){
	while(1);
}

static void task_reap(){
	interrupts_disable();
	task[active_task].status=TASK_DORMANT;
	task_yield();
}

void task_init(){
	int i;

	for(i=0;i<PRIO_COUNT;++i){
		task_queue_init(&queue_ready[i]);
	}

	task_queue_init(&sleep_queue);

	for(i=0;i<TASK_COUNT;++i){
		task[i].status=0;
	}

	stack_next=stack;
	active_task=0;
	sched_running=0;
	task_create(task_idle, 0, 0, 64, "idle");
}

void task_sched_start(){
	int i;
	task_t *new;

	sched_running=1;

	for(i=PRIO_COUNT-1;i>=0;--i){
		new=task_dequeue(&queue_ready[i]);
		if(new){
			break;
		}
	}
	
	active_task=new->id;
	context_switch(0, new);
}

int task_create(func fn, void *args, uint prio, uint stack, char *name){
	int i;

	if(!fn){
		return -ERR_ARGS;
	}

	for(i=0;i<TASK_COUNT;++i){
		if(!task[i].status){
			if(i>=TASK_COUNT){
				return -ERR_NORES;
			}else{
				break;
			}
		}
	}

	task[i].id=i;
	task[i].prio=prio;
	task[i].sp=(uint*)(stack_next+stack-4);
	task[i].sp-=sizeof(context_t);
	task[i].status=TASK_READY;

	if(name)
		strcpy(task[i].name, name);
	else
		strcpy(task[i].name, "no_name");

	stack_next+=stack;

	context_t *ctx=(context_t*)task[i].sp;
	context_create(ctx, fn, args, task_reap);

	task_enqueue(&queue_ready[prio], &task[i]);
	return i;
}

int task_getname(uint id, char *name){
	if(id>TASK_COUNT){
		return -ERR_NOEXIST;
	}

	if(task[id].status==TASK_DORMANT){
		return -ERR_NORES;
	}

	strcpy(name, task[id].name);
	return 0;
}

int task_getstat(uint id){
	if(id>TASK_COUNT)
		return -ERR_NOEXIST;
	else
		return task[id].status;
}

int task_term(uint id){
	if(id==active_task){
		task_reap();
	}

	if(id>TASK_COUNT){
		return -ERR_NOEXIST;
	}

	if(task[id].status!=TASK_DORMANT){
		task[id].status=TASK_DORMANT;
		task_remove(&queue_ready[task[id].prio], &task[id]);
		task_yield();
	}

	return 0;
}

int task_getid(){
	return active_task;
}

void task_yield(){
	int i;
	task_t *old;
	task_t *new;

	if(!sched_running){
		return;
	}

	task_wakeup(&sleep_queue);

	old=&task[active_task];
	if(old->status==TASK_READY)
		task_enqueue(&queue_ready[old->prio], old);

	for(i=PRIO_COUNT-1;i>=0;--i){
		new=task_dequeue(&queue_ready[i]);
		if(new){
			break;
		}
	}
	active_task=new->id;
	context_switch(old, new);
}

void task_exit(){
	task_reap();
}

int task_suspend(uint id){
	if(id>TASK_COUNT){
		return -ERR_NOEXIST;
	}

	if(id==active_task){
		task[id].status=TASK_SUSPEND;
		task_yield();
		return 0;
	}

	if(task[id].status==TASK_READY){
		task[id].status=TASK_SUSPEND;
		task_remove(&queue_ready[task[id].prio], &task[id]);
		task_yield();
		return 0;
	}else{
		return -ERR_ILSTAT;
	}
}

int task_resume(uint id){
	if(id>TASK_COUNT){
		return -ERR_NOEXIST;
	}

	if(task[id].status!=TASK_SUSPEND){
		return -ERR_ILSTAT;
	}

	task[id].status=TASK_READY;
	task_enqueue(&queue_ready[task[id].prio], &task[id]);
	task_yield();
	return 0;
}

int task_set_prio(uint id, uint p){
	if(id>TASK_COUNT){
		return -ERR_NOEXIST;
	}

	if(task[id].status!=TASK_READY){
		return -ERR_ILSTAT;
	}

	if(id!=active_task){
		task_remove(&queue_ready[task[id].prio], &task[id]);
		task[id].prio=p;
		task_enqueue(&queue_ready[p], &task[id]);
	}else{
		task[active_task].prio=p;
		task_yield();
	}
	return 0;
}

int task_get_prio(uint id){
	if(id>TASK_COUNT){
		return -ERR_NOEXIST;
	}
	return task[id].prio;
}

void task_wait(task_queue_t *q){
	int i;
	task_t *old;
	task_t *new;

	if(!sched_running){
		return;
	}

	old=&task[active_task];
	task_enqueue(q, old);

	for(i=PRIO_COUNT-1;i>=0;--i){
		new=task_dequeue(&queue_ready[i]);
		if(new){
			break;
		}
	}
	active_task=new->id;
	context_switch(old, new);
}

int task_wakeup(task_queue_t *q){
	task_t *t;

	t=task_dequeue(q);
	if(!t){
		return -ERR_NORES;
	}

	task_enqueue(&queue_ready[t->prio], &task[t->id]);
	return 0;
}

int task_wakeup_all(task_queue_t *q){
	while(!task_wakeup(q));
	return 0;
}

int task_sleep(uint ms){
	uint rest;
	uint tv=time_get_ticks()+ms;

	task[active_task].status=TASK_SLEEP;

	while(tv > time_get_ticks()){
		if(task[active_task].status != TASK_SLEEP){
			break;
		}
		task_wait(&sleep_queue);
	}

	task[active_task].status=TASK_READY;

	rest=tv-time_get_ticks();
	if(rest <= 0){
		return 0;
	}else{
		return rest;
	}
}

int task_signal(uint id){
	if(task[id].status != TASK_SLEEP){
		return -ERR_ILSTAT;
	}

	task[id].status=TASK_READY;
	task_yield();
	return 0;
}
