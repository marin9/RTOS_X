#include "types.h"
#include "errno.h"
#include "task.h"
/*

typedef struct{
	uint value;
	task_queue_t wait_q;
} sem_t;


static sem_t sem[SEM_COUNT];


int sem_init(uint id, uint v){
	if(id>=SEM_COUNT){
		return -ERR_NORES;
	}

	sem[id].value=v;
	task_queue_init(&sem[id].wait_q);
	return 0;
}

int sem_take(uint id){
	if(id>=SEM_COUNT){
		return -1;
	}

	if(sem[id].value == 0){
		task_wait(&sem[id].wait_q, TASK_BLOCKSEM);
	}

	sem[id].value -= 1;
	return 0;
}

int sem_give(uint id){
	if(id>=SEM_COUNT){
		return -1;
	}

	if(!task_queue_empty(&sem[id].wait_q)){
		sem[id].value += 1;
	}else{
		task_wakeup(&sem[id].wait_q);
		task_yield();
	}
	return 0;
}

int sem_try(uint id){
	if(id>=SEM_COUNT){
		return -1;
	}

	if(sem[id].value == 0){
		return 1;
	}else{
		sem[id].value -= 1;
		return 0;
	}
}
*/