#pragma once
#include "types.h"

#define TASK_DORMANT	0
#define TASK_READY		1
#define TASK_SLEEP 		2
#define TASK_SUSPEND 	3
#define TASK_BLOCKSEM	4
#define TASK_BLOCKMUX	5
#define TASK_BLOCKCOND	6
#define TASK_BLOCKFLAG	7
#define TASK_BLOCKRECV	8	
#define TASK_BLOCKJOIN	9


typedef struct task_t{
	uint *sp;
	uint id;
	uint prio;
	uint sleep;
	uint status;
	char name[NAME_LEN];
	struct task_t *next;
	struct task_t *prev;
	task_t *join_q;
	int block_id;
	int retval;
} task_t;

typedef struct{
	task_t *first;
	task_t *last;
} task_queue_t;


void task_queue_init(task_queue_t *q);
void task_enqueue(task_queue_t *q, task_t *t);
task_t* task_dequeue(task_queue_t *q);
void task_remove(task_queue_t *q, task_t *t);
task_t* task_peek(task_queue_t *q);
void task_enqueue_sleep(task_queue_t *q, task_t *t, uint ticks);
void task_dequeue_sleep(task_queue_t *q, task_t *t);
