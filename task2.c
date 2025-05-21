#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>

#define ITER 10

static RT_TASK  t1;
static RT_TASK  t2;
static RT_SEM mutex1;
static RT_SEM mutex2;
int global = 0;

void taskOne(void *arg)
{
    int i;
    for (i=0; i < ITER; i++) {
	/*int rt_sem_p(RT_SEM *sem, RTIME timeout)*/
	rt_sem_p(&mutex1,TM_INFINITE);
        printf("I am taskOne and global = %d................\n", ++global);
	rt_sem_v(&mutex2);
    }
}

void taskTwo(void *arg)
{
    int i;
    for (i=0; i < ITER; i++) {
	/*int rt_sem_p(RT_SEM *sem, RTIME timeout)*/
	rt_sem_p(&mutex2,TM_INFINITE);
        printf("I am taskTwo and global = %d----------------\n", --global);
	rt_sem_v(&mutex1);
    }
}

int main(int argc, char* argv[]) {

    /*int rt_sem_create(RT_SEM *sem, const char *mutex, unsigned long icount, int mode)*/
    rt_sem_create(&mutex1, "mutex1", 1, S_FIFO);
    rt_sem_create(&mutex2, "mutex2", 0, S_FIFO);
    rt_task_create(&t1, "task1", 0, 1, 0);
    rt_task_create(&t2, "task2", 0, 1, 0);
    rt_task_start(&t1, &taskOne, 0);
    rt_task_start(&t2, &taskTwo, 0);
    sleep(5);
    rt_sem_delete(&mutex1);
    rt_sem_delete(&mutex2);
    return 0;
} 

