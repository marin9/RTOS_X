#pragma once
#include "task.h"
#include "types.h"


int os_task_create(func fn, void *args, uint prio, uint stack, char *name);
void os_task_yield();
void os_task_sleep(uint ms);
void os_task_exit(int ret);
int os_task_term(uint id);
int os_task_getname(uint id, char *name);
int os_task_getstat(uint id);
int os_task_getid();
int os_task_suspend(uint id);
int os_task_resume(uint id);
int os_task_set_prio(uint id, uint p);
int os_task_get_prio(uint id);

int os_task_signal(uint id);

int os_sem_init(uint id, uint v);
int os_sem_take(uint id);
int os_sem_give(uint id);
int os_sem_try(uint id);
