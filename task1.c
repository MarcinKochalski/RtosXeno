#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <alchemy/task.h>

#define period_ns 1000000000

RT_TASK hello_task;
RT_TASK periodic_task;

// function to be executed by task
void helloWorld(void *arg)
{
  RT_TASK_INFO curtaskinfo;
  printf("Hello World!\n");
  // inquire current task
  rt_task_inquire(NULL,&curtaskinfo);
  // print task name
  printf("Task name : %s \n", curtaskinfo.name);
}

void task_function(void *arg)
{
  rt_task_set_periodic(NULL, TM_NOW, period_ns);
  while (1) {
    printf("Hello World!\n");
    rt_task_wait_period(NULL);
  }
  return;
}

int main(int argc, char* argv[])
{
  char str[10];
  printf("start task\n");
  sprintf(str,"hello");
  /* Create task
   * Arguments: &task,
   *            name,
   *            stack size (0=default),
   *            priority,
   *            mode (FPU, start suspended, ...)
   */
  rt_task_create(&hello_task, str, 0, 50, 0);
  /*  Start task
   * Arguments: &task,
   *            task function,
   *            function argument
   */
  rt_task_start(&hello_task, &helloWorld, 0);
  rt_task_create(&periodic_task, "periodic", 0, 60, 0);
  rt_task_start(&periodic_task, &task_function, 0);
  pause();

  return 0;
}
