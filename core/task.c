#include "param.h"
#include "types.h"


typedef struct{
	uint *sp;
	uint id;
	uint prio;
	uint status;
	char name[NAMELEN];
	struct task_t *next;
	struct task_t *prev;
} task_t;

typedef struct{
	task_t *first;
	task_t *last;
} task_queue_t;


static task_t task[TASK_COUNT];
static task_queue_t queue_ready[PRIO_COUNT];
static uint active_task;



int task_create(func fn, void *args, uint prio, void *stack){
	if(!fn){
		return -1;
	}

	interrupts_disable();
/*
	int i;
	for(i=0;i<TASK_COUNT;++i){
		if(task[i].status==TASK_DORMANT){
			if(i>=MAX_TASKS){
				interrupts_enable();
				return -2;
			}else{
				break;
			}
		}
	}

	task[i].sp=(uint*)(stack+i*STACK_SIZE+STACK_SIZE);
	task[i].sp-=sizeof(context_t);
	task[i].status=TASK_RUNNING;

	context_t *ctx=(context_t*)task[i].sp;
	context_create(ctx, fn, task_selfterminate);
*/
	interrupts_enable();
	return i;
}

